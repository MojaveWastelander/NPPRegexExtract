#include "Encoding.hpp"

std::string to_utf8(const wchar_t* buffer, int len)
{
        int nChars = ::WideCharToMultiByte(
                CP_UTF8,
                0,
                buffer,
                len,
                NULL,
                0,
                NULL,
                NULL);
        if (nChars == 0) return "";

        std::string newbuffer;
        newbuffer.resize(nChars) ;
        ::WideCharToMultiByte(
                CP_UTF8,
                0,
                buffer,
                len,
                const_cast< char* >(newbuffer.c_str()),
                nChars,
                NULL,
                NULL); 

        return newbuffer;
}

std::string to_utf8(const std::wstring& str)
{
        return to_utf8(str.c_str(), (int)str.size());
}