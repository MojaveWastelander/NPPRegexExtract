#pragma once
//#include <wincore.h>
//#include <stdcontrols.h>
#include <vector>
#include <string>
#include <wxx_controls.h>
//#include "Base.h"

inline void SetCheckBox(Win32xx::CButton& checkBox)
{
    checkBox.SetCheck(BST_CHECKED);
}

inline bool IsChecked(Win32xx::CButton& checkBox)
{
    return checkBox.GetCheck() == BST_CHECKED;
}

const size_t COMBOBOX_MAX_ITEMS = 16;

void InsertFrontUniqueNonEmpty(std::vector<std::wstring>& vLines, std::wstring&& val)
{
    if (val.empty()) return;
    auto it = std::find(begin(vLines), end(vLines), val);

    if (it != begin(vLines) || vLines.empty())
    {
        if (it != end(vLines))
        {
            vLines.erase(it);
        }
        if (vLines.size() >= COMBOBOX_MAX_ITEMS) vLines.erase(vLines.end());
        vLines.insert(begin(vLines), val);
    }

}