#include "stdafx.h"
#include "w32error.h"



namespace w32coro {
namespace details {

    std::string ConvertWideStringToString(const std::wstring& wsText)
    {
        int cchString = WideCharToMultiByte(
            CP_UTF8, 0, wsText.c_str(), wsText.length(), nullptr, 0, nullptr, nullptr);

        std::string sResult(cchString, '\0');
        WideCharToMultiByte(
            CP_UTF8, 0, wsText.c_str(), wsText.length(), &sResult[0], cchString, nullptr, nullptr);

        return sResult;
    }


    std::wstring GetErrorDescription(DWORD dwCode)
    {
        LPWSTR pszBuffer;

        DWORD dwWritten = FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, dwCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&pszBuffer), 0, nullptr);

        std::unique_ptr<WCHAR, std::function<VOID(LPVOID)>> spBuffer(
            pszBuffer, [](LPVOID pv) { if (pv) LocalFree(pv); });

        if (dwWritten) {
            return std::wstring(spBuffer.get(), dwWritten);
        } else {
            return std::to_wstring(dwCode);
        }
    }

} // namespace details

    
    void VerifyPointer(void* p, DWORD dwFailCode /* = GetLastError() */)
    {
        if (!p) {
            throw W32Error{ dwFailCode };
        }
    }

} // namespace w32coro