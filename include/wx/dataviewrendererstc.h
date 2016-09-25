/////////////////////////////////////////////////////////////////////////////
// Name:        wx/dataviewrendererstc.h
// Purpose:     wxDataViewRendererStyledTextCtrl - wxDataViewCtrl renderer for wxStyledTextCtrl (Scintilla)
// Author:      Malcolm Macleod
// Created:     08.01.06
// RCS-ID:      $Id$
// Copyright:   
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_STC_RENDERER_STC_H_
#define _WX_STC_RENDERER_STC_H_

#include "wx/defs.h"
#include <wx/dataview.h>

class wxBitmap;
class wxStyledTextCtrl;
class wxPoint;
class wxSize;


class WXDLLIMPEXP_STC wxDataViewStyledTextCtrlRenderer: public wxDataViewCustomRenderer
{
public:
    wxDataViewStyledTextCtrlRenderer(wxDataViewCellMode mode,int alignment);
    virtual void Create(wxWindow* parent,int style=0);
    virtual ~wxDataViewStyledTextCtrlRenderer();
    virtual bool HasEditorCtrl() const { return true; }
    virtual wxWindow* CreateEditorCtrl( wxWindow *parent, wxRect labelRect, const wxVariant &value );
    virtual bool GetValueFromEditorCtrl( wxWindow* editor, wxVariant &value );
    virtual bool Render( wxRect rect, wxDC *dc, int state );
    virtual wxSize GetSize() const;
    virtual bool SetValue( const wxVariant &value );
    virtual bool GetValue( wxVariant &value ) const;
    wxStyledTextCtrl* GetImpl(){return m_impl;};
protected:
    //Opportunity to overload control with a custom one - that e.g implements its own lexer
    virtual wxStyledTextCtrl* AllocControl(const wxPoint& Pos,const wxSize& Size);
    //Opportunity to overload styling/parsers etc. when setting text
    virtual void DoCtrlSetup(wxStyledTextCtrl* ctrl,const wxVariant& value);
private:
	wxWindow* m_parent;
    wxStyledTextCtrl* m_impl;
    wxString m_data;
    wxRect m_prevRect;
    wxBitmap* m_renderBuffer;
};

#endif
