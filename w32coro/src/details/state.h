#pragma once
#include "stdafx.h"


namespace w32coro {
namespace details {

    struct ICoroState
    {
        virtual ~ICoroState() = default;

        virtual void* GetPointer() noexcept = 0;
        virtual const void* GetPointer() const noexcept = 0;
    };


    template<typename Type>
    class CImplCoroState
        : public ICoroState
    {
    public:
        CImplCoroState(Type&& value)
            : m_Value(std::forward<Type>(value))
        { }

        void* GetPointer() noexcept override { return &m_Value; }
        const void* GetPointer() const noexcept override { return &m_Value; }

    private:
        Type m_Value;
    };


    template<>
    class CImplCoroState<void>
        : public ICoroState
    {
    public:
        CImplCoroState() = default;

        void* GetPointer() noexcept override { return nullptr; }
        const void* GetPointer() const noexcept override { return nullptr; }
    };

} // namespace details
} // namespace w32coro