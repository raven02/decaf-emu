#include "interpreter.h"
#include <cfenv>
#include "interpreter_insreg.h"
#include "cpu/espresso/espresso_instructionset.h"
#include "../trace.h"
#include "../cpu_internal.h"
#include "debugcontrol.h"
#include "mem/mem.h"
#include "utils/log.h"

namespace cpu
{

namespace interpreter
{

static std::vector<instrfptr_t>
sInstructionMap;

void initialise()
{
   sInstructionMap.resize(static_cast<size_t>(espresso::InstructionID::InstructionCount), nullptr);

   // Register instruction handlers
   registerBranchInstructions();
   registerConditionInstructions();
   registerFloatInstructions();
   registerIntegerInstructions();
   registerLoadStoreInstructions();
   registerPairedInstructions();
   registerSystemInstructions();
}

instrfptr_t getInstructionHandler(espresso::InstructionID id)
{
   auto instrId = static_cast<size_t>(id);

   if (instrId >= sInstructionMap.size()) {
      return nullptr;
   }

   return sInstructionMap[instrId];
}

void registerInstruction(espresso::InstructionID id, instrfptr_t fptr)
{
   sInstructionMap[static_cast<size_t>(id)] = fptr;
}

bool hasInstruction(espresso::InstructionID id)
{
   return getInstructionHandler(id) != nullptr;
}

void step_one(Core *core)
{
   if (core->interruptEnabled.load(std::memory_order_acquire)) {
      uint32_t interrupt_flags = core->interrupt.exchange(0);

      if (interrupt_flags != 0) {
         cpu::gInterruptHandler(interrupt_flags);
      }
   }

   core->cia = core->nia;
   core->nia = core->cia + 4;

   auto instr = mem::read<espresso::Instruction>(core->cia);
   auto data = espresso::decodeInstruction(instr);

   if (!data) {
      gLog->error("Could not decode instruction at {:08x} = {:08x}", core->cia, instr.value);
   }
   assert(data);

   auto trace = traceInstructionStart(instr, data, core);
   auto fptr = sInstructionMap[static_cast<size_t>(data->id)];

   if (!fptr) {
      gLog->error("Unimplemented interpreter instruction {}", data->name);
   }
   assert(fptr);

   fptr(core, instr);
   traceInstructionEnd(trace, instr, data, core);
}

void resume(Core *core)
{
   // Before we resume, we need to update our states!
   cpu::update_rounding_mode(core);
   std::feclearexcept(FE_ALL_EXCEPT);

   while (core->nia != cpu::CALLBACK_ADDR) {
      step_one(core);
   }
}

} // namespace interpreter

} // namespace cpu
