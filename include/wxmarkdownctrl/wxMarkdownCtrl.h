#pragma once

#include "cppmarkdown/cppmarkdown.h"

#include <wx/wx.h>
#include <wx/wxhtml.h>
#include <wx/string.h>

#include <regex>
#include <functional>

class MarkdownCtrl : public wxHtmlWindow
{
public:
    using wxHtmlWindow::wxHtmlWindow;

    using HtmlModifier = std::function<void(wxString&, MarkdownCtrl&)>;

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

    wxHtmlOpeningStatus OnOpeningURL(wxHtmlURLType type, const wxString& url, wxString* redirect) const
    {
        if (type == wxHtmlURLType::wxHTML_URL_IMAGE)
        {
            wxFileName urlPath = url;
            for (const auto& lookupPath : this->lookupPaths)
            {
                wxFileName path(lookupPath);
                for (const auto& dir : urlPath.GetDirs())
                {
                    path.AppendDir(dir);
                }
                path.SetFullName(urlPath.GetFullName());

                if (wxFile::Exists(path.GetFullPath()))
                {
                    *redirect = path.GetFullPath();
                    return wxHtmlOpeningStatus::wxHTML_REDIRECT;
                }
            }
        }

        return wxHtmlOpeningStatus::wxHTML_OPEN;
    }

    void appendLookPath(const wxString& path)
    {
        this->lookupPaths.push_back(path);
    }

    void clearLookupPaths()
    {
        this->lookupPaths.clear();
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
            mod(html, *this);
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

    void insertImageWidth(const wxString& width = "100%", bool apply = true)
    {
        this->addModifier(createImageWidthModifier(width));

        if (apply)
            this->applyModifiers();
    }

    // Create a modifier that inserts custom text into head tag
    static HtmlModifier createHeadModifier(const wxString& head)
    {
        return [head](wxString& html, MarkdownCtrl&) {
            size_t headpos = html.find("</head>");
            if (headpos != wxString::npos)
            {
                html.insert(headpos, head);
            }
            };
    }

    // Create a modifier that inserts background color to body tag
    static HtmlModifier createBackgroundColorModifier(const wxString& color)
    {
        return [color](wxString& html, MarkdownCtrl&) {
            size_t pos = html.find("<body");
            if (pos != wxString::npos)
            {
                html.insert(pos + 5, wxString(" bgcolor=\"") + color + "\"");
            }
            };
    }

    // Create a modifier that inserts text color to body tag
    static HtmlModifier createTextColorModifier(const wxString& color)
    {
        return [color](wxString& html, MarkdownCtrl&) {
            size_t pos = html.find("<body");
            if (pos != wxString::npos)
            {
                html.insert(pos + 5, wxString(" text=\"") + color + "\"");
            }
            };
    }

    // Create a modifier that inserts link colors to body tag
    static HtmlModifier createLinkColorModifier(const wxString& normal, const wxString& active = "", const wxString& visited = "")
    {
        return [normal, active, visited](wxString& html, MarkdownCtrl&) {
            size_t pos = html.find("<body");
            if (pos != wxString::npos)
            {
                html.insert(pos + 5, wxString(" link=\"") + normal + "\" alink=\"" + active + "\" vlink=\"" + visited + "\"");
            }
            };
    }

    // Create a modifier that inserts width attribute to images that exceed window size
    static HtmlModifier createImageWidthModifier(const wxString& width = "100%")
    {
        return [width](wxString& html, MarkdownCtrl& wnd) {

            auto windowWidth = wnd.GetClientSize().GetWidth();
            std::string html2 = html;
            size_t begin = 0;
            std::regex imgregex("<img.*?src=\\\"(.+?)\\\".*?>");
            for (std::smatch matches; std::regex_search(html2, matches, imgregex); html2 = matches.suffix())
            {
                auto pos = matches.position();
                auto len = matches.length();

                std::string replacement;
                if (matches.size() >= 2)
                {
                    std::string src = matches[1];
                    wxString redirect;
                    wxHtmlOpeningStatus openStatus = wnd.OnOpeningURL(wxHtmlURLType::wxHTML_URL_IMAGE, src, &redirect);

                    if (openStatus == wxHtmlOpeningStatus::wxHTML_REDIRECT)
                        src = redirect;

                    wxImage img;
                    if (img.LoadFile(src) && img.IsOk())
                    {
                        int imgWidth = img.GetWidth();
                        if (imgWidth > windowWidth)
                        {
                            replacement = matches[0];
                            replacement.insert(replacement.length() - 1, " width=\"" + width + "\"");
                        }
                    }
                }

                if (!replacement.empty())
                {
                    html.replace(begin + pos, len, replacement);
                    begin += pos + replacement.length();
                }
                else
                    begin += pos + len;;
            }
            };
    }

private:
    Markdown::Document markdownDocument;
    wxString html = "<!DOCTYPE html><html><head></head><body></body></html>";
    wxVector<HtmlModifier> modifiers;
    wxVector<wxString> lookupPaths;
};
