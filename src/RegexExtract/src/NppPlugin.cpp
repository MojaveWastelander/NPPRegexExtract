/* NppPlugin.cpp
 *
 * This file is part of the Notepad++ Plugin Interface Lib.
 * Copyright 2009 Thell Fowler (thell@almostautomated.com)
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program;
 * if not, write to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 *  This is the main Notepad++ Plugin file for use with Notepad++ Plugin Interface Lib.
 *
 */
#include "../inc/NppPlugin.h"
#include "../inc/SearchDialog.hpp"
#include "../resources/resource.h"
#include <WinUser.h>
#include <thread>
#include <wxx_wincore.h>
#include <functional>
#include <atomic>

/*
 *  The v_getfuncarray namespace alias allows for emulation of a class's 'virtual' function by
 *  providing a 'symlink' like pointer to whichever npp_plugin namsespace extension that will
 *  be responsible for responding to the N++ PluginsManager getfuncArray call.
 *
 *  An example of this is npp_plugin::external_lexer::getfuncArray() which adds lexer funcItems
 *  to the array and sorts them, then appends the plugin's primary funcItems in the order
 *  the were registered.
 *
 */
namespace v_getfuncarray = npp_plugin;

static Win32xx::CWinApp g_app;
static SearchDialog g_dlg(IDD_SEARCH_DIALOG);

static ShortcutKey g_extractShortcut = {true, false, true, 'e'};
volatile int g;

std::function<nana::form&(void)> g_fn;
std::atomic<bool> g_close_dialog = false;
//  <--- Required Plugin Interface Routines --->
BOOL APIENTRY DllMain(HANDLE hModule, DWORD reasonForCall, LPVOID /*lpReserved*/)
{
    using namespace npp_plugin;

    switch (reasonForCall)
    {

    case DLL_PROCESS_ATTACH:
        {
            // <--- Base initialization of the plugin object --->
            npp_plugin::initPlugin(TEXT("RegexExtract"), hModule);

            // <--- Base menu function items setup --->
            setPluginFuncItem(TEXT("Show main window"), npp_plugin::show_dialog, 0, false, &g_extractShortcut);
           std::vector<wchar_t> vPath(MAX_PATH + 1);
           int iPathChars = ::GetModuleFileNameW((HMODULE)hModule, &vPath[0], MAX_PATH); 
           std::experimental::filesystem::path p{vPath.begin(), vPath.end()};
           p = p.parent_path();
           p += "\\";
           if (!Options::initialize(p.wstring() + L"Config\\RegexExtractSettings.xml"))
           {
               ::MessageBox(nullptr, (std::wstring(L"Failed to open settings file: ") + p.wstring() + L"Config\\RegexExtractSettings.xml").c_str(), L"RegexExtract", MB_ICONEXCLAMATION);
           }
            std::locale::global(std::locale(""));

            


#ifdef NPP_PLUGININTERFACE_CMDMAP_EXTENSION_H
            // <--- Initalize internal cmdId to funcItem cmdId map. --->
            createCmdIdMap();
#endif
        }
        break;

    case DLL_PROCESS_DETACH: 
        ++g;
        break;

    case DLL_THREAD_ATTACH: 
        ++g;
        break;

    case DLL_THREAD_DETACH: 
        ++g;
        break;

    }

    return TRUE;
}

extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *FuncsArraySize)
{
    /*
     *  PluginsManager stores pointers to the functions that are exposed, allowing dialog and
     *  menu interaction.  The base plugin's functions to retrieve the size and array can be
     *  'virtualized' by altering the namespace alias located near the top this file.
     *
     */

    *FuncsArraySize = v_getfuncarray::getPluginFuncCount();

    return ( v_getfuncarray::getPluginFuncArray() );

}


//---------------------------------------------------------------------------------------------
//  Notepad++ and Scintilla Communications Processing
                
    /*
     *  Listings of notifications and messages are in Notepad_plus_msgs.h and Scintilla.h.
     *
     *  Notifications use the SCNotification structure described in Scintilla.h.
     *
     */

//  This function gives access to Notepad++'s notification facilities including forwarded
//  notifications from Scintilla.
extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode)
{
    /*
     *  
     *  Notifications can be filtered, and language specific handlers called using a
     *  Namespace::Function() call.
     *
     *  To route a notification to one of this plugin's registered lexers use the
     *  external_lexer::getSCILexerIDByName("LexerName") and compare with the  value returned
     *  by messageProc(SCI_GETLEXER, 0, 0).
     *
     */
    
    using namespace npp_plugin;
    //OutputDebugString((std::to_wstring(notifyCode->nmhdr.code) + L"\n").c_str());
    WINDOWPLACEMENT wp{0};
    wp.length = sizeof(WINDOWPLACEMENT);

    switch (notifyCode->nmhdr.code) 
    {
    case NPPN_READY:
        npp_plugin::setNppReady();
        npp_plugin::hCurrViewNeedsUpdate();
        break;
    case NPPN_SHUTDOWN:
        if (g_fn)
        {
            g_close_dialog = true; // allow window closing
            g_fn().close();
        }
                       break;
    case 0x7ED:
    case 0x7EC:
           // OutputDebugString(fmt::format(L"IsIconic: {}\n", ::IsWindowVisible(static_cast<HWND>(notifyCode->nmhdr.hwndFrom))).c_str());
        if (g_fn)
        {
            //::GetWindowPlacement(static_cast<HWND>(notifyCode->nmhdr.hwndFrom), &wp);
        } break;
   //     if (g_fn)
   //        g_fn().hide(); break;
   // case 0x7EC:
   //     if (g_fn)
   //         if (!g_fn().visible())
   //         {
   //             g_fn().show(); break;
   //         }
    default:
        break;
    }
}


extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam)
{
    /*
     *  This function give access to Notepad++'s messaging facilities.  It was originally
     *  intended for the relay of Notepad++ messages and inter-plugin messages, yet having
     *    ::SendMessage( hCurrentView, SOME_MESSAGE_TOKENNAME, value, value );
     *  mixed in all over the place was ugly so this plugin uses messageProc to keep all of
     *  these in one area.
     *
     *  This function either returns true or the return value that comes from each individual
     *  handler.  So be sure to explicitly state the return in the switch case.
     *
     *  Use the npp_plugin:: hCurrView, hMainView, hSecondView, and hNpp for the standard handles,
     *  some messages needs to be sent to both the main and second handle to get the desired
     *  results.  ( Indicator setup messages are one example. )
     *
     *  See Notepad_plus_msgs.h and Scintilla.h for notification IDs.
     *
     *  Some messages sent to N++ get forwarded back here by N++ plugin notify.  For instance
     *  NPPM_DOOPEN sent to messageProc will recieved a messageProc message again by the
     *  pluginNotify routine, so it needs to be sent via ::SendMessage
     *
     */

    using namespace npp_plugin;
    // ===>  Include optional messaging handlers here.
    switch (Message)
    {

        //  <---  Notepad++ Messages --->
        case NPPM_MSGTOPLUGIN:
        {
            //  Inter-Plugin messaging
            CommunicationInfo* comm = reinterpret_cast<CommunicationInfo *>(lParam);
            //OutputDebugString((std::to_wstring(comm->internalMsg) + L"\n").c_str());
            switch ( comm->internalMsg )
            {
                default:
                    break;
            break;
            }
        break;
        }

        default:
            return false;

    }  // End: Switch ( Message )

    return TRUE;
}

void npp_plugin::show_dialog()
{
    static bool b = false;
    if (!b)
    {
        std::thread t{npp_plugin::runMainDialog};
        while (!t.joinable())
        {
        }
        t.detach();
        b = true;
    }
    else
    {
        g_fn().show();
    }
}

#include "../inc/MainForm.hpp"
void npp_plugin::runMainDialog()
{
    static MainForm s_fm;

    auto window_handle = reinterpret_cast<HWND>(s_fm.native_handle());
    auto prev_style = ::GetWindowLong(window_handle, GWL_STYLE);
    ::SetWindowLong(window_handle, GWL_STYLE, ((prev_style & ~(WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)) | WS_CHILD));
    ::SetWindowPos(window_handle, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    ::SetParent(window_handle, npp_plugin::hMainView());
    ::SetWindowPos(window_handle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    s_fm.show();
    s_fm.events().unload([&](const nana::arg_unload& arg){
        arg.cancel = !g_close_dialog;
        s_fm.hide();
    }); 


    static auto fm_ret = [&]() -> nana::form&
    {
        return s_fm;
    };
    g_fn = fm_ret;
    nana::exec();
}
