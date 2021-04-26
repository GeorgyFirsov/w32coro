#include "stdafx.h"
#include "coroutine.h"


namespace w32coro {

    /* static */ 
    std::atomic<HANDLE> Coroutine::hFiber = nullptr;

    /* static */
    VOID WINAPI Coroutine::FiberProcedure(Coroutine* pThis)
    {
        details::SehTranslator _;

        if (!pThis) {
            throw W32Error{ ERROR_INVALID_HANDLE };
        }

        pThis->m_pWorker->Run();
    }

} // namespace w32coro