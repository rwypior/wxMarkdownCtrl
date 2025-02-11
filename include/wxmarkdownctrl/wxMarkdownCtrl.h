#pragma once

#include "cppmarkdown/cppmarkdown.h"

#include <wx/wx.h>
#include <wx/wxhtml.h>
#include <wx/string.h>

#include <functional>

class MarkdownCtrl : public wxHtmlWindow
{
public:
    using wxHtmlWindow::wxHtmlWindow;

    using HtmlModifier = std::function<void(wxString&)>;

    void setMarkdown(const wxString& markdown, bool applyModifiers = true)
    {
        this->markdownDocument.clear();
        this->markdownDocument.parse(markdown.ToStdString());
        this->refreshMarkdown(applyModifiers);
    }

    void refreshMarkdown(bool applyModifiers)
    {
        this->html = this->markdownDocument.getHtml();

        if (applyModifiers)
            this->applyModifiers();
        else
            this->SetPage(this->html);
    }

    Markdown::Document& getMarkdownDocument()
    {
        return this->markdownDocument;
    }

    void addModifier(HtmlModifier mod)
    {
        this->modifiers.push_back(mod);
    }

    void clearModifiers(bool apply = true)
    {
        this->modifiers.clear();

        if (apply)
            this->applyModifiers();
    }

    void applyModifiers()
    {
        wxString html = this->html;
        for (HtmlModifier mod : this->modifiers)
        {
            mod(html);
        }
        this->SetPage(html);
    }

    void insertHead(const wxString& html, bool apply = true)
    {
        this->addModifier(createHeadModifier(html));

        if (apply)
            this->applyModifiers();
    }

    void insertBackgroundColor(const wxString& color, bool apply = true)
    {
        this->addModifier(createBackgroundColorModifier(color));

        if (apply)
            this->applyModifiers();
    }

    void insertTextColor(const wxString& color, bool apply = true)
    {
        this->addModifier(createTextColorModifier(color));

        if (apply)
            this->applyModifiers();
    }

    void insertLinkColor(const wxString& normal, const wxString& active = "", const wxString& visited = "", bool apply = true)
    {
        this->addModifier(createLinkColorModifier(normal, active, visited));

        if (apply)
            this->applyModifiers();
    }

    static HtmlModifier createHeadModifier(const wxString& head)
    {
        return [head](wxString& html) {
            size_t headpos = html.find("</head>");
            if (headpos != wxString::npos)
            {
                html.insert(headpos, head);
            }
        };
    }

    static HtmlModifier createBackgroundColorModifier(const wxString& color)
    {
        return [color](wxString& html) {
            size_t pos = html.find("<body");
            if (pos != wxString::npos)
            {
                html.insert(pos + 5, wxString(" bgcolor=\"") + color + "\"");
            }
        };
    }

    static HtmlModifier createTextColorModifier(const wxString& color)
    {
        return [color](wxString& html) {
            size_t pos = html.find("<body");
            if (pos != wxString::npos)
            {
                html.insert(pos + 5, wxString(" text=\"") + color + "\"");
            }
        };
    }

    static HtmlModifier createLinkColorModifier(const wxString& normal, const wxString& active = "", const wxString& visited = "")
    {
        return [normal, active, visited](wxString& html) {
            size_t pos = html.find("<body");
            if (pos != wxString::npos)
            {
                html.insert(pos + 5, wxString(" link=\"") + normal + "\" alink=\"" + active + "\" vlink=\"" + visited + "\"");
            }
        };
    }

private:
    Markdown::Document markdownDocument;
    wxString html = "<!DOCTYPE html><html><head></head><body></body></html>";
    wxVector<HtmlModifier> modifiers;
};
