#include <cassert>
#include <cfenv>
#include <fstream>
#include "cpu/cpu.h"
#include "cpu/jit/jit.h"
#include "hardwaretests.h"
#include "mem/mem.h"
#include "utils/bit_cast.h"
#include "utils/floatutils.h"
#include "utils/log.h"
#include "utils/strutils.h"
#include "filesystem/filesystem.h"

#include "cpu/espresso/espresso_disassembler.h"
#include "cpu/espresso/espresso_instructionset.h"
#include "cpu/espresso/espresso_registers.h"

using namespace espresso;

static const auto TEST_FPSCR = true;
static const auto TEST_FPSCR_FR = false;

namespace hwtest
{

static void
printTestField(InstructionField field, Instruction instr, RegisterState *input, RegisterState *output, cpu::Core *state)
{
   auto printGPR = [&](uint32_t reg) {
      assert(reg >= GPR_BASE);

      gLog->debug("r{:02d}    =         {:08X}         {:08X}         {:08X}", reg,
                  input->gpr[reg - GPR_BASE],
                  output->gpr[reg - GPR_BASE],
                  state->gpr[reg]);
   };

   auto printFPR = [&](uint32_t reg) {
      assert(reg >= FPR_BASE);

      gLog->debug("f{:02d}    = {:16e} {:16e} {:16e}", reg,
                  input->fr[reg - FPR_BASE],
                  output->fr[reg - FPR_BASE],
                  state->fpr[reg].value);

      gLog->debug("         {:16X} {:16X} {:16X}",
                  bit_cast<uint64_t>(input->fr[reg - FPR_BASE]),
                  bit_cast<uint64_t>(output->fr[reg - FPR_BASE]),
                  bit_cast<uint64_t>(state->fpr[reg].value));
   };

   switch (field) {
   case InstructionField::rA:
      printGPR(instr.rA);
      break;
   case InstructionField::rB:
      printGPR(instr.rB);
      break;
   case InstructionField::rD:
      printGPR(instr.rS);
      break;
   case InstructionField::rS:
      printGPR(instr.rS);
      break;
   case InstructionField::frA:
      printFPR(instr.frA);
      break;
   case InstructionField::frB:
      printFPR(instr.frB);
      break;
   case InstructionField::frC:
      printFPR(instr.frC);
      break;
   case InstructionField::frD:
      printFPR(instr.frD);
      break;
   case InstructionField::frS:
      printFPR(instr.frS);
      break;
   case InstructionField::XERC:
      gLog->debug("xer.ca =         {:08X}         {:08X}         {:08X}", input->xer.ca, output->xer.ca, state->xer.ca);
      break;
   case InstructionField::XERSO:
      gLog->debug("xer.so =         {:08X}         {:08X}         {:08X}", input->xer.so, output->xer.so, state->xer.so);
      break;
   case InstructionField::FPSCR:
      gLog->debug("fpscr =          {:08X}         {:08x}         {:08X}", input->fpscr.value, output->fpscr.value, state->fpscr.value);
      break;
   default:
      break;
   }
}

#define CompareFPSCRField(field) \
   if (result.field != expected.field) { \
      gLog->debug("fpscr." #field " = {} {} {}", input.field, expected.field, result.field); \
      failed = true; \
   }

// Compare all individual fields in fpscr
static bool
compareFPSCR(fpscr_t input, fpscr_t expected, fpscr_t result)
{
   auto failed = false;
   CompareFPSCRField(rn);
   CompareFPSCRField(ni);
   CompareFPSCRField(xe);
   CompareFPSCRField(ze);
   CompareFPSCRField(ue);
   CompareFPSCRField(oe);
   CompareFPSCRField(ve);
   CompareFPSCRField(vxcvi);
   CompareFPSCRField(vxsqrt);
   CompareFPSCRField(vxsoft);
   CompareFPSCRField(fprf);
   CompareFPSCRField(fi);
   CompareFPSCRField(fr);
   CompareFPSCRField(vxvc);
   CompareFPSCRField(vximz);
   CompareFPSCRField(vxzdz);
   CompareFPSCRField(vxidi);
   CompareFPSCRField(vxisi);
   CompareFPSCRField(vxsnan);
   CompareFPSCRField(xx);
   CompareFPSCRField(zx);
   CompareFPSCRField(ux);
   CompareFPSCRField(ox);
   CompareFPSCRField(vx);
   CompareFPSCRField(fex);
   CompareFPSCRField(fx);
   return failed;
}

bool runTests(const std::string &path)
{
   uint32_t testsFailed = 0, testsPassed = 0;
   uint32_t baseAddress = mem::ApplicationBase;

   Instruction bclr = encodeInstruction(InstructionID::bclr);
   bclr.bo = 0x1f;
   mem::write(baseAddress + 4, bclr.value);

   fs::FileSystem filesystem;
   fs::FolderEntry entry;
   fs::HostPath base = path;
   filesystem.mountHostFolder("/tests", base);
   auto folder = filesystem.openFolder("/tests");

   while (folder->read(entry)) {
      std::ifstream file(base.join(entry.name).path(), std::ifstream::in | std::ifstream::binary);
      cereal::BinaryInputArchive cerealInput(file);
      TestFile testFile;

      // Parse test file with cereal
      testFile.name = entry.name;
      cerealInput(testFile);

      // Run tests
      gLog->info("Checking {}", testFile.name);

      for (auto &test : testFile.tests) {
         cpu::Core state;
         bool failed = false;

         // Setup thread state from test input
         memset(&state, 0, sizeof(cpu::Core));
         state.cia = 0;
         state.nia = baseAddress;
         state.xer = test.input.xer;
         state.cr = test.input.cr;
         state.fpscr = test.input.fpscr;
         state.ctr = test.input.ctr;

         for (auto i = 0; i < 4; ++i) {
            state.gpr[i + 3] = test.input.gpr[i];
            state.fpr[i + 1].paired0 = test.input.fr[i];
         }

         // Execute test
         mem::write(baseAddress, test.instr.value);
         cpu::jit::clearCache();
         std::feclearexcept(FE_ALL_EXCEPT);
         // TODO: Fix This!
         //cpu::executeSub(nullptr, &state);

         // Check XER (all bits)
         if (state.xer.value != test.output.xer.value) {
            gLog->error("Test failed, xer expected {:08X} found {:08X}", test.output.xer.value, state.xer.value);
            failed = true;
         }

         // Check Condition Register (all bits)
         if (state.cr.value != test.output.cr.value) {
            gLog->error("Test failed, cr expected {:08X} found {:08X}", test.output.cr.value, state.cr.value);
            failed = true;
         }

         // Check FPSCR (all bits except possibly FR)
         if (TEST_FPSCR) {
            auto state_fpscr = state.fpscr.value;
            auto test_fpscr = test.output.fpscr.value;
            if (!TEST_FPSCR_FR) {
               state_fpscr &= ~0x00040000;
               test_fpscr &= ~0x00040000;
            }
            if (state_fpscr != test_fpscr) {
               gLog->error("Test failed, fpscr {:08X} found {:08X}", test.output.fpscr.value, state.fpscr.value);
               failed = true;
            }
         }

         // Check CTR
         if (state.ctr != test.output.ctr) {
            gLog->error("Test failed, ctr expected {:08X} found {:08X}", test.output.ctr, state.ctr);
            failed = true;
         }

         // Check all GPR
         for (auto i = 0; i < 4; ++i) {
            auto reg = i + hwtest::GPR_BASE;
            auto value = state.gpr[reg];
            auto expected = test.output.gpr[i];

            if (value != expected) {
               gLog->error("Test failed, r{} expected {:08X} found {:08X}", reg, expected, value);
               failed = true;
            }
         }

         // Check all FPR
         for (auto i = 0; i < 4; ++i) {
            auto reg = i + hwtest::FPR_BASE;
            auto value = state.fpr[reg].value;
            auto expected = test.output.fr[i];

            if (!is_nan(value) && !is_nan(expected) && !is_infinity(value) && !is_infinity(expected)) {
               double dval = value / expected;

               if (dval < 0.999 || dval > 1.001) {
                  gLog->error("Test failed, f{} expected {:16f} found {:16f}", reg, expected, value);
                  failed = true;
               }
            } else {
               if (is_nan(value) && is_nan(expected)) {
                  auto bits = get_float_bits(value);
                  bits.sign = get_float_bits(expected).sign;
                  value = bits.v;
               }

               if (bit_cast<uint64_t>(value) != bit_cast<uint64_t>(expected)) {
                  gLog->error("Test failed, f{} expected {:16X} found {:16X}", reg, bit_cast<uint64_t>(expected), bit_cast<uint64_t>(value));
                  failed = true;
               }
            }
         }

         if (failed) {
            Disassembly dis;

            // Print disassembly
            disassemble(test.instr, dis, baseAddress);
            gLog->debug(dis.text);

            // Print all test fields
            gLog->debug("{:08x}            Input         Hardware           Interp", test.instr.value);

            for (auto field : dis.instruction->read) {
               printTestField(field, test.instr, &test.input, &test.output, &state);
            }

            for (auto field : dis.instruction->write) {
               printTestField(field, test.instr, &test.input, &test.output, &state);
            }

            for (auto field : dis.instruction->flags) {
               printTestField(field, test.instr, &test.input, &test.output, &state);
            }

            gLog->debug("");
            ++testsFailed;
         } else {
            ++testsPassed;
         }
      }
   }

   gLog->info("Passed: {}, Failed: {}", testsPassed, testsFailed);
   return true;
}

} // namespace hwtest
