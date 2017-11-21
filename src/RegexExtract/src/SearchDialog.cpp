///////////////////////////////////////
// SearchDialog.cpp

#include <sdkddkver.h>			// Set the supported window features

#include <wxx_wincore.h>
#include <wxx_dialog.h>
#include "../inc/SearchDialog.hpp"
#include "../inc/Base.h"
#include "../inc/Helpers.h"
#include "../inc/FolderBrowserDialog.h"
#include "../resources/resource.h"

#include "../inc/DataExtractorSingleFile.hpp"
#include "../inc/DataExtractorMultipleFiles.hpp"
#include "../inc/DataExtractorNotepadSingle.hpp"
#include "../inc/DataExtractorNotepadMultiple.hpp"
#include "../inc/DataPath.hpp"
#include "../inc/DataString.hpp"
#include "../inc/OutputDataProcessorSingleFile.hpp"
#include "../inc/OutputDataProcessorMultipleFiles.hpp"
#include <npp\NppPluginIface.h>
#include <boost\algorithm\string\replace.hpp>

// Definitions for the SearchDialog class
SearchDialog::SearchDialog(UINT nResID, Win32xx::CWnd* pParent)
    : Win32xx::CDialog(nResID)
{}

SearchDialog::~SearchDialog()
{}

INT_PTR SearchDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
    {
        SaveSettings();
        if (Options::save_options() == false)
        {
            ::MessageBox(nullptr, L"Settings file not saved", L"RegexExtract", MB_ICONEXCLAMATION);
        }

        ::PostQuitMessage(0);
    }
    break;
    }

    // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(uMsg, wParam, lParam);
}

BOOL SearchDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (LOWORD(wParam))
    {
    case IDC_BTN_FOLDER:
    case IDC_BTN_BROWSE_BASE_PATH:
    {
        auto folderBrowserDialog1 = std::make_unique<FolderBrowserDialog>();
        folderBrowserDialog1->Flags |= BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_VALIDATE | BIF_NEWDIALOGSTYLE | BIF_UAHINT;
        folderBrowserDialog1->Title = L"Choose a folder or try to create new one";
        folderBrowserDialog1->Owner = m_hWnd;

        if (folderBrowserDialog1->ShowDialog())
        {
            switch (LOWORD(wParam))
            {
            case IDC_BTN_FOLDER: this->m_cbxPath.SetWindowText(folderBrowserDialog1->SelectedPath.c_str()); break;
            case IDC_BTN_BROWSE_BASE_PATH: this->m_cbxBasePath.SetWindowText(folderBrowserDialog1->SelectedPath.c_str()); break;
            }
        }

    } break;
    case IDC_BTN_SEARCH:
    {
        //LOG(INFO) << "Command: IDC_BTN_SEARCH";
        OnSearch();
    } break;
    case IDC_BTN_EXTRACT:
    {
        //LOG(INFO) << "Command: IDC_BTN_EXTRACT";
        OnExtract();
    } break;
    case IDC_RBTN_CURRENT_FILE:
    case IDC_RBTN_ALL_OPENED_FILES:
    case IDC_RBTN_ALL_FROM_LOCATION:
    {
        m_upDataProcessor = GetDataProcessor();
    } break;
    } //switch (LOWORD(wParam))

    return FALSE;
}

BOOL SearchDialog::OnInitDialog()
{
    m_curArrow = ::LoadCursor(nullptr, MAKEINTRESOURCE(IDC_ARROW));
    m_curBusySearch = ::LoadCursor((HINSTANCE)npp_plugin::hModule(), MAKEINTRESOURCE(IDI_CUR_BUSY_SEARCH));
    m_curBusyExtract = ::LoadCursor((HINSTANCE)npp_plugin::hModule(), MAKEINTRESOURCE(IDI_CUR_BUSY_EXTRACT));
    m_icoFolder = static_cast<HICON>(::LoadImage((HINSTANCE)npp_plugin::hModule(), MAKEINTRESOURCE(IDI_FOLDER), IMAGE_ICON, 14, 14, 0));

    AttachItem(IDC_RBTN_NO_CONVERSION, m_rbtnNoConversion);
    AttachItem(IDC_RBTN_ALL_UPPERCASE, m_rbtnAllUppercase);
    AttachItem(IDC_RBTN_ALL_LOWERCASE, m_rbtnAllLowerCase);
    AttachItem(IDC_RBTN_FIRST_UPPERCASE, m_rbtnFirstUppercase);
    AttachItem(IDC_BTN_BROWSE_BASE_PATH, m_btnBrowseBasePath);
    AttachItem(IDC_RBTN_EXTRACT_EACH_MATCH, m_rbtnExtractEachDiffFile);
    AttachItem(IDC_RBTN_EXTRACT_SINGLE_FILE, m_rbtnExtractSingleFile);
    AttachItem(IDC_RBTN_WITH_SEPARATOR, m_rbtnWithSeparator);
    AttachItem(IDC_RBTN_PRETTY_PRINT, m_rbtnPrettyPrint);
    AttachItem(IDC_EDIT_SEPARATOR, m_editSeparator);
    AttachItem(IDC_CHK_ADD_HEADER, m_chkAddHeader);
    AttachItem(IDC_RBTN_EXTRACT_WITH_REPLACE, m_rbtnExtractReplace);
    AttachItem(IDC_RBTN_EXTRACT_TO_NPP, m_rbtnExtractToNotepad);
    AttachItem(IDC_RBTN_SAVE_AS_NEW_FILES, m_rbtnSaveAsNewFiles);
    AttachItem(IDC_CHK_OPEN_FILES_IN_NPP, m_chkOpenFilesInNotepad);
    AttachItem(IDC_CHK_SKIP_WHOLE_MATCH, m_chkSkipWholeMatch);
    AttachItem(IDC_RBTN_NO_SORT, m_rbtnNoSort);
    AttachItem(IDC_RBTN_SORT_ASCENDING, m_rbtnSortAscending);
    AttachItem(IDC_RBTN_SORT_DESCENDING, m_rbtnSortDescending);
    AttachItem(IDC_CHK_FILTER_UNIQUE, m_chkFilterUnique);
    AttachItem(IDC_CHK_CASE_INSENSITIVE, m_chkCaseInsensitive);
    AttachItem(IDC_CHK_DOT_MATCH_NEW_LINES, m_chkDotMatchNewLine);
    AttachItem(IDC_BTN_SEARCH, m_btnSearch);
    AttachItem(IDC_BTN_EXTRACT, m_btnReplace);
    AttachItem(IDC_EDIT_TEMPALTE_NAME, m_editTemplateName);
    AttachItem(IDC_CBX_FIND, m_cbxFind);
    AttachItem(IDC_CBX_REPLACE, m_cbxReplace);
    AttachItem(IDC_CBX_BASE_PATH, m_cbxBasePath);
    AttachItem(IDC_RBTN_CURRENT_FILE, m_rbtnCurrentFile);
    AttachItem(IDC_RBTN_ALL_OPENED_FILES, m_rbtnAllOpenedFiles);
    AttachItem(IDC_RBTN_ALL_FROM_LOCATION, m_rbtnAllFilesFromLocation);
    AttachItem(IDC_CHK_FROM_SELECTION, m_chkFromSelection);
    AttachItem(IDC_CBX_PATH, m_cbxPath);
    AttachItem(IDC_CBX_MASK, m_cbxMask);
    AttachItem(IDC_BTN_FOLDER, m_btnFolder);

    // Get settings
    switch (Options::extract_case_conversion())
    {
    case Options::en_ExtractCaseConversion::NoConversion:   SetCheckBox(m_rbtnNoConversion); break;
    case Options::en_ExtractCaseConversion::AllUppercase:   SetCheckBox(m_rbtnAllUppercase); break;
    case Options::en_ExtractCaseConversion::AllLowercase:   SetCheckBox(m_rbtnAllLowerCase); break;
    case Options::en_ExtractCaseConversion::FirstUppercase: SetCheckBox(m_rbtnFirstUppercase); break;
    default:
    break;
    }

    switch (Options::extract_mode())
    {
    case Options::en_ExtractMode::ExtractInDifferentFile: SetCheckBox(m_rbtnExtractEachDiffFile); break;
    case Options::en_ExtractMode::ExtractInSingleFile: SetCheckBox(m_rbtnExtractSingleFile); break;
    case Options::en_ExtractMode::ExtractWithReplace: SetCheckBox(m_rbtnExtractReplace); break;
    default:
    break;
    }

    switch (Options::extract_mode_single_file())
    {
    case Options::en_ExtractModeSingleFile::WithSeparator: SetCheckBox(m_rbtnWithSeparator); break;
    case Options::en_ExtractModeSingleFile::PrettyPrint: SetCheckBox(m_rbtnPrettyPrint); break;
    default:
    break;
    }

    switch (Options::sort_mode())
    {
    case Options::en_SortMode::NoSort: SetCheckBox(m_rbtnNoSort); break;
    case Options::en_SortMode::SortAscending: SetCheckBox(m_rbtnSortAscending); break;
    case Options::en_SortMode::SortDescending: SetCheckBox(m_rbtnSortDescending); break;
    default:
    break;
    }

    switch (Options::save_mode())
    {
    case Options::en_SaveMode::ExtractToNotepad: SetCheckBox(m_rbtnExtractToNotepad); break;
    case Options::en_SaveMode::SaveAsNewFile: SetCheckBox(m_rbtnSaveAsNewFiles); break;
    default:
    break;
    }

    m_chkAddHeader.SetCheck(Options::add_header());
    m_chkCaseInsensitive.SetCheck(Options::case_insensitive());
    m_chkFilterUnique.SetCheck(Options::filter_unique());
    m_chkDotMatchNewLine.SetCheck(Options::dot_match_newline());
    m_chkOpenFilesInNotepad.SetCheck(Options::open_files_in_notepad());
    m_chkSkipWholeMatch.SetCheck(Options::skip_whole_match());
    
    m_cbxFind.Clear();
    for (auto& s : Options::find_history())
    {
        m_cbxFind.AddString(s.c_str());
    }
    m_cbxFind.SetCurSel(0);

    m_cbxReplace.Clear();
    for (auto& s : Options::replace_history())
    {
        m_cbxReplace.AddString(s.c_str());
    }
    m_cbxReplace.SetCurSel(0);

    m_cbxBasePath.Clear();
    for (auto& s : Options::base_path())
    {
        m_cbxBasePath.AddString(s.c_str());
    }
    m_cbxBasePath.SetCurSel(0);

    m_editSeparator.SetWindowText(Options::separator().c_str());
    m_editTemplateName.SetWindowText(Options::template_name().c_str());

    switch (Options::data_location())
    {
    case Options::en_DataLocation::CurrentFile: SetCheckBox(m_rbtnCurrentFile); break;
    case Options::en_DataLocation::SpecificFiles: SetCheckBox(m_rbtnAllFilesFromLocation); break;
    case Options::en_DataLocation::AllOpenedFiles: SetCheckBox(m_rbtnAllOpenedFiles); break;
    default: SetCheckBox(m_rbtnCurrentFile);
    }

    m_chkFromSelection.SetCheck((Options::in_selection() == true) ? BST_CHECKED : BST_UNCHECKED);

    m_cbxMask.Clear(); 
    for (auto& s : Options::files_masks())
    {
        m_cbxMask.AddString(s.c_str());
    }
    m_cbxMask.SetCurSel(0);

    m_cbxPath.Clear();
    for (auto& s : Options::files_paths())
    {
        m_cbxPath.AddString(s.c_str());
    }
    m_cbxPath.SetCurSel(0);
    m_cbxFind.SetFont(fnt);
    m_cbxReplace.SetFont(fnt);


    m_upDataProcessor = GetDataProcessor();
    m_upDataExtractor.reset(GetDataExtractor());
    m_SciMessager.setSciWnd(npp_plugin::hMainView());
    NppData data = {npp_plugin::hNpp(), npp_plugin::hMainView(), npp_plugin::hSecondView()}; 
    m_NPPMessager.setNppData(data);
    // Set the Icon
    SetIconLarge(IDI_DLL);
    SetIconSmall(IDI_DLL);

    m_btnFolder.SetIcon(m_icoFolder);
    m_btnBrowseBasePath.SetIcon(m_icoFolder);
    //logging::log() << L"Init OK!";
    return true;
}

void SearchDialog::OnOK()
{
    CDialog::OnOK();
}

void SearchDialog::SaveSettings()
{
    if (IsChecked(m_rbtnNoConversion)) Options::extract_case_conversion() = Options::en_ExtractCaseConversion::NoConversion;
    else if (IsChecked(m_rbtnAllUppercase)) Options::extract_case_conversion() = Options::en_ExtractCaseConversion::AllUppercase;
    else if (IsChecked(m_rbtnAllLowerCase)) Options::extract_case_conversion() = Options::en_ExtractCaseConversion::AllLowercase;
    else if (IsChecked(m_rbtnFirstUppercase)) Options::extract_case_conversion() = Options::en_ExtractCaseConversion::FirstUppercase;

    if (IsChecked(m_rbtnExtractEachDiffFile)) Options::extract_mode() = Options::en_ExtractMode::ExtractInDifferentFile;
    else if (IsChecked(m_rbtnExtractSingleFile)) Options::extract_mode() = Options::en_ExtractMode::ExtractInSingleFile;
    else if (IsChecked(m_rbtnExtractReplace)) Options::extract_mode() = Options::en_ExtractMode::ExtractWithReplace;

    if (IsChecked(m_rbtnWithSeparator)) Options::extract_mode_single_file() = Options::en_ExtractModeSingleFile::WithSeparator;
    else if (IsChecked(m_rbtnPrettyPrint)) Options::extract_mode_single_file() = Options::en_ExtractModeSingleFile::PrettyPrint;

    if (IsChecked(m_rbtnNoSort)) Options::sort_mode() = Options::en_SortMode::NoSort;
    else if (IsChecked(m_rbtnSortAscending)) Options::sort_mode() = Options::en_SortMode::SortAscending;
    else if (IsChecked(m_rbtnSortDescending)) Options::sort_mode() = Options::en_SortMode::SortDescending;

    if (IsChecked(m_rbtnExtractToNotepad)) Options::save_mode() = Options::en_SaveMode::ExtractToNotepad;
    else if (IsChecked(m_rbtnSaveAsNewFiles)) Options::save_mode() = Options::en_SaveMode::SaveAsNewFile;

    Options::add_header() = IsChecked(m_chkAddHeader);
    Options::case_insensitive() = IsChecked(m_chkCaseInsensitive);
    Options::filter_unique() = IsChecked(m_chkFilterUnique);
    Options::dot_match_newline() = IsChecked(m_chkDotMatchNewLine);
    Options::open_files_in_notepad() = IsChecked(m_chkOpenFilesInNotepad);
    Options::skip_whole_match() = IsChecked(m_chkSkipWholeMatch);

    Options::separator().assign(m_editSeparator.GetWindowText());

    Options::template_name().assign(m_editTemplateName.GetWindowText());

    
    InsertFrontUniqueNonEmpty(Options::find_history(), m_cbxFind);
    InsertFrontUniqueNonEmpty(Options::replace_history(), m_cbxReplace);
    InsertFrontUniqueNonEmpty(Options::base_path(), m_cbxBasePath);
    InsertFrontUniqueNonEmpty(Options::files_paths(), m_cbxPath);
    InsertFrontUniqueNonEmpty(Options::files_masks(), m_cbxMask);


    if (IsChecked(m_rbtnCurrentFile)) Options::data_location() = Options::en_DataLocation::CurrentFile;
    else if (IsChecked(m_rbtnAllFilesFromLocation)) Options::data_location() = Options::en_DataLocation::SpecificFiles;
    else if (IsChecked(m_rbtnAllOpenedFiles)) Options::data_location() = Options::en_DataLocation::AllOpenedFiles;
    else Options::data_location() = Options::en_DataLocation::CurrentFile;

    Options::in_selection() = (IsChecked(m_chkFromSelection)) ? true : false;

    //TODO: Check if search was done already
    Options::get().save_options();
    m_upDataProcessor = GetDataProcessor();
    m_upDataExtractor.reset(GetDataExtractor());
}

void SearchDialog::OnSearch(bool bShowResults)
{
    SaveSettings();
    try
    {
        std::wstring wsExpression = m_cbxFind.GetWindowText();
        std::wstring wsReplaceExpression = m_cbxReplace.GetWindowText();
       // LOG(INFO) << "Regex expression: " << wsExpression;
       // LOG(INFO) << "Replace expression: " << wsExpression;
        std::unique_ptr<IDataKind> upData(nullptr);
        m_RegexSearch.ChangeExpression(UnescapeString(wsExpression), UnescapeString(wsReplaceExpression));
        
        auto start = std::chrono::high_resolution_clock::now();
        ::SetCursor(m_curBusySearch);
        while ((upData = NextData()))
        {
            m_RegexSearch.ParseData(upData.get(), m_upDataProcessor.get());
        }

        auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
        auto minutes = std::chrono::duration_cast<std::chrono::minutes>(std::chrono::high_resolution_clock::now() - start);
        if (bShowResults)
        {
            Win32xx::CString strStat;
            if (minutes.count())
            {
                strStat.Format(L"Found %d matches in %dm %.2fs.", m_RegexSearch.GetMatchesCount(), minutes.count(), (msec - minutes).count() / 1000.f);
            }
            else
            {
                strStat.Format(L"Found %d matches in %.2fs.", m_RegexSearch.GetMatchesCount(), msec.count() / 1000.f);
            }
            UpdateTitle(strStat);
        }
    }
    catch (std::exception& e)
    {
        ::MessageBox(this->m_hWnd, utf8util::UTF16FromUTF8(e.what()).c_str(), L"RegexExtract", MB_ICONERROR);
    }
    catch (...)
    {
       // LOG(INFO) << "Fatal exception!";
    }
    ::SetCursor(m_curArrow);

}

std::unique_ptr<IDataKind> SearchDialog::NextData()
{
   // LOG(INFO) << "data location: " << static_cast<int>(Options::GetDataLocation());
    switch (Options::data_location())
    {
    case Options::en_DataLocation::CurrentFile:
    {
        static bool bDone = false;
        if (bDone == false)
        {
            bDone = true;
            if (!Options::in_selection())
            {
                // Get all text from current document
               // LOG(INFO) << "Get all text from current document";
                std::vector<char> vTemp(m_SciMessager.getTextLength() + 1);
               // LOG(INFO) << "Text size: " << vTemp.size();
                m_SciMessager.getText(static_cast<int>(vTemp.size() - 1), &vTemp[0]);
                return std::make_unique<DataKindString>(utf8util::UTF16FromUTF8(&vTemp[0]));
            }
            else
            {
                // Get selected text from current document
               // LOG(INFO) << "Get selected text from current document";
                std::vector<char> vTemp(m_SciMessager.getSelText(nullptr) + 1);
               // LOG(INFO) << "Text size: " << vTemp.size();
                m_SciMessager.getSelText(&vTemp[0]);
                return std::make_unique<DataKindString>(utf8util::UTF16FromUTF8(&vTemp[0]));
            }
        }
        else
        {
            bDone = false;
        }
    } break;
    case Options::en_DataLocation::AllOpenedFiles:
    {
        static std::vector<std::wstring> vOpenedFiles;

        switch (vOpenedFiles.size())
        {
        case 1: // Vector cleared of useful data, now end
        {
            vOpenedFiles.clear();
        } break;
        case 0:
        {
            size_t uiOpenFiles = m_NPPMessager.SendNppMsg(NPPM_GETNBOPENFILES, 0, ALL_OPEN_FILES);
            vOpenedFiles.reserve(uiOpenFiles + 1);
            wchar_t** vTemp = new wchar_t*[uiOpenFiles];
            for (size_t i = 0; i < uiOpenFiles; ++i)
            {
                vTemp[i] = new wchar_t[MAX_PATH];
            }
            uiOpenFiles = m_NPPMessager.SendNppMsg(NPPM_GETOPENFILENAMES, (WPARAM)(vTemp), uiOpenFiles);
            vOpenedFiles.push_back(std::wstring(L"")); // Add an empty marker at beginning
            for (size_t i = 0; i < uiOpenFiles; ++i)
            {
                vOpenedFiles.push_back(std::wstring(vTemp[i]));
                delete[] vTemp[i];
            }
            delete[] vTemp;
        } // Fall trough, so will be returned first found file.
        default:
        {
            m_NPPMessager.SendNppMsg(NPPM_SWITCHTOFILE, 0, (LPARAM)vOpenedFiles.back().c_str());
            // Get all text from current document
            std::vector<char> vTemp(m_SciMessager.getTextLength() + 1);
            m_SciMessager.getText(static_cast<int>(vTemp.size() - 1), &vTemp[0]);
            vOpenedFiles.pop_back();
            return std::unique_ptr<IDataKind>(new DataKindString(utf8util::UTF16FromUTF8(&vTemp[0])));
        }
        }
    } break;
    case Options::en_DataLocation::SpecificFiles:
    {
        static std::experimental::filesystem::recursive_directory_iterator itPos;
        static std::experimental::filesystem::recursive_directory_iterator itEnd;
        static std::wregex rxMask;

        if (itPos == itEnd)
        {
            std::experimental::filesystem::path pathToSearch(std::wstring(m_cbxPath.GetWindowText()));
            // If path does not exist or is not a directory search will not be executed
            if (!std::experimental::filesystem::exists(pathToSearch))
            {
                ::MessageBox(this->m_hWnd, L"Path does not exist", L"Data location path field", MB_ICONEXCLAMATION);
                break;
            }
            else if (!std::experimental::filesystem::is_directory(pathToSearch))
            {
                ::MessageBox(this->m_hWnd, L"Field provided is not a directory", L"Data location path field", MB_ICONEXCLAMATION);
                break;
            }

            try
            {
                std::wstring temp{std::wstring(m_cbxMask.GetWindowText())};
                if (!temp.empty())
                {
                    boost::replace_all(temp, L".", L"\\.");
                    boost::replace_all(temp, L"*", L".*");
                    boost::replace_all(temp, L"?", L".");
                }
                rxMask.assign(temp);
                itPos = std::experimental::filesystem::recursive_directory_iterator(pathToSearch);
            }
            catch (std::exception& e)
            {
                ::MessageBox(this->m_hWnd, utf8util::UTF16FromUTF8(e.what()).c_str(), L"Data location mask regex error", MB_ICONERROR);
                break;
            }
        }
        
        std::unique_ptr<IDataKind> up{nullptr};
        while (itPos != itEnd)
        {
            if (std::experimental::filesystem::is_regular_file(itPos->path()))
            {
                auto fn = itPos->path().filename().wstring();
                if (std::regex_match(itPos->path().filename().wstring(), rxMask))
                {
                    up = std::unique_ptr<IDataKind>(new DataKindPath(itPos->path().wstring()));
                }
            }
            try
            {
                itPos++;
            }
            catch (std::exception& e)
            {
                itPos++;
            }
            if (up) return up;
        }
    } break;
    default: break;
    }
    return std::unique_ptr<IDataKind>(nullptr);
}

std::unique_ptr<IOutputDataProcessor> SearchDialog::GetDataProcessor()
{
    if (IsChecked(m_rbtnAllFilesFromLocation) || IsChecked(m_rbtnAllOpenedFiles))
    {
        return std::make_unique<OutputDataProcessorMultipleFiles>();
    }
    else
    {
        return std::make_unique<OutputDataProcessorSingleFile>();
    }
}

IDataExtractor* SearchDialog::GetDataExtractor()
{
    if (IsChecked(m_rbtnExtractToNotepad))
    {
        if (IsChecked(m_rbtnAllFilesFromLocation) || IsChecked(m_rbtnAllOpenedFiles))
        {
            return new DataExtractorNotepadMultiple(m_SciMessager, m_NPPMessager);
        }
        else
        {
            return new DataExtractorNotepadSingle(m_SciMessager, m_NPPMessager);
        }
    }
    else
    {
        if (IsChecked(m_rbtnAllFilesFromLocation) || IsChecked(m_rbtnAllOpenedFiles))
        {
            return new DataExtractorMultipleFiles(m_NPPMessager);
        }
        else
        {
            return new DataExtractorSingleFile;
        }
    }
}

void SearchDialog::OnExtract()
{
    OnSearch(false);
    ::SetCursor(m_curBusyExtract);
    try
    {
        if (m_upDataProcessor->IsEmpty() == false)
        {
            m_upDataExtractor->ExtractData(m_upDataProcessor.get());
        }
    }
    catch (std::exception& e)
    {
        ::MessageBox(nullptr, utf8util::UTF16FromUTF8(e.what()).c_str(), L"RegexExtract", MB_ICONEXCLAMATION);
        ::SetCursor(m_curArrow);
        return;
    }
    ::SetCursor(m_curArrow);
    this->EndDialog(0);
}

void SearchDialog::UpdateTitle(CString& sText)
{
    CString title = L"RegexExtract";
    if (!sText.IsEmpty())
    {
        title += L" - ";
        title += sText;
    }
    this->SetWindowText(title);
}

void SearchDialog::GetComboboxTitle(Win32xx::CComboBox& cbx, std::vector<std::wstring> &vData)
{
    auto sText = std::wstring(cbx.GetWindowText());
    std::vector<std::wstring>::iterator itTitle;
    if (sText.empty()) return;
    if ((itTitle = std::find(vData.begin(), vData.end(), sText)) == vData.end())
    {
        vData.push_back(std::move(sText));
        std::swap(vData.end() - 1, vData.begin());
    }
    else if (itTitle != vData.begin())
    {
        std::swap(itTitle, vData.begin());
    }
}
