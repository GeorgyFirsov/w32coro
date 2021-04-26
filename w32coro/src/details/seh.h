#include "stdafx.h"



namespace w32coro {
namespace details {

	class SehTranslator final
	{
		SehTranslator(const SehTranslator&) = delete;
		SehTranslator& operator=(const SehTranslator&) = delete;
		SehTranslator(SehTranslator&&) = delete;
		SehTranslator& operator=(SehTranslator&&) = delete;

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