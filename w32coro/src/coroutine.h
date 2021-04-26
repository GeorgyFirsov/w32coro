#pragma once
#include "stdafx.h"
#include "w32error.h"
#include "details/seh.h"
#include "details/worker.h"



namespace w32coro {

	class Coroutine
	{
		template<typename Type>
		friend void CoYield(Type&& value);

	public:
		template<typename Callable, typename... Args>
		explicit Coroutine(Callable&& callable, Args&&... args)
			: m_pWorker(std::make_unique<details::CImplCoroWorker<Callable, Args...>>(
				std::forward<Callable>(callable), std::forward<Args>(args)...))
		{
			if (hFiber.load(std::memory_order_acquire) == nullptr) {
				hFiber.store(ConvertThreadToFiber(nullptr), std::memory_order_release);
			}

			if (hFiber.load(std::memory_order_acquire) == nullptr) {
				throw W32Error{};
			}

			m_lpCurrentFiber = CreateFiber(
				0, reinterpret_cast<LPFIBER_START_ROUTINE>(&Coroutine::FiberProcedure), this);
			SwitchToFiber(m_lpCurrentFiber);
		}

		template<typename Type>
		Type Get()
		{

		}

	private:
		static VOID WINAPI FiberProcedure(Coroutine* pThis);

		template<typename Type>
		void SwitchToMainAndUpdateState(Type&& value)
		{

		}

	private:
		LPVOID									m_lpCurrentFiber;
		std::unique_ptr<details::ICoroWorker>	m_pWorker;

	private:
		static std::atomic<HANDLE> hFiber;
	};

	template<typename Type>
	void CoYield(Type&& value)
	{
		details::SehTranslator _;

		auto Context = reinterpret_cast<Coroutine>(GetFiberData());
		if (!Context) {
			throw W32Error{ ERROR_INVALID_HANDLE };
		}

		Context.SwitchToMainAndUpdateState(std::forward<Type>(value));
	}

} // namespace w32coro

