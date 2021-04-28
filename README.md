# w32coro

Small hand-written coroutines library. Written in C++14 with Win32 API (Fibers).

## Examples
#### Receiving some information from server

```cpp
#include <w32coro/w32coro.h>
#include "MyAwesomeProject.h"

int main()
{
    const auto ReceiveData = [](const std::wstring& wsServerName, DWORD dwPort) {
        IConnectionPtr spConn = Connect(wsServerName, dwPort);
        std::wstring wsData = spConn->Receive();
        
        w32coro::CoReturn(wsData);
    };
    
    const auto wsReceivedData = w32coro::CoAwait<std::wstring>(
        w32coro::Coroutine(ReceiveData, std::cref(wsServerName), dwPort));
        
    std::wcout << wsReceivedData << std::endl;
}

```
