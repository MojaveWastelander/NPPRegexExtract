#pragma once
#include "idatakind.hpp"

/// Object storing contents of a text file. Object is responsible for correct handling of input encoding.
class DataKindPath :
    public IDataKind
{
public:

    /// Constructor.
    ///
    /// \param  wsPath   Path to file.
    DataKindPath(const std::wstring& wsPath):
        IDataKind(wsPath) 
    {
        GetText();
    }
protected:
    void GetText();
};

