#pragma once

#include "cppmarkdown/cppmarkdown.h"

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
