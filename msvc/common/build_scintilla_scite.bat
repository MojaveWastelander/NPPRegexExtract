rem echo off
set bin=%2
set npp_path=%1
set npp_base_path=%1..
set sc_path=%npp_base_path%\..\scintilla

if exist "%bin%\Scintilla.dll" exit

echo N++ path %npp_path%
echo Scintilla path %sc_path%
echo bin path %bin%
cd %sc_path%\win32
nmake -f scintilla.mak clean
nmake -f scintilla.mak
copy %npp_base_path%\bin\Scintilla.dll %bin%\Scintilla.dll
copy %npp_base_path%\bin\SciLexer.dll %bin%\SciLexer.dll
copy %npp_base_path%\src\langs.model.xml %bin%\langs.model.xml
copy %npp_base_path%\src\stylers.model.xml %bin%\stylers.model.xml
copy %npp_base_path%\src\shortcuts.xml %bin%\shortcuts.xml
copy %npp_base_path%\src\functionList.xml %bin%\functionList.xml
copy %npp_base_path%\src\contextMenu.xml %bin%\contextMenu.xml