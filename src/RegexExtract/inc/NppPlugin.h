/* NppPlugin.h
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
  *  These includes provide plugins with functions that enhance the N++ plugin interface.
  *
  *  The basic interface enhancement provides plugin registration and handle controls, with 
  *  multiple common ways to access view handles.
  *
  *  The ExtLexer extension provides styler (lexer) registration helper functions for implementing 
  *  external lexers.
  *
  *  - Both of those also allow for function registration.  The basic difference between the
  *  two function registrations is the base functions display under a separator in the order
  *  registered, and the ExtLexer registered functions get sorted and display above the
  *  separator.
  *
  *  The CmdMap extension allows plugins to use an internal enum value for a cmdId when working
  *  with registered N++ funcItems.  This allows the plugin writer to register the commands
  *  with N++ in one order yet use the command values from an enum in an order more useful to
  *  the code, as well as not needing to deal with the funcItem array, and instead can use a
  *  call ( getCmdId( CMD_HIGHLIGHT ) ) to get the assigned N++ menu command id.
  *
  *  The DocTabMap provides plugins some common helper functions that allows plugins to get
  *  the Scintilla document id from a N++ bufferId, determine if a file is open and ready, and
  *  get the Scintilla document id for the actively focused document in either N++ view.
  *
  *  The XmlConfig extension makes it easy for a plugin to read/write configuration data in
  *  the same way that N++ does.  So, if the plugin provides styling, the plugins' styles
  *  xml can also be used for config params.
  *
  *  The Markers extension provides plugins with functions to read and react to the basic
  *  Notepad++/Scintilla margin and marker settings.  For instance if the plugin provides
  *  a marker the defineMarker function will check for a valid and free marker number to use,
  *  as well as set the mask information for the margins in both views.
  *
  *  The ActionIndex extension provides an action counter for a given document synchronized
  *  with the users actions for inserte/delete/undo/redo actions.
  *
  *  The ActionHistory extension provides a history container for a plugin to store actions
  *  that it wants to have tied to the users insert/delete/undo/redo actions.  It uses the 
  *  ActionIndex count for the 'major' index and allows multiple action item entries per index
  *  ( or 'minor index' ).  An example of this is the NppPlugin_ChangeMarker plugin that shows
  *  lines that have changed since a document was opened and if those changes are saved or not.
  *
  *  Lastly, the msgs extension provides a common place to store inter plugin msg definitions.


  *  TODO:  Two other extensions that would be very nice to have available are dialog related.
  *    1)  A Preferences dialog hook into the Edit Components panel.
  *    2)  A Styler Configurator hook to allow widget registration similar to the 'Global
  *        Overrides' checkboxes.
  *
  */

#ifndef NPP_PLUGIN_H
#define NPP_PLUGIN_H

//  <--- Notepad++ Plugin Interface Library Includes --->
#include <WinSock2.h>
#include <npp\NppPluginIface.h>
#include <npp\NppPluginIface_msgs.h>
//#include "NppPluginIface_ActionIndex.h"
//#include "NppPluginIface_ActionHistory.h"
//#include "NppPluginIface_CmdMap.h"
//#include "NppPluginIface_DocTabMap.h"
//#include "NppPluginIface_ExtLexer.h"
//#include "NppPluginIface_Markers.h"
//#include "NppPluginIface_XmlConfig.h"
//  <--- Windows --->
#include "Shlwapi.h"
#pragma comment( lib, "Shlwapi.lib" )

// RegexExtract includes
#include "RegexSearch.hpp"
#include "Options.hpp"
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>

namespace npp_plugin 
{
     void show_dialog();
     void runMainDialog();

}


#endif	//  Include Guard:  NPP_PLUGIN_H
