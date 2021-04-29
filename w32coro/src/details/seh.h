#pragma once
#include "stdafx.h"
#include "details/utils.h"



namespace w32coro {
namespace details {

    class SehTranslator final
        : private NonCopyable, private NonMovable
    {
    public:
        SehTranslator();
        ~SehTranslator();

    private:
        static void __cdecl TranslateToW32Error [[noreturn]] (unsigned int uiCode, _EXCEPTION_POINTERS* pPointers);

    private:
        _se_translator_function m_pfnPreviousTranslator;
    };

} // namespace details
} // namespace w32coro