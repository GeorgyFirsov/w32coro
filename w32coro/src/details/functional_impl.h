#pragma once
#include "stdafx.h"
#include "w32error.h"
#include "details/seh.h"


namespace w32coro {
namespace details {
    
    template<typename Type>
    Type SafeGetFiberData()
    {
        static_assert(std::is_pointer<Type>::value, "Type must be a pointer");

        details::SehTranslator _;

        auto Data = reinterpret_cast<Type>(GetFiberData());
        if (!Data) {
            throw W32Error{ ERROR_INVALID_DATA };
        }
        return Data;
    }

} // namespace details
} // namespace w32coro