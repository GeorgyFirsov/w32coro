#include "pch.h"


TEST(Creation, EmptyNoError)
{
    EXPECT_NO_THROW(
        w32coro::Coroutine cor{ []() {} };
    );
}

TEST(Creation, MultipleNoError)
{
    w32coro::Coroutine cor1{ []() {} };

    EXPECT_NO_THROW(
        w32coro::Coroutine cor2{ []() {} };
    );
}

TEST(GetResult, NoError)
{
    w32coro::Coroutine cor{
        []() { w32coro::CoReturn(10); }
    };

    EXPECT_NO_THROW(
        cor.Get<int>();
    );
}

TEST(GetResult, VoidNoError)
{
    w32coro::Coroutine cor{
        []() { w32coro::CoReturn(); }
    };

    EXPECT_NO_THROW(
        cor.Get();
    );
}

TEST(GetResult, ExactResult)
{
    w32coro::Coroutine cor{
        []() { w32coro::CoReturn(10); }
    };

    EXPECT_EQ(
        cor.Get<int>(), 
        10
    );
}

TEST(ErrorInCallee, Get)
{
    w32coro::Coroutine cor{
        []() { throw std::runtime_error{"Test exception"}; }
    };

    EXPECT_THROW(
        cor.Get(),
        std::runtime_error
    );
}

TEST(ErrorInCallee, GetResult)
{
    w32coro::Coroutine cor{
        [](bool b) { 
            if (b)
                throw std::runtime_error{"Test exception"};
            else 
                w32coro::CoReturn(std::string("No success here"));
        },
        true
    };

    EXPECT_THROW(
        cor.Get<std::string>(),
        std::runtime_error
    );
}

TEST(ErrorInCallee, NoResume)
{
    w32coro::Coroutine cor{
        []() {
            w32coro::CoYield(10);
            throw std::runtime_error{"Test exception"};
            w32coro::CoYield(20);
        }
    };

    EXPECT_EQ(
        cor.Get<int>(),
        10
    );

    EXPECT_NO_THROW(
        cor.Resume()
    );

    EXPECT_THROW(
        cor.Resume(),
        std::runtime_error
    );
}

TEST(YieldResult, NoError)
{
    w32coro::Coroutine cor{
        []() {
            w32coro::CoYield(10);
            w32coro::CoYield(20);
        }
    };

    EXPECT_NO_THROW(
        cor.Get<int>();
    );

    EXPECT_NO_THROW(
        cor.Resume();
    );

    EXPECT_NO_THROW(
        cor.Get<int>();
    );
}

TEST(YieldResult, ExactMatch)
{
    w32coro::Coroutine cor{
        []() {
            w32coro::CoYield(10);
            w32coro::CoYield(20);
        }
    };

    EXPECT_EQ(
        cor.Get<int>(),
        10
    );

    cor.Resume();

    EXPECT_EQ(
        cor.Get<int>(),
        20
    );
}

TEST(AwaitResult, NoError)
{
    const auto GetResult = [](const std::string& server, size_t port) {
        std::cout << "Receiving result from " << server << ':' << port << std::endl;
        w32coro::CoReturn(std::string("Hello world!"));
    };

    std::string server = "my.awesome.host";
    size_t port = 8080;

    //
    // Exp
    const auto Result = w32coro::CoAwait<std::string>(
        w32coro::Coroutine(GetResult, std::cref(server), port));
    
    EXPECT_STREQ(
        Result.c_str(),
        "Hello world!"
    );
}