#include "stdafx.h"
#include "coroutine.h"


namespace w32coro {

    /* virtual */
    Coroutine::~Coroutine()
    {
        //
        // Mark coroutine as completed and free all resources
        // 

        MarkCompleted();
        DeleteFiber(m_lpCurrentFiber);
        ConvertFiberToThread();
    }


    void Coroutine::Resume()
    {
        //
        // Cannot resume in case of exception
        // 

        if (m_pException) {
            std::rethrow_exception(m_pException);
        }

        //
        // Resumable coroutine must not be completed
        // 

        if (WaitForSingleObject(m_hDoneEvent, 0) == WAIT_OBJECT_0) {
            return;
        }

        //
        // Mark running and resume
        // 

        // TODO: implement syncronization
        SwitchToFiber(m_lpCurrentFiber);
    }


    void Coroutine::Get() const
    {
        if (m_pException) {
            std::rethrow_exception(m_pException);
        }

        m_pState->GetPointer();
    }


    /* static */
    VOID WINAPI Coroutine::FiberProcedure(Coroutine* pThis)
    {
        try
        {
            details::SehTranslator _;
            pThis->m_pWorker->Run();
        }
        catch(const Coroutine::CoroReturnException&)
        { /* Do nothing here */ }
        catch(...)
        {
            pThis->m_pException = std::current_exception();
        }

        pThis->MarkCompleted();
        pThis->SwitchToMain();
    }


    void Coroutine::UpdateState()
    {
        m_pState = std::make_unique<details::CImplCoroState<void>>();
        // TODO:: implement syncronization
    }


    void Coroutine::SwitchToMain()
    {
        SwitchToFiber(m_lpMainFiber);
    }


    void Coroutine::MarkCompleted()
    {
        m_hDoneEvent.Set();
        // TODO: implement syncronization
    }


    void CoYield()
    {
        auto Context = details::SafeGetFiberData<Coroutine*>();
        Context->YieldImpl([Context]() { Context->UpdateState(); });
    }


    void CoReturn()
    {
        auto Context = details::SafeGetFiberData<Coroutine*>();
        Context->ReturnImpl([Context]() { Context->UpdateState(); });
    }

} // namespace w32coro