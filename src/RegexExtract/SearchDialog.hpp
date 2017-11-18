///////////////////////////////////////
// SearchDialog.h

#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H
#include <wxx_wincore.h>
#include <wxx_dialog.h>
#include <wxx_stdcontrols.h>
#include <wxx_controls.h>
#include <memory>
#include "RegexSearch.hpp"
#include "IDataKind.hpp"
#include "IDataExtractor.hpp"
#include "IOutputDataProcessor.hpp"
#include <npp\SciMessager.h>
#include <npp\NppMessager.h>

// Declaration of the SearchDialog class
class SearchDialog : public Win32xx::CDialog
{
public:
    SearchDialog(UINT nResID, Win32xx::CWnd* pParent = NULL);
    virtual ~SearchDialog();

protected:
    virtual BOOL OnInitDialog();
    virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual void OnOK();

private:
    Win32xx::CFont fnt{CreateFont(16, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, L"Consolas")};
    HCURSOR m_curBusySearch;
    HCURSOR m_curBusyExtract;
    HCURSOR m_curArrow;
    HICON m_icoFolder;
    CSciMessager m_SciMessager;
    CNppMessager m_NPPMessager;
    RegexSearch m_RegexSearch;
    bool m_bSearchDone;
    std::unique_ptr<IDataExtractor> m_upDataExtractor;
    std::unique_ptr<IOutputDataProcessor> m_upDataProcessor;

    Win32xx::CComboBox m_cbxFind;
    Win32xx::CButton m_btnSearch;
    Win32xx::CComboBox m_cbxReplace;
    Win32xx::CButton m_btnReplace;

    Win32xx::CButton m_rbtnExtractEachDiffFile;
    Win32xx::CButton m_rbtnExtractSingleFile;
    Win32xx::CButton m_rbtnWithSeparator;
    Win32xx::CEdit m_editSeparator;
    Win32xx::CButton m_rbtnPrettyPrint;
    Win32xx::CButton m_chkAddHeader;
    Win32xx::CButton m_rbtnExtractReplace;

    Win32xx::CButton m_rbtnExtractToNotepad;
    Win32xx::CButton m_rbtnSaveAsNewFiles;
    Win32xx::CComboBox m_cbxBasePath;
    Win32xx::CButton m_btnBrowseBasePath;
    Win32xx::CEdit m_editTemplateName;
    Win32xx::CButton m_chkOpenFilesInNotepad;

    Win32xx::CButton m_rbtnNoConversion;
    Win32xx::CButton m_rbtnAllUppercase;
    Win32xx::CButton m_rbtnAllLowerCase;
    Win32xx::CButton m_rbtnFirstUppercase;
    Win32xx::CButton m_chkSkipWholeMatch;

    Win32xx::CButton m_rbtnNoSort;
    Win32xx::CButton m_rbtnSortAscending;
    Win32xx::CButton m_rbtnSortDescending;

    Win32xx::CButton m_chkFilterUnique;
    Win32xx::CButton m_chkCaseInsensitive;
    Win32xx::CButton m_chkDotMatchNewLine;

    Win32xx::CButton m_rbtnCurrentFile;
    Win32xx::CButton m_rbtnAllOpenedFiles;
    Win32xx::CButton m_rbtnAllFilesFromLocation;
    Win32xx::CButton m_chkFromSelection;

    Win32xx::CComboBoxEx m_cbxMask;
    Win32xx::CComboBoxEx m_cbxPath;
    Win32xx::CButton m_btnFolder;

    void SaveSettings();

    void GetComboboxTitle( std::wstring &sText, std::vector<std::wstring>::iterator &itTitle );
    void GetComboboxTitle( Win32xx::CComboBox& cbx, std::vector<std::wstring> &vData );
    void OnSearch(bool bShowResults = true);
    void OnExtract();
	void UpdateTitle(CString& sText);

    std::unique_ptr<IOutputDataProcessor> GetDataProcessor();
    IDataExtractor* GetDataExtractor();
    std::unique_ptr<IDataKind> NextData();
    std::vector<std::unique_ptr<IDataKind>> m_vData;
};

#endif //SEARCHDIALOG_H
