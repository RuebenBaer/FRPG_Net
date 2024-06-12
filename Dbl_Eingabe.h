/*  SISerator - creates Single Image Stereograms from Heightmaps and Textures (both loaded from user) or Single Image Random Dot Stereograms from Heightmaps
    Copyright (C) 2015  Ansgar Rütten

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef __Dbl_Eingabe_H
#define __Dbl_Eingabe_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include "wx/wx.h"
#endif

class aruDblTxtCtrl : public wxTextCtrl
{
	public:
		aruDblTxtCtrl(wxWindow*, wxWindowID, const wxString&, const wxPoint&, const wxSize&,
						long style, const wxValidator&, const wxString&);
		~aruDblTxtCtrl();
		void OnLostFocus(wxFocusEvent &event);
		void OnGotFocus(wxFocusEvent &event);
		void OnKeyPress(wxKeyEvent &event);
		void OnKeyRelease(wxKeyEvent &event);
		void OnLeftUp(wxMouseEvent& event);
		/*Nachfolger und Vorgaenger fuer TAB und ENTER*/
		void SetNachfolger(wxWindow*);
		void SetVorgaenger(wxWindow*);
		double HoleWert(void);
	private:
	    double m_wert;
		void KommaGegenPunkt(wxString*);
		wxWindow *vorheriger, *naechster;
		DECLARE_EVENT_TABLE()
};

#endif
