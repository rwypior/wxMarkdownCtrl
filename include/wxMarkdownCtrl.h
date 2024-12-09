// wxMarkdownCtrl.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <wx/wx.h>

#include <iostream>

// TODO: Reference additional headers your program requires here.

class MarkdownCtrl : public wxWindow
{
public:
    MarkdownCtrl()
        : wxWindow()
    {
    }

    MarkdownCtrl(wxWindow* parent,
        wxWindowID id,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = wxASCII_STR(wxPanelNameStr))
        : wxWindow(parent, id, pos, size, style, name)
    {
    }
};
