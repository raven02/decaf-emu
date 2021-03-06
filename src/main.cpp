#include <pugixml.hpp>
#include <excmd.h>
#include "config.h"
#include "utils/bitutils.h"
#include "cpu/cpu.h"
#include "cpu/trace.h"
#include "cpu/jit/jit.h"
#include "debugger.h"
#include "fuzztests.h"
#include "filesystem/filesystem.h"
#include "gpu/opengl/opengl_driver.h"
#include "hardwaretests.h"
#include "input/input.h"
#include "loader.h"
#include "mem/mem.h"
#include "modules/coreinit/coreinit.h"
#include "modules/coreinit/coreinit_core.h"
#include "modules/coreinit/coreinit_memheap.h"
#include "modules/coreinit/coreinit_scheduler.h"
#include "modules/coreinit/coreinit_systeminfo.h"
#include "modules/erreula/erreula.h"
#include "modules/gx2/gx2.h"
#include "modules/mic/mic.h"
#include "modules/nn_ac/nn_ac.h"
#include "modules/nn_acp/nn_acp.h"
#include "modules/nn_act/nn_act.h"
#include "modules/nn_boss/nn_boss.h"
#include "modules/nn_fp/nn_fp.h"
#include "modules/nn_ndm/nn_ndm.h"
#include "modules/nn_nfp/nn_nfp.h"
#include "modules/nn_olv/nn_olv.h"
#include "modules/nn_save/nn_save.h"
#include "modules/nn_temp/nn_temp.h"
#include "modules/nsysnet/nsysnet.h"
#include "modules/proc_ui/proc_ui.h"
#include "modules/padscore/padscore.h"
#include "modules/snd_core/snd_core.h"
#include "modules/swkbd/swkbd.h"
#include "modules/sysapp/sysapp.h"
#include "modules/vpad/vpad.h"
#include "modules/zlib125/zlib125.h"
#include "platform/platform_dir.h"
#include "platform/platform_glfw.h"
#include "platform/platform_sdl.h"
#include "platform/platform_ui.h"
#include "system.h"
#include "usermodule.h"
#include "utils/log.h"
#include "utils/teenyheap.h"
#include "kernel/kernel.h"

static void
initialiseEmulator();

static bool
play(const fs::HostPath &path);

static const std::string
getGameName(const fs::HostPath &path)
{
   return path.filename();
}

static excmd::parser
getCommandLineParser()
{
   excmd::parser parser;
   using excmd::description;
   using excmd::optional;
   using excmd::default_value;
   using excmd::allowed;
   using excmd::value;

   parser.global_options()
      .add_option("v,version", description { "Show version." })
      .add_option("h,help", description { "Show help." });

   parser.add_command("help")
      .add_argument("help-command", optional {}, value<std::string> {});

   auto jit_options = parser.add_option_group("JIT Options")
      .add_option("jit", description { "Enables the JIT engine." })
      .add_option("jit-debug", description { "Verify JIT implementation against interpreter." });

   auto log_options = parser.add_option_group("Log Options")
      .add_option("log-file", description { "Redirect log output to file." })
      .add_option("log-async", description { "Enable asynchronous logging." })
      .add_option("log-no-stdout", description { "Disable logging to stdout." })
      .add_option("log-level", description { "Only display logs with severity equal to or greater than this level." },
                  default_value<std::string> { "trace" },
                  allowed<std::string> { {
                     "trace", "debug", "info", "notice", "warning", "error", "critical", "alert", "emerg", "off"
                  } });

   auto sys_options = parser.add_option_group("System Options")
      .add_option("sys-path", description { "Where to locate any external system files." }, value<std::string> {});

   parser.add_command("play")
      .add_option_group(jit_options)
      .add_option_group(log_options)
      .add_option_group(sys_options)
      .add_argument("game directory", value<std::string> {});

   parser.add_command("fuzztest");

   parser.add_command("hwtest")
      .add_option_group(jit_options)
      .add_option_group(log_options);

   return parser;
}

int main(int argc, char **argv)
{
   auto parser = getCommandLineParser();
   excmd::option_state options;
   auto result = -1;

   try {
      options = parser.parse(argc, argv);
   } catch (excmd::exception ex) {
      std::cout << "Error parsing options: " << ex.what() << std::endl;
      std::exit(-1);
   }

   // Print version
   if (options.has("version")) {
      // TODO: print git hash
      std::cout << "Decaf Emulator version 0.0.1" << std::endl;
      std::exit(0);
   }

   // Print help
   if (argc == 1 || options.has("help")) {
      if (options.has("help-command")) {
         std::cout << parser.format_help("decaf", options.get<std::string>("help-command")) << std::endl;
      } else {
         std::cout << parser.format_help("decaf") << std::endl;
      }

      std::exit(0);
   }

   // First thing, load the config!
   config::load("config.json");

   // Allow command line options to override config
   if (options.has("jit-debug")) {
      config::jit::debug = true;
   }

   if (options.has("jit")) {
      config::jit::enabled = true;
   }

   if (options.has("log-no-stdout")) {
      config::log::to_stdout = true;
   }

   if (options.has("log-file")) {
      config::log::to_file = true;
   }

   if (options.has("log-async")) {
      config::log::async = true;
   }

   if (options.has("log-level")) {
      config::log::level = options.get<std::string>("log-level");
   }

   if (options.has("sys-path")) {
      config::system::system_path = options.get<std::string>("sys-path");
   }

   // Set log filename
   std::string logFilename;

   if (options.has("play")) {
      logFilename = getGameName(options.get<std::string>("game directory"));
   } else if (options.has("hwtest")) {
      logFilename = "hwtest";
   } else {
      logFilename = "log";
   }

   // Start!
   logging::initialise(logFilename);
   initialiseEmulator();

   if (options.has("play")) {
      gLog->set_pattern("[%l:%t] %v");
      result = play(options.get<std::string>("game directory"));
   } else if (options.has("fuzztest")) {
      gLog->set_pattern("%v");
      result = executeFuzzTests();
   } else if (options.has("hwtest")) {
      gLog->set_pattern("%v");
      result = hwtest::runTests("tests/cpu/wiiu");
   }

#ifdef PLATFORM_WINDOWS
   system("PAUSE");
#endif
   return result ? 0 : -1;
}

static void
initialiseEmulator()
{
   // Setup jit from config
   if (config::jit::enabled) {
      if (config::jit::debug) {
         cpu::set_jit_mode(cpu::jit_mode::debug);
      } else {
         cpu::set_jit_mode(cpu::jit_mode::enabled);
      }
   } else {
      cpu::set_jit_mode(cpu::jit_mode::disabled);
   }

   // Setup core
   mem::initialise();
   cpu::initialise();
   kernel::initialise();

   // Kernel modules
   coreinit::Module::RegisterFunctions();
   nn::erreula::Module::RegisterFunctions();
   gx2::Module::RegisterFunctions();
   mic::Module::RegisterFunctions();
   nn::ac::Module::RegisterFunctions();
   nn::acp::Module::RegisterFunctions();
   nn::act::Module::RegisterFunctions();
   nn::boss::Module::RegisterFunctions();
   nn::fp::Module::RegisterFunctions();
   nn::ndm::Module::RegisterFunctions();
   nn::nfp::Module::RegisterFunctions();
   nn::olv::Module::RegisterFunctions();
   nn::save::Module::RegisterFunctions();
   nn::temp::Module::RegisterFunctions();
   nsysnet::Module::RegisterFunctions();
   padscore::Module::RegisterFunctions();
   proc_ui::Module::RegisterFunctions();
   snd_core::Module::RegisterFunctions();
   nn::swkbd::Module::RegisterFunctions();
   sysapp::Module::RegisterFunctions();
   vpad::Module::RegisterFunctions();
   zlib125::Module::RegisterFunctions();

   // Initialise emulator systems
   gSystem.initialise();
   gSystem.registerModule("coreinit.rpl", new coreinit::Module {});
   gSystem.registerModule("erreula.rpl", new nn::erreula::Module {});
   gSystem.registerModule("gx2.rpl", new gx2::Module {});
   gSystem.registerModule("mic.rpl", new mic::Module {});
   gSystem.registerModule("nn_ac.rpl", new nn::ac::Module {});
   gSystem.registerModule("nn_acp.rpl", new nn::acp::Module {});
   gSystem.registerModule("nn_act.rpl", new nn::act::Module {});
   gSystem.registerModule("nn_boss.rpl", new nn::boss::Module {});
   gSystem.registerModule("nn_fp.rpl", new nn::fp::Module {});
   gSystem.registerModule("nn_nfp.rpl", new nn::nfp::Module {});
   gSystem.registerModule("nn_ndm.rpl", new nn::ndm::Module {});
   gSystem.registerModule("nn_olv.rpl", new nn::olv::Module {});
   gSystem.registerModule("nn_save.rpl", new nn::save::Module {});
   gSystem.registerModule("nn_temp.rpl", new nn::temp::Module {});
   gSystem.registerModule("nsysnet.rpl", new nsysnet::Module {});
   gSystem.registerModule("padscore.rpl", new padscore::Module {});
   gSystem.registerModule("proc_ui.rpl", new proc_ui::Module {});
   gSystem.registerModule("snd_core.rpl", new snd_core::Module {});
   gSystem.registerModuleAlias("snd_core.rpl", "sndcore2.rpl");
   gSystem.registerModule("swkbd.rpl", new nn::swkbd::Module {});
   gSystem.registerModule("sysapp.rpl", new sysapp::Module {});
   gSystem.registerModule("vpad.rpl", new vpad::Module {});
   gSystem.registerModule("zlib125.rpl", new zlib125::Module {});

   // Initialise debugger
   gDebugger.initialise();
}

static bool
play(const fs::HostPath &path)
{
   // Create gpu driver
   gpu::driver::create(new gpu::opengl::GLDriver);

   // Create window
#ifdef DECAF_GLFW
   if (config::system::platform.compare("glfw") == 0) {
      gLog->info("Using GLFW");
      platform::setPlatform(new platform::PlatformGLFW);
   } else
#endif
#ifdef DECAF_SDL
   if (config::system::platform.compare("sdl") == 0) {
      gLog->info("Using SDL");
      platform::setPlatform(new platform::PlatformSDL);
   } else
#endif
   {
      gLog->error("Unsupported platform {}");
      return false;
   }

   if (!platform::ui::init()) {
      gLog->error("Error initializing UI");
      return false;
   }

   if (!platform::ui::createWindows("Decaf", "Decaf - Gamepad")) {
      gLog->error("Error creating window");
      return false;
   }

   if (!input::init()) {
      gLog->error("Error setting up input");
      return false;
   }

   // Setup filesystem
   fs::FileSystem fs;
   fs::HostPath sysPath = config::system::system_path;

   if (platform::isDirectory(path.path())) {
      // See if we can find path/cos.xml
      fs.mountHostFolder("/vol", path);
      auto fh = fs.openFile("/vol/code/cos.xml", fs::File::Read);

      if (fh) {
         fh->close();
         delete fh;
      } else {
         // Try path/data
         fs.deleteFolder("/vol");
         fs.mountHostFolder("/vol", path.join("data"));
      }
   } else if (platform::isFile(path.path())) {
      // Load game file, currently only .rpx is supported
      // TODO: Support .WUD .WUX
      if (path.extension().compare("rpx") == 0) {
         fs.mountHostFile("/vol/code/" + path.filename(), path);
      } else {
         gLog->error("Only loading files with .rpx extension is currently supported {}", path.path());
         return false;
      }
   } else {
      gLog->error("Could not find file or directory {}", path.path());
      return false;
   }

   gSystem.setFileSystem(&fs);
   kernel::set_game_name(path.filename());

   // Lock out some memory for unimplemented data access
   mem::protect(0xfff00000, 0x000fffff);

   // Mount system path
   fs.mountHostFolder("/vol/storage_mlc01", sysPath.join("mlc"));

   // Start up our CPUs
   cpu::start();

   platform::ui::run();

   cpu::halt();

   platform::ui::shutdown();
   config::save("config.json");

   // Force inclusion in release builds
   volatile int zero = 0;
   if (zero) {
      tracePrint(nullptr, 0, 0);
      tracePrintSyscall(0);
      fallbacksPrint();
   }

   // TODO: OSFreeToSystem data
   return true;
}
