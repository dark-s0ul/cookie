#include "coroutine.h"

thread_local coroutine* coroutine::this_coro = nullptr;
