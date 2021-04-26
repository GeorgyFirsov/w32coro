#include "stdafx.h"
#include "seh.h"
#include "w32error.h"



namespace w32coro {
namespace details {

	SehTranslator::SehTranslator()
		: m_pfnPreviousTranslator(_set_se_translator(&SehTranslator::TranslateToW32Error))
	{ }

	SehTranslator::~SehTranslator()
	{
		_set_se_translator(m_pfnPreviousTranslator);
	}

	/* static */ 
	void __cdecl SehTranslator::TranslateToW32Error [[noreturn]] (unsigned int uiCode, _EXCEPTION_POINTERS* /* pPointers */)
	{
		throw W32Error{ uiCode };
	}

} // namespace details
} // namespace w32coro