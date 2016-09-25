/////////////////////////////////////////////////////////////////////////////
// Name:        src/stc/dataviewrendererstc.cpp
// Purpose:     Implementation of wxDataViewStyledTextCtrlRenderer - A data view renderer for wxStyledTextCtrl (Scintilla)
// Author:      Malcolm MacLeod
// Created:     2012-02-10
// RCS-ID:      $Id$
// Copyright:   
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif // WX_PRECOMP

#include <wx/dataviewrendererstc.h>
#include <wx/stc/stc.h>
#include "ScintillaWX.h"
#include <wx/dcmemory.h>
#include <wx/rawbmp.h>
#include <cmath>

wxDataViewStyledTextCtrlRenderer::wxDataViewStyledTextCtrlRenderer(wxDataViewCellMode mode,int alignment)
: wxDataViewCustomRenderer("string",mode,alignment)
, m_impl(NULL)
, m_renderBuffer(NULL)
{
}

void wxDataViewStyledTextCtrlRenderer::Create(wxWindow* parent,int style)
{
    m_parent=parent;
    m_impl = AllocControl(wxDefaultPosition,wxSize(10000,10000));
    m_impl->Show(false);
}

wxDataViewStyledTextCtrlRenderer::~wxDataViewStyledTextCtrlRenderer()
{
    delete m_renderBuffer;
    //m_impl->Destroy();
}

wxStyledTextCtrl* wxDataViewStyledTextCtrlRenderer::AllocControl(const wxPoint& Pos,const wxSize& Size)
{
	wxStyledTextCtrl* Ret = new wxStyledTextCtrl(m_parent,wxID_ANY,Pos,Size);
	Ret->SetUseHorizontalScrollBar(false);
    Ret->SetMarginLeft(0);
	return Ret;
}

void wxDataViewStyledTextCtrlRenderer::DoCtrlSetup(wxStyledTextCtrl* ctrl,const wxVariant& value)
{
    ctrl->SetText(value.GetString());
    ctrl->SetUseHorizontalScrollBar(false);
    ctrl->SetMarginLeft(0);
}

wxWindow* wxDataViewStyledTextCtrlRenderer::CreateEditorCtrl( wxWindow *parent, wxRect labelRect, const wxVariant &value )
{
    wxStyledTextCtrl* implEdit=AllocControl(labelRect.GetTopLeft(),wxSize(labelRect.GetWidth(),labelRect.GetHeight()*1.5));
    
    DoCtrlSetup(implEdit,value);
    
    wxPoint mousePoint = wxGetMousePosition(); 
    implEdit->ScreenToClient(&mousePoint.x,&mousePoint.y);
    
    mousePoint.x-=labelRect.GetTopLeft().x;
    mousePoint.y-=labelRect.GetTopLeft().y;    
    
    int pos = implEdit->PositionFromPoint(mousePoint);
    implEdit->SetAnchor(pos);
    implEdit->SetCurrentPos(pos);

	implEdit->Show(true);

    return implEdit;
}

bool wxDataViewStyledTextCtrlRenderer::GetValueFromEditorCtrl( wxWindow* editor, wxVariant &value )
{
    value=dynamic_cast<wxStyledTextCtrl*>(editor)->GetText();
    return true;
}

bool wxDataViewStyledTextCtrlRenderer::Render( wxRect rect, wxDC *dc, int state )
{
    //Size implementation wxStyledTextCtrl appropriately and setup buffer
    //For speed reasons we only do this when size changes
    wxRect implRect;
    implRect.x=0;
    implRect.y=0;
    implRect.width=rect.width+std::abs(rect.x);
    implRect.height=rect.height+std::abs(rect.y);
    if(!m_renderBuffer||m_prevRect!=implRect)
    {
        delete m_renderBuffer;
        m_prevRect=implRect;
	//For some reason this results in first draw always being incorrect and 'damaging' output - so instead we leave size stable and use a really big initial size
        //m_impl->SetSize(implRect);
        //m_impl->Update();
        m_renderBuffer = new wxBitmap(implRect.width,implRect.height,24);
    }
      
    //Draw scintilla implementation control onto memory DC
    wxMemoryDC mydc; 
    mydc.SelectObject(*m_renderBuffer); 
    mydc.SetBackground(*wxWHITE_BRUSH);
    mydc.Clear();
    
    //Blit memory DC onto actual DC
    m_impl->m_swx->DoPaint(&mydc, implRect);
    dc->Blit(wxPoint(rect.x, rect.y), wxSize(rect.width, rect.height), &mydc, wxPoint(0, 0));
    
    //Cleanup of memory DC
    mydc.SelectObject(wxNullBitmap);
    return true;
}

wxSize wxDataViewStyledTextCtrlRenderer::GetSize() const
{
    return wxSize(600,40);
}

bool wxDataViewStyledTextCtrlRenderer::SetValue( const wxVariant &value )
{
    m_data = value.GetString();
    DoCtrlSetup(m_impl,m_data);
    return true;
}

bool wxDataViewStyledTextCtrlRenderer::GetValue( wxVariant &value ) const
{
    value = m_data;
    return true;
}
