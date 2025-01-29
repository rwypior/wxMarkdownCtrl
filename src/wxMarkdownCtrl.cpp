// wxMarkdownCtrl.cpp : Defines the entry point for the application.
//

#include "wxMarkdownCtrl.h"

MarkdownCtrl::MarkdownCtrl()
    : wxHtmlWindow()
{
}

MarkdownCtrl::MarkdownCtrl(wxWindow* parent,
    wxWindowID id,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name
)
    : wxHtmlWindow(parent, id, pos, size, style, name)
{
}

void MarkdownCtrl::setMarkdown(const wxString& markdown)
{
    this->markdownDocument.clear();
	this->markdownDocument.parse(markdown.ToStdString());
    this->refreshMarkdown();
}

void MarkdownCtrl::refreshMarkdown()
{
    this->SetPage(this->markdownDocument.getHtml());
}