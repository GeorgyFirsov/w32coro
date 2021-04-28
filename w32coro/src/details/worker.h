#pragma once
#include "stdafx.h"

namespace w32coro {
namespace details {

    struct ICoroWorker 
    {
        ICoroWorker(const ICoroWorker&) = delete;
        ICoroWorker& operator=(const ICoroWorker&) = delete;
        ICoroWorker(ICoroWorker&&) = delete;
        ICoroWorker& operator=(ICoroWorker&&) = delete;

        ICoroWorker() = default;
        virtual ~ICoroWorker() = default;

        virtual void Run() = 0;
    };

    template<typename Callable, typename... Args>
    class CImplCoroWorker
        : public ICoroWorker
    {
    public:
        CImplCoroWorker(Callable&& callable, Args... args)
            : m_callable(std::forward<Callable>(callable))
            , m_args(std::make_tuple(args...))
        { }

        void Run() override
        {
            RunImpl(std::index_sequence_for<Args...>{});
        }

    private:
        template<size_t... Idxs>
        void RunImpl(std::index_sequence<Idxs...>)
        {
            m_callable(std::get<Idxs>(m_args)...);
        }

    private:
        Callable            m_callable;
        std::tuple<Args...> m_args;
    };

} // namespace details
} // namespace w32coro