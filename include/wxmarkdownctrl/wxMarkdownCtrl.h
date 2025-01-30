//#pragma once
//
//#include "cppmarkdown/cppMarkdown.h"
//
//#include <wx/wx.h>
//#include <wx/wxhtml.h>
//
//class MarkdownCtrl : public wxHtmlWindow
//{
//public:
//    MarkdownCtrl();
//
//    MarkdownCtrl(wxWindow* parent,
//        wxWindowID id,
//        const wxPoint& pos = wxDefaultPosition,
//        const wxSize& size = wxDefaultSize,
//        long style = 0,
//        const wxString& name = wxASCII_STR(wxPanelNameStr));
//
//    void setMarkdown(const wxString &markdown);
//    void refreshMarkdown();
//
//private:
//    Markdown::Document markdownDocument;
//};

#pragma once

#include "cppmarkdown/cppMarkdown.h"

#include <wx/wx.h>
#include <wx/wxhtml.h>

class MarkdownCtrl : public wxHtmlWindow
{
public:
    using wxHtmlWindow::wxHtmlWindow;

    void setMarkdown(const wxString& markdown)
    {
        this->markdownDocument.clear();
        this->markdownDocument.parse(markdown.ToStdString());
        this->refreshMarkdown();
    }

    void refreshMarkdown()
    {
        this->SetPage(this->markdownDocument.getHtml());
    }

    Markdown::Document& getMarkdownDocument()
    {
        return this->markdownDocument;
    }

private:
    Markdown::Document markdownDocument;
};
