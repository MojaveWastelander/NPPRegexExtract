#pragma once
#include "Base.h"

/// Object that store data for parsing. Provide basic interface so it can be extended to have other data such as URL contents.
class IDataKind
{
public:
    IDataKind() = default;
    IDataKind(const std::wstring& wsRef):
        m_wsRef{wsRef}, m_bParsed(false), m_bMatched(false) {; }

    IDataKind(std::wstring&& wsRef):
        m_wsRef{std::move(wsRef)}, m_bParsed(false), m_bMatched(false) {; }

    void SetParsed() { m_bParsed = true; }

    /// Query if this object is parsed.
    ///
    /// \return true if parsed, false if not.
    bool IsParsed() const { return m_bParsed; }
    void SetMatched(bool bMatched) { m_bMatched = bMatched; }

    /// Query if this expression was matched at least once.
    ///
    /// \return true if matched, false if not.
    bool IsMatched() const { return m_bMatched; }

    /// Gets reference to original data location string (path, URL, string).
    const std::wstring& data_location() const { return m_wsRef; }

    /// Gets raw text data from location specified by m_wsRef.
    const std::wstring& raw_text_data() const { return m_wsRawTextData; }
    virtual ~IDataKind(void) {}
protected:
    std::wstring m_wsRef; //< Reference to data location
    std::wstring m_wsRawTextData; //< Text data in raw format
    bool m_bParsed;
    bool m_bMatched;
    /// Function implemented by child classes which gets data from location specified in m_wsRef.
    virtual void GetData() = 0;
};



