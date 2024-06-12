/***************************************************************
 * Name:      GUI_NetMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2016-12-24
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef GUI_NETMAIN_H
#define GUI_NETMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/wx.h>

#include "NetzBerechnung.h"
#include "Dbl_Eingabe.h"
#include "Kamera_3D.h"
#include "Verkettete_Liste\Verkettete_Liste.h"

class GUI_NetOptions;


class GUI_NetFrame: public wxFrame
{
    public:
        GUI_NetFrame(wxFrame *frame, const wxString& title);
        ~GUI_NetFrame();
    private:
        Vektor mPos;
        enum
        {
            idMenuQuit = 1000,
            idMenuAbout, idMenuFileOpen, idMenuFileSavePic, idTimer
        };

        wxTimer timer;
        int TIMERTICK, MAX_ABTEILE, aktiverPkt;
        Station *abteil;

        //int abzugX, abzugY;
        float offsetX, offsetY;
        float zeichenFkt;
        wxPoint AlteMousePosition, AltePhysMousePosition;

        wxFileDialog *FileHandler;
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnOpenFile(wxCommandEvent& event);
        void OnSavePic(wxCommandEvent& event);
        void OnPaint(wxPaintEvent& event);
        void OnKeyDown(wxKeyEvent& event);
        void OnKeyUp(wxKeyEvent& event);
        void OnMouseMove(wxMouseEvent& event);
        void OnMouseWheel(wxMouseEvent& event);
        void OnMouseClick(wxMouseEvent& event);
        void OnTimer(wxTimerEvent& event);
        void OnEraseBackground(wxEraseEvent& event);

        void AmGitterRuetteln(float maximum);
        void Z_Nullen(void);
        int FindeNaechstesAbteil(Vektor& tPos);

        bool berechnungLaeuft, pktSchiebenAktiv, mouselook, anaglypheLook;

        float EineRundeRechnen(void);
        double g_intervall, g_verbAbst, g_loserAbst, g_daempfung, g_ruettelruck;

        Kamera* dasAuge;
        bool KameraVor, KameraZurueck, KameraRechts, KameraLinks, KameraHoch, KameraRunter;
        double halbesAuge;
        double kameraGeschw;

    public:
        void SetzeIntevall(double wert){g_intervall=wert;return;};
        void SetzeVerbAbst(double wert){g_verbAbst=wert;return;};
        void SetzeLoserAbst(double wert){g_loserAbst=wert;return;};
        void SetzeDaempfung(double wert){g_daempfung=wert;return;};
        void SetzeRuettelRuck(double wert){g_ruettelruck=wert;return;};
        void SetzeAugenAbstand(double wert);
        void SetzeKameraGeschw(double wert);

        /*Logbuch*/
        void logSchreiben(const char*);
        void logSchreiben(const float msg, int i=0);
        void logSchreiben(const void *msg);
        /*ENDE Logbuch*/

        DECLARE_EVENT_TABLE()
};

class GUI_NetOptions : public wxFrame
{
    public:
        GUI_NetOptions(GUI_NetFrame *frame, const wxString& title, double intervall, double verbAbst,
                       double loserAbst, double daempfung, double ruettelruck, double Auge, double KameraGeschw);
        ~GUI_NetOptions();
    private:
        enum
        {
            idOptionQuit = 2000,
            idIntervall, idVerbAbst, idLoserAbst, idDaempfung, idRuettelruck, idAuge, idKameraGeschw
        };

        aruDblTxtCtrl *m_intervall, *m_verbAbst, *m_loserAbst, *m_daempfung, *m_ruettelruck, *m_Auge, *m_KameraGeschw;
        GUI_NetFrame *Mama;

        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnParameterEingabe(wxCommandEvent& event);

        void ParentUpdaten(void);

        DECLARE_EVENT_TABLE()
};

#endif // GUI_NETMAIN_H
