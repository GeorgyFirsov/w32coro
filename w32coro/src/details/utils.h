#pragma once


namespace w32coro {
namespace details {

    class NonCopyable
    {
    protected:
        NonCopyable() = default;
        virtual ~NonCopyable() = default;
        
    private:
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
    };


    class NonMovable
    {
    protected:
        NonMovable() = default;
        virtual ~NonMovable() = default;

    private:
        NonMovable(NonMovable&&) = delete;
        NonMovable& operator=(NonMovable&&) = delete;
    };

} // namespace details
} // namespace w32coro