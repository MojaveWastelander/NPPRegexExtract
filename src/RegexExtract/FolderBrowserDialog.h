#pragma once
#include <Windows.h>
#include <string>
#include <Shlobj.h>

#pragma comment(lib, "Shell32.lib")

struct FolderBrowserDialog
{
public:
    FolderBrowserDialog(void);

    int Flags;
    HWND Owner;
    std::wstring SelectedPath;
    std::wstring Title;

    bool ShowDialog();
};