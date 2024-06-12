#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif
#include "Dbl_Eingabe.h"

BEGIN_EVENT_TABLE(aruDblTxtCtrl, wxTextCtrl)
	EVT_SET_FOCUS(aruDblTxtCtrl::OnGotFocus)
	EVT_KILL_FOCUS(aruDblTxtCtrl::OnLostFocus)
	EVT_KEY_DOWN(aruDblTxtCtrl::OnKeyPress)
	EVT_KEY_UP(aruDblTxtCtrl::OnKeyRelease)
	EVT_LEFT_UP(aruDblTxtCtrl::OnLeftUp)
END_EVENT_TABLE()

aruDblTxtCtrl::aruDblTxtCtrl(wxWindow* parent, wxWindowID ID, const wxString& Value, const wxPoint& t_pos, const wxSize& t_size,
						long style, const wxValidator& validator, const wxString& name)
						:wxTextCtrl(parent, ID, Value, t_pos, t_size, style, validator, name)
{
	vorheriger = parent;
	naechster = parent;
}

aruDblTxtCtrl::~aruDblTxtCtrl()
{
}

void aruDblTxtCtrl::OnLostFocus(wxFocusEvent& WXUNUSED(event))
{
	wxString tempStr = GetValue();
	KommaGegenPunkt(&tempStr);
	SelectNone();
	SetValue(tempStr);
	return;
}

void aruDblTxtCtrl::OnGotFocus(wxFocusEvent& WXUNUSED(event))
{
	//SetSelection(0, GetLineText(0).Len());
	SelectAll();
	return;
}

void aruDblTxtCtrl::OnKeyPress(wxKeyEvent &event)
{
	int KeyCode = event.GetKeyCode();
	if((KeyCode>31)&&(KeyCode<44))return; /*Zeichen bis Komma*/
	if((KeyCode==45))
    {
        if(!event.ShouldPropagate())event.ResumePropagation(1);
	    event.Skip();
        return; /*'-'*/
    }
	if((KeyCode==47))return; /*'/'*/
	if((KeyCode>57)&&(KeyCode<127))return; /*Zeichen jenseits der Zahlen*/

	switch(KeyCode)
	{
		/*Eingabeabschluß*/
		case WXK_TAB:
		case WXK_RETURN:
		case WXK_NUMPAD_TAB:
    	case WXK_NUMPAD_ENTER:
			if(event.ControlDown())
			{
				SetSelection(0, GetLineText(0).Len());
			}else{
				if(event.ShiftDown())
				{
					if(vorheriger) vorheriger->SetFocus();
				}else{
					if(naechster) naechster->SetFocus();
				}
			}
			wxCommandEvent myEvent = wxCommandEvent(wxEVT_COMMAND_TEXT_ENTER);
			myEvent.SetId(this->GetId());
			AddPendingEvent(myEvent);
			break;
	}
	/*Zahlen, Kommas, Punkte und Steuertasten*/
	if((KeyCode==',') || (KeyCode=='.') || (KeyCode)==WXK_NUMPAD_DECIMAL)
	{
		wxString Inhalt = GetValue();
		for(size_t i=0; i<Inhalt.Len(); i++)
		{
			if(Inhalt.GetChar(i) == ',')return;
			if(Inhalt.GetChar(i) == '.')return;
		}
	}
	if(KeyCode==13)return;
	if(KeyCode==WXK_NUMPAD_ENTER)return;
	if(!event.ShouldPropagate())event.ResumePropagation(1);
	event.Skip();
	return;
}

void aruDblTxtCtrl::OnKeyRelease(wxKeyEvent &event)
{
    if(!event.ShouldPropagate())event.ResumePropagation(1);
    event.Skip();
    return;
}

void aruDblTxtCtrl::OnLeftUp(wxMouseEvent& event)
{
    SelectAll();
	//SetSelection(0, GetLineText(0).Len());
	event.Skip();
	return;
}

void aruDblTxtCtrl::KommaGegenPunkt(wxString *Txt)
{
	for(size_t i=0; i<Txt->Len(); i++)
	{
		if(Txt->GetChar(i) == ',') Txt->SetChar(i, '.');
	}
	return;
}

void aruDblTxtCtrl::SetNachfolger(wxWindow* win)
{
	naechster = win;
	return;
}

void aruDblTxtCtrl::SetVorgaenger(wxWindow* win)
{
	vorheriger = win;
	return;
}

double aruDblTxtCtrl::HoleWert(void)
{
    wxString tempStr = GetValue();
    tempStr.ToDouble(&m_wert);
    return m_wert;
}
