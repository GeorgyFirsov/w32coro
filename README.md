# w32coro

Small hand-written coroutines library. Written in C++14 with Win32 API (Fibers).

## Examples
#### Receiving some information from server

```cpp
#include "w32coro/w32coro.h"
#include "MyAwesomeProject.h"

int main()
{
    const auto ReceiveData = [](const std::wstring& wsServerName, DWORD dwPort) {
        IConnectionPtr spConn = Connect(wsServerName, dwPort);
        std::wstring wsData = spConn->Receive();
        
        w32coro::CoReturn(wsData);
    };
    
    std::wstring wsServerName = L"my.awesome.server";
    DWORD dwPort = 8080;
    
    const auto wsReceivedData = w32coro::CoAwait<std::wstring>(
        w32coro::Coroutine(ReceiveData, std::cref(wsServerName), dwPort));
        
    std::wcout << wsReceivedData << std::endl;
}

```

#### Suspending, resuming and returning various values

Actually this example can be extended to write real generator, that implements some methods such as `begin` and `end` to use it in range-based for loop.

```cpp
#include "w32coro/w32coro.h"
#include "MyAwesomeProject.h"

int main()
{
    const auto Generate = [](size_t Initial) {
        while (true) {
            w32coro::CoYield(Initial++);
        }
    };
    
    w32coro::Coroutine gen(Generate, 0);
    
    std::vector<size_t> Numbers;
    for (size_t i = 0; i < 10; i++) {
        Numbers.emplace_back(gen.Get<size_t>());
    }
    
    //
    // Will print 0 1 2 ...
    //
    for (size_t n : Numbers) {
        std::cout << n << ' ';
    }
}
```
