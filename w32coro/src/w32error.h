#pragma once
#include "stdafx.h"



namespace w32coro {
namespace details {

	std::string ConvertWideStringToString(const std::wstring& wsText);

	std::wstring GetErrorDescription(DWORD dwCode);

} // namespace details

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

} // namespace w32coro
