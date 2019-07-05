RegexExtract is a Notepad++ plugin that allows to extract text using regular expressions from Notepad++ opened files or files on storage.
Searching and extracting is done through plugin modal dialog.

Plugin dialog in Win32++
![main_dialog](docs/main_window.png)

**How to build**

Clone repo:
```
$ git clone https://github.com/MojaveWastelander/NPPRegexExtract.git
$ cd NPPRegexExtract
```

Switch to old_version branch
```
$ git checkout old_version
```

Update submodules
```
$ git submodule update --init --recursive
```

Update Visual Studio project
```
$ cd msvc\common\npp
$ update_npp_proj.bat
```

**Prepare dependencies**

Install and integrate vcpkg as described [here](https://github.com/Microsoft/vcpkg)

Install following packages: boost-format, boost-property-tree, boost-regex, fmt, catch2. 

E.g. for x64 static build an install will look like this
```
$ vcpkg install boost-regex --triplet x64-windows-static
```

Download [Win32++](https://sourceforge.net/projects/win32-framework) and extract archive contents to `external/Win32xx` directory

Open the solution from `msvc/RegexExtract/RegexExtract.sln` and build `RegexExtract`

When building `RegexExtract` it will first build Notepad++ project, then scintilla dlls (if they are missin in bin folder) then the plugin itself.

**Used 3rd party libraries**

[Alphanum Algorithm](http://www.davekoelle.com/alphanum.html)

[hpslib](https://www.codeproject.com/articles/22046/high-performance-unicode-text-file-i-o-routines-fo)

[utf8conv](https://github.com/GiovanniDicanio/UnicodeConversions)

[win32++](https://sourceforge.net/projects/win32-framework)

[boost](https://www.boost.org)

[fmtlib](https://github.com/fmtlib/fmt)

[catch2](https://github.com/catchorg/Catch2)


