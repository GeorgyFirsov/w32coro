#pragma once
#include "stdafx.h"
#include "w32error.h"
#include "details/seh.h"
#include "details/lock.h"
#include "details/utils.h"
#include "details/state.h"
#include "details/worker.h"
#include "details/functional_impl.h"



namespace w32coro {

    /**
     * @brief Coroutine wrapper class. Receives any type of callable
     * object and necessary arguments for it
     * 
     */
    class Coroutine
        : private details::NonCopyable, private details::NonMovable
    {
        class CoroReturnException {};

        template<typename Type>
        friend void CoYield(const Type& value);
        friend void CoYield();

        template<typename Type>
        friend void CoReturn(const Type& value);
        friend void CoReturn();

    public:
        template<typename Callable, typename... Args>
        explicit Coroutine(Callable&& callable, Args... args)
            : m_hDoneEvent(TRUE, FALSE)
            , m_lpCurrentFiber(nullptr)
            , m_lpMainFiber(nullptr)
            , m_pException(nullptr)
            , m_pWorker(std::make_unique<details::CImplCoroWorker<Callable, Args...>>(std::forward<Callable>(callable), args...))
            , m_pState(std::make_unique<details::CImplCoroState<void>>())
        {
            m_lpMainFiber = ConvertThreadToFiber(nullptr);

            if (!m_lpMainFiber) {
                m_lpMainFiber = GetCurrentFiber();
            }

            VerifyPointer(m_lpMainFiber);

            //
            // Create fiber for received function
            // 

            m_lpCurrentFiber = CreateFiber(
                0, reinterpret_cast<LPFIBER_START_ROUTINE>(&Coroutine::FiberProcedure), this);

            VerifyPointer(m_lpCurrentFiber);

            //
            // Schedule created fiber
            // 

            Resume();
        }

        virtual ~Coroutine();

        template<typename Type>
        Type Get() const
        {
            //
            // Retrieving result is possible only in case of suspended fiber
            // 

            // TODO: implement syncronization

            if (m_pException) {
                std::rethrow_exception(m_pException);
            }

            return *reinterpret_cast<Type*>(m_pState->GetPointer());
        }

        void Get() const;

        void Resume();

    private:
        static VOID WINAPI FiberProcedure(Coroutine* pThis);

        template<typename UpdateProc>
        void YieldImpl(UpdateProc&& Proc)
        {
            std::forward<UpdateProc>(Proc)();
            SwitchToMain();
        }

        template<typename UpdateProc>
        void ReturnImpl(UpdateProc&& Proc)
        {
            std::forward<UpdateProc>(Proc)();
            throw CoroReturnException{};
        }

        template<typename Type>
        void UpdateState(const Type& value)
        {
            m_pState = std::make_unique<details::CImplCoroState<Type>>(value);
            // TODO: implement syncronization
        }

        void UpdateState();

        void SwitchToMain();

        void MarkCompleted();

    private:
        // Coroutine function exited
        ATL::CEvent                              m_hDoneEvent;

        // Pointer to current fiber (internal descriptor)
        LPVOID                                   m_lpCurrentFiber;

        // Pointer to caller fiber
        LPVOID                                   m_lpMainFiber;

        // Pointer to the exception (not nullptr if there was an exception in callee)
        std::exception_ptr                       m_pException;

        // Pointer to type-erasure wrapper over internal callable
        std::unique_ptr<details::ICoroWorker>    m_pWorker;

        // Pointer to type-erasure wrapper over internal state
        std::unique_ptr<details::ICoroState>     m_pState;
    };

    template<typename Type>
    void CoYield(const Type& value)
    {
        auto Context = details::SafeGetFiberData<Coroutine*>();
        Context->YieldImpl([Context, &value]() {
            Context->UpdateState(value); });
    }

    template<typename Type>
    void CoReturn(const Type& value)
    {
        auto Context = details::SafeGetFiberData<Coroutine*>();
        Context->ReturnImpl([Context, &value]() {
            Context->UpdateState(value); });
    }

    template<typename Type>
    Type CoAwait(const Coroutine& cor)
    {
        return cor.Get<Type>();
    }

} // namespace w32coro

