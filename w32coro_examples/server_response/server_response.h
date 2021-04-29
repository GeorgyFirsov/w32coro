#pragma once
#include "stdafx.h"


namespace server_response {

    class Server
    {
    public:
        Server(const std::wstring& dns, size_t port)
            : m_wsDnsName(dns)
            , m_Port(port)
        {
            Connect();
        }

        void Connect() const
        {
            // Simulate some work
            Sleep(100);
            std::wcout << L"Connected to " << m_wsDnsName << L" at port " << m_Port << std::endl;
        }

        std::wstring RequestHello(const std::wstring& wsRequest) const
        {
            // Simulate some work
            Sleep(1000);
            return L"Hello, " + wsRequest;
        }

    private:
        std::wstring m_wsDnsName;
        size_t       m_Port;
    };


    inline void RunExample()
    {
        Server Srv(L"my.awesome.server", 1337);

        const auto GetResponse = [&Srv](const std::wstring& wsRequest) {
            w32coro::CoReturn(Srv.RequestHello(wsRequest));
        };

        std::wstring wsAlice = L"Alice";

        std::wstring wsHelloAlice = w32coro::CoAwait<std::wstring>(
            w32coro::Coroutine(GetResponse, std::cref(wsAlice)));

        // Will print "Hello, Alice"
        std::wcout << wsHelloAlice << std::endl;

        std::wstring wsHelloBob = w32coro::CoAwait<std::wstring>(
            w32coro::Coroutine(GetResponse, L"Bob"));

        // Will print "Hello, Bob"
        std::wcout << wsHelloBob << std::endl;
    }

} // namespace server_response