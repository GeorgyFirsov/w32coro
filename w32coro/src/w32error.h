#pragma once
#include "stdafx.h"



namespace w32coro {
namespace details {

    std::string ConvertWideStringToString(const std::wstring& wsText);

    std::wstring GetErrorDescription(DWORD dwCode);

} // namespace details

    /**
     * @brief WIN32 error code wrapper. Used as an exception
     */
    class W32Error
        : public std::runtime_error
    {
    public:
        W32Error()
            : W32Error(GetLastError())
        { }

        explicit W32Error(DWORD dwErrorCode)
            : std::runtime_error(details::ConvertWideStringToString(
                details::GetErrorDescription(dwErrorCode)))
        { }
    };


    /**
     * @brief Checks if pointer is not null. If it is, throws W32Error
     */ 
    void VerifyPointer(void* p, DWORD dwFailCode = GetLastError());

} // namespace w32coro
