#pragma once
#include "Base.h"

std::string to_utf8(const wchar_t* buffer, int len);

std::string to_utf8(const std::wstring& str);