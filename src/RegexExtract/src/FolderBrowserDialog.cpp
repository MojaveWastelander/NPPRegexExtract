#include "../inc/FolderBrowserDialog.h"

FolderBrowserDialog::FolderBrowserDialog(void)
{
    this->Flags = BIF_USENEWUI;
    this->Owner = 0;
}

bool FolderBrowserDialog::ShowDialog(void)
{
    BROWSEINFO bi;
    memset(&bi, 0, sizeof(bi));

    bi.hwndOwner = this->Owner;
    bi.lpszTitle = Title.c_str();
    bi.ulFlags = this->Flags;

    OleInitialize(NULL);

    LPITEMIDLIST pIDL = SHBrowseForFolder(&bi);

    if (pIDL == NULL)
    {
        return false;
    }

    TCHAR *buffer = new TCHAR[MAX_PATH];
    if (!SHGetPathFromIDList(pIDL, buffer) != 0)
    {
        CoTaskMemFree(pIDL);
        return false;
    }
    this->SelectedPath = buffer;

    CoTaskMemFree(pIDL);
    OleUninitialize();
    return true;
}