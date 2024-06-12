/***************************************************************
 * Name:      GUI_NetApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2016-12-24
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "GUI_NetApp.h"
#include "GUI_NetMain.h"

IMPLEMENT_APP(GUI_NetApp);

bool GUI_NetApp::OnInit()
{
    GUI_NetFrame* frame = new GUI_NetFrame(0L, _("FRPG Netz"));
    frame->SetIcon(wxICON("RUZmBi.ico")); // To Set App Icon
    frame->Show();

    return true;
}
