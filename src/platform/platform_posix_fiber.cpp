#include "platform.h"
#include "platform_fiber.h"
#include "utils/log.h"

#ifdef PLATFORM_POSIX
#include <array>
#include <errno.h>
#include <signal.h>
#include <ucontext.h>

namespace platform
{

static const size_t
DefaultStackSize = 1024 * 1024;

struct Fiber
{
   ucontext_t context;
   FiberEntryPoint entry = nullptr;
   void *entryParam = nullptr;
   std::array<char, DefaultStackSize> stack;
};

Fiber *
getThreadFiber()
{
   auto fiber = new Fiber();
   return fiber;
}

static void
fiberEntryPoint(Fiber *fiber)
{
   fiber->entry(fiber->entryParam);
}

Fiber *
createFiber(FiberEntryPoint entry, void *entryParam)
{
   auto fiber = new Fiber();
   fiber->entry = entry;
   fiber->entryParam = entryParam;

   getcontext(&fiber->context);
   fiber->context.uc_stack.ss_sp = &fiber->stack[0];
   fiber->context.uc_stack.ss_size = fiber->stack.size();
   fiber->context.uc_link = NULL;

   makecontext(&fiber->context, reinterpret_cast<void(*)()>(&fiberEntryPoint), 1, fiber);
   return fiber;
}

void
destroyFiber(Fiber *fiber)
{
   delete fiber;
}

void
swapToFiber(Fiber *current, Fiber *target)
{
   if (!current) {
      setcontext(&target->context);
   } else {
      swapcontext(&current->context, &target->context);
   }
}

} // namespace platform

#endif
