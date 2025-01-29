#pragma once

#include "cppmarkdown/cppMarkdown.h"

#include <wx/wx.h>
#include <wx/wxhtml.h>

class MarkdownCtrl : public wxHtmlWindow
{
public:
    MarkdownCtrl();

    MarkdownCtrl(wxWindow* parent,
        wxWindowID id,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = wxASCII_STR(wxPanelNameStr));

    void setMarkdown(const wxString &markdown);
    void refreshMarkdown();

private:
    Markdown::Document markdownDocument;
};
