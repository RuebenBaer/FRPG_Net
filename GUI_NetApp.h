/***************************************************************
 * Name:      GUI_NetApp.h
 * Purpose:   Defines Application Class
 * Author:     ()
 * Created:   2016-12-24
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef GUI_NETAPP_H
#define GUI_NETAPP_H

#include <wx/app.h>
#include "Vektor\Vektor.h"
#include "NetzBerechnung.h"

class GUI_NetApp : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // GUI_NETAPP_H
