/***************************************************************
 * Name:      GUI_NetMain.cpp
 * Purpose:   Code for Application Frame
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

#include <wx/dcbuffer.h>
#include <wx/timer.h>
#include "GUI_NetMain.h"
#include "NetzBerechnung.h"
#include "GUI_NetOptions.h"
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <fstream>



BEGIN_EVENT_TABLE(GUI_NetFrame, wxFrame)
    EVT_CLOSE(GUI_NetFrame::OnClose)
    EVT_MENU(idMenuFileOpen, GUI_NetFrame::OnOpenFile)
    EVT_MENU(idMenuFileSavePic, GUI_NetFrame::OnSavePic)
    EVT_MENU(idMenuQuit, GUI_NetFrame::OnQuit)
    EVT_MENU(idMenuAbout, GUI_NetFrame::OnAbout)
    EVT_PAINT(GUI_NetFrame::OnPaint)
    EVT_KEY_DOWN(GUI_NetFrame::OnKeyDown)
    EVT_KEY_UP(GUI_NetFrame::OnKeyUp)
    EVT_TIMER(idTimer, GUI_NetFrame::OnTimer)
    EVT_MOUSE_EVENTS(GUI_NetFrame::OnMouseMove)
    EVT_MOUSEWHEEL(GUI_NetFrame::OnMouseWheel)
    EVT_LEFT_DOWN(GUI_NetFrame::OnMouseClick)
    EVT_RIGHT_DOWN(GUI_NetFrame::OnMouseClick)
    EVT_ERASE_BACKGROUND(GUI_NetFrame::OnEraseBackground)
END_EVENT_TABLE()

GUI_NetFrame::GUI_NetFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, 1, title)
{
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idMenuQuit, _("&Beenden\tAlt-F4"), _("Anwendung schließen"));
    fileMenu->Append(idMenuFileOpen, _("&Struktur öffnen\tF3"), _("Öffnen einer Datei mit Strukturinformationen"));
    fileMenu->Append(idMenuFileSavePic, _("&Bild speichern\tF2"), _("Speichert die Struktur als PNG"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&Optionen\tF1"), _("Einstellungen zu verschiedenen Parametern"));
    mbar->Append(helpMenu, _("&Optionen"));

    SetMenuBar(mbar);

    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(3);

    MAX_ABTEILE = 1;

    abteil = new Station [MAX_ABTEILE];
    for(int i=0; i<MAX_ABTEILE; i++)
    {
        abteil[i].gesetzt = false;
        abteil[i].loesung = false;
        for(int j=0; j<20; j++)
        {
            abteil[i].Anhang[j] = NULL;
            abteil[i].lsgWeg[j] = false;
        }
    }

    FileHandler = new wxFileDialog(this, _("Struktur öffnen"),_(""),_(""),_("*.txt"),1);

    timer.SetOwner(this, idTimer);
    TIMERTICK = 100;

    zeichenFkt = 100;
    SetStatusText(wxString::Format("Zeichenfaktor: %.1f", zeichenFkt), 2);
    offsetX = 0.0;
    offsetY = 0.0;

    g_intervall = 0.005;
    g_verbAbst = 10.0;
    g_loserAbst = 20.0;
    g_daempfung = 0.05;
    g_ruettelruck = 0.005;

    pktSchiebenAktiv = false;
    mouselook = false;
    anaglypheLook = false;

    Vektor Anfangsvektor = Vektor(0, 100, 0);
    dasAuge = new Kamera(Anfangsvektor, 0, -1.57, 3.5, 30.0);
    KameraVor = KameraZurueck = KameraRechts = KameraLinks = KameraHoch = KameraRunter = false;
    kameraGeschw = 2.0;
    halbesAuge = 3.0;
}

GUI_NetFrame::~GUI_NetFrame()
{
    delete []abteil;
    delete dasAuge;
}

void GUI_NetFrame::OnOpenFile(wxCommandEvent &event)
{
    FileHandler->SetMessage(_("Struktur öffnen"));
    FileHandler->SetWildcard("*.txt");
    int Rueckgabe = FileHandler->ShowModal();
    if(Rueckgabe==wxID_CANCEL)return;
	SetStatusText(FileHandler->GetPath());

    //abteil = NULL;
    MAX_ABTEILE = StrukturGroesse(static_cast<const char*>(FileHandler->GetPath().c_str()));
    wxMessageDialog(this, wxString::Format("Anzahl der Stationen in der Stuktur:\n%d", MAX_ABTEILE)).ShowModal();
    if(MAX_ABTEILE<1)return;
    delete []abteil;
    abteil = new Station[MAX_ABTEILE];
    for(int i=0; i<MAX_ABTEILE; i++)
    {
        abteil[i].gesetzt = false;
        abteil[i].loesung = false;
        abteil[i].Nr = i;
        for(int j=0; j<20; j++)
        {
            abteil[i].Anhang[j] = NULL;
            abteil[i].lsgWeg[j] = 0;
        }
    }

    /*Einlesen*/
    bool hatOrte = StrukturEinlesen(static_cast<const char*>(FileHandler->GetPath().c_str()), abteil, MAX_ABTEILE);

    srand (time(NULL));
    /*Alles Nullen*/
    //netzHaut.ListeLoeschen("GUI_NetFrame::OpenFile");
    if(!hatOrte)
    {
        for(int i=0; i<MAX_ABTEILE; i++)
        {
            abteil[i].Ort = Vektor(rand()%10-5, rand()%10-5, rand()%10-5);
            abteil[i].Zuwachs = Vektor(0, 0, 0);

            //netzHaut.Hinzufuegen();
        }
    }
    Refresh();
    //timer.Start(TIMERTICK);
    return;
}

void GUI_NetFrame::OnSavePic(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(this, _("Bild der Struktur speichern"), "", "",
                       "PNG-Bild (*.png)|*.png", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    int Rueckgabe = saveFileDialog.ShowModal();
    if(Rueckgabe == wxID_CANCEL)return;

    wxImage::AddHandler(new wxPNGHandler());

    /*Grenzen suchen*/
    int min_x, max_x, min_y, max_y;
    min_x = max_x = abteil[0].Ort.x();
    min_y = max_y = abteil[0].Ort.y();
    for(int i = 1; i<MAX_ABTEILE; i++)
    {
        if(abteil[i].Ort.x() > max_x)max_x = abteil[i].Ort.x();
        if(abteil[i].Ort.x() < min_x)min_x = abteil[i].Ort.x();
        if(abteil[i].Ort.y() > max_y)max_y = abteil[i].Ort.y();
        if(abteil[i].Ort.y() < min_y)min_y = abteil[i].Ort.y();
    }

    wxBitmap temp_BM((max_x - min_x + 2)*zeichenFkt, (max_y - min_y + 2)*zeichenFkt, 32);
    if(!temp_BM.IsOk())
    {
        SetStatusText("Bitmap konnte nicht erzeugt werden!", 1);
        return;
    }
    wxMemoryDC dc(temp_BM);

    /*Hintergrund zeichnen*/
    wxRect rect(wxPoint(0, 0), dc.GetSize());
    wxColour back = wxColour(255, 255, 255);
    dc.SetBrush(wxBrush(back));
    dc.SetPen(wxPen(back, 1));
    dc.DrawRectangle(rect);

    wxFont TextFont = wxFont((int)(0.1*zeichenFkt), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    dc.SetFont(TextFont);
    wxColour TextColour = wxColour(0, 0, 0);
    dc.SetTextForeground(TextColour);
    TextColour = wxColour(0,0,0);
    dc.SetTextBackground(TextColour);

    /*Verschub und Skalierung*/
    float abzugX = (1 - min_x) * zeichenFkt;
    float abzugY = (1 - min_y) * zeichenFkt;

    /*Kreise*/
    for(int i = 0; i<MAX_ABTEILE; i++)
    {
        if(abteil[i].loesung)
        {
            dc.SetPen(wxPen(wxColour(180, 40, 0), 2));
        }else
        if(i == aktiverPkt)
        {
            dc.SetPen(wxPen(wxColour(20, 140, 0), 1));
        }else
        {
            dc.SetPen(wxPen(wxColour(0, 0, 0), 1));
        }
        dc.DrawCircle(abteil[i].Ort.x()*zeichenFkt + abzugX,
                        abteil[i].Ort.y()*zeichenFkt + abzugY,
                        (int)(0.05*zeichenFkt));
    }

    /*Linien*/
    float l_Pfeil = 0.25, b_Pfeil = 0.0625;

    for(int i = 0; i<MAX_ABTEILE; i++)
    {
        for(int j = 0; j<20; j++)
        {
            if(abteil[i].Anhang[j] != NULL)
            {
                if(abteil[i].loesung && abteil[i].lsgWeg[j])
                {
                    dc.SetPen(wxPen(wxColour(180, 40, 0), 2));
                }else
                {
                    dc.SetPen(wxPen(wxColour(0, 0, 0), 1));
                }
                /*Pfeilspitze*/
                Vektor richtung, senkrecht, pfeil_1, pfeil_2, endeLinie;
                richtung = abteil[i].Anhang[j]->Ort - abteil[i].Ort;
                senkrecht = Vektor(richtung.y(), -richtung.x(), 0);
                endeLinie = abteil[i].Ort + richtung * (1 - l_Pfeil/richtung.Laenge());
                pfeil_1 = abteil[i].Ort + richtung * (1 - l_Pfeil/richtung.Laenge()) + senkrecht * (b_Pfeil/senkrecht.Laenge());
                pfeil_2 = abteil[i].Ort + richtung * (1 - l_Pfeil/richtung.Laenge()) - senkrecht * (b_Pfeil/senkrecht.Laenge());

                dc.DrawLine(abteil[i].Ort.x()*zeichenFkt + abzugX, abteil[i].Ort.y()*zeichenFkt + abzugY,
                            endeLinie.x()*zeichenFkt + abzugX, endeLinie.y()*zeichenFkt + abzugY);

                dc.DrawLine(pfeil_1.x()*zeichenFkt + abzugX, pfeil_1.y() *zeichenFkt + abzugY,
                            abteil[i].Anhang[j]->Ort.x()*zeichenFkt + abzugX, abteil[i].Anhang[j]->Ort.y()*zeichenFkt + abzugY);
                dc.DrawLine(pfeil_2.x()*zeichenFkt + abzugX, pfeil_2.y() *zeichenFkt + abzugY,
                            abteil[i].Anhang[j]->Ort.x()*zeichenFkt + abzugX, abteil[i].Anhang[j]->Ort.y()*zeichenFkt + abzugY);
                dc.DrawLine(pfeil_1.x()*zeichenFkt + abzugX, pfeil_1.y() *zeichenFkt + abzugY,
                            pfeil_2.x()*zeichenFkt + abzugX, pfeil_2.y() *zeichenFkt + abzugY);

            }else{
                break;
            }
        }
    }
    /*Text*/
    dc.SetPen(wxPen(wxColour(0, 0, 0), 1));
    for(int i = 0; i<MAX_ABTEILE; i++)
    {
        dc.DrawText(wxString::Format("%d", i+1), abteil[i].Ort.x()*zeichenFkt + abzugX,
                                                    abteil[i].Ort.y()*zeichenFkt + abzugY);
    }

    SetStatusText("Speicherung des Bildes beginnt", 1);

    bool erfolg = (temp_BM.ConvertToImage()).SaveFile(saveFileDialog.GetPath(), wxBITMAP_TYPE_PNG);

	if(!erfolg)
	{
		SetStatusText(_("Bild wurde nicht gespeichert"), 1);
	}else{
		SetStatusText(_("Bild erfolgreich gespeichert"), 1);
	}

    return;
}

void GUI_NetFrame::OnPaint(wxPaintEvent &event)
{
    wxBufferedPaintDC dc(this);
    wxClientDC CL_dc(this);

    /*Verschub und Skalierung*/
    float abzugX = CL_dc.GetSize().GetWidth();
    float abzugY = CL_dc.GetSize().GetHeight();

    /*Hintergrund zeichnen*/
    wxRect rect(wxPoint(0, 0), GetClientSize());
    wxColour back = GetBackgroundColour();
    dc.SetBrush(wxBrush(back));
    dc.SetPen(wxPen(back, 1));
    dc.DrawRectangle(rect);

    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    wxFont TextFont = wxFont((int)(0.1*zeichenFkt), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    dc.SetFont(TextFont);
    wxColour TextColour = wxColour(0, 0, 0);
    dc.SetTextForeground(TextColour);
    TextColour = wxColour(0,0,0);
    dc.SetTextBackground(TextColour);

    if(mouselook)
    {
        abzugX = CL_dc.GetSize().GetWidth()/2;
        abzugY = CL_dc.GetSize().GetHeight()/2;
        int maxRunde;
        if(anaglypheLook)
        {
            maxRunde = 2;
        }else
        {
            maxRunde = 1;
        }
        for(int runde = 0; runde < maxRunde; runde++)
        {
            wxColour rot, gruen, schwarz, weiss;
            if(runde == 0)
            {
                if(maxRunde == 1)
                {
                    rot.Set(120, 80, 80);
                    gruen.Set(80, 140, 0);
                    schwarz.Set(0, 0, 0);
                    weiss.Set(255, 255, 255);
                }else{
                    dasAuge->Verschieben(-(halbesAuge), 0, 0);
                    rot.Set(120, back.Green(), back.Blue());
                    gruen.Set(80, back.Green(), back.Blue());
                    schwarz.Set(0, back.Green(), back.Blue());
                    weiss.Set(255, back.Green(), back.Blue());
                    //abzugX = halbesAuge * zeichenFkt;
                }
            }else{
                dasAuge->Verschieben((2*halbesAuge), 0, 0);
                rot.Set(back.Red(), 80 , 80);
                gruen.Set(back.Red(), 140, 0);
                schwarz.Set(back.Red(), 0, 0);
                weiss.Set(back.Red(), 255, 255);
                //abzugX -= 2 * halbesAuge * zeichenFkt;
            }

            for(int i = 0; i<MAX_ABTEILE; i++)
            {
                abteil[i].Projektion = dasAuge->Aufnahme(abteil[i].Ort);
            }
            for(int i = 0; i<MAX_ABTEILE; i++)
            {
                if(abteil[i].loesung)
                {
                    dc.SetPen(wxPen(rot, 3));
                }else
                if(i == aktiverPkt)
                {
                    dc.SetPen(wxPen(gruen, 1));
                }else
                {
                    dc.SetPen(wxPen(schwarz, 1));
                }

                if((abteil[i].Projektion.z()>0)&&(abteil[i].Projektion.z()<1))
                {
                    /*Kreise*/
                    dc.DrawCircle(abteil[i].Projektion.x()*zeichenFkt + abzugX,
                            -abteil[i].Projektion.y()*zeichenFkt + abzugY,
                            (int)(0.05*g_verbAbst*zeichenFkt*abteil[i].Projektion.z()));

                    /*Linien*/
                    float l_Pfeil = 0.25*g_verbAbst, b_Pfeil = 0.0625*g_verbAbst;
                    for(int j = 0; j<20; j++)
                    {
                        if(abteil[i].loesung && abteil[i].lsgWeg[j])
                        {
                            dc.SetPen(wxPen(rot, 3));
                        }else
                        {
                            dc.SetPen(wxPen(weiss, 1));
                        }
                        Station* aktAnhang = abteil[i].Anhang[j];
                        if(aktAnhang != NULL)
                        {
                            /*Pfeilspitze*/
                            Vektor richtung, senkrecht, pfeil_1, pfeil_2, endeLinie;

                            richtung = aktAnhang->Ort - abteil[i].Ort;
                            senkrecht = Vektor(richtung.y(), -richtung.x(), 0);
                            endeLinie = abteil[i].Ort + richtung * (1 - l_Pfeil/richtung.Laenge());
                            pfeil_1 = abteil[i].Ort + richtung * (1 - l_Pfeil/richtung.Laenge()) + senkrecht * (b_Pfeil/senkrecht.Laenge());
                            pfeil_2 = abteil[i].Ort + richtung * (1 - l_Pfeil/richtung.Laenge()) - senkrecht * (b_Pfeil/senkrecht.Laenge());

                            endeLinie = dasAuge->Aufnahme(endeLinie);
                            pfeil_1 = dasAuge->Aufnahme(pfeil_1);
                            pfeil_2 = dasAuge->Aufnahme(pfeil_2);

                            /*Pfeil*/
                            //if(((abteil[i].Projektion.z()>0)&&(abteil[i].Projektion.z()<1)) || ((aktAnhang->Projektion.z()>0)&&(aktAnhang->Projektion.z()<1)))
                            //{
                                dc.DrawLine(abteil[i].Projektion.x()*zeichenFkt + abzugX,
                                            -abteil[i].Projektion.y()*zeichenFkt + abzugY,
                                            endeLinie.x()*zeichenFkt + abzugX,
                                            -endeLinie.y()*zeichenFkt + abzugY);

                                dc.DrawLine(pfeil_1.x()*zeichenFkt + abzugX, -pfeil_1.y() *zeichenFkt + abzugY,
                                            aktAnhang->Projektion.x()*zeichenFkt + abzugX, -aktAnhang->Projektion.y()*zeichenFkt + abzugY);
                                dc.DrawLine(pfeil_2.x()*zeichenFkt + abzugX, -pfeil_2.y() *zeichenFkt + abzugY,
                                            aktAnhang->Projektion.x()*zeichenFkt + abzugX, -aktAnhang->Projektion.y()*zeichenFkt + abzugY);
                                dc.DrawLine(pfeil_1.x()*zeichenFkt + abzugX, -pfeil_1.y() *zeichenFkt + abzugY,
                                            pfeil_2.x()*zeichenFkt + abzugX, -pfeil_2.y() *zeichenFkt + abzugY);
                            //}
                        }else
                        {
                            break;
                        }
                    }
                    /*Text*/
                    dc.SetTextForeground(schwarz);
                    wxFont TextFont = wxFont((int)(0.1*g_verbAbst*zeichenFkt*abteil[i].Projektion.z()), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
                    dc.SetFont(TextFont);
                    if((abteil[i].Projektion.z()>0)&&(abteil[i].Projektion.z()<1))
                    {
                        dc.DrawText(wxString::Format("%d", i+1), abteil[i].Projektion.x()*zeichenFkt + abzugX,
                                                                -abteil[i].Projektion.y()*zeichenFkt + abzugY);
                    }
                }
            }
        }
        if(maxRunde!=1)dasAuge->Verschieben(-(halbesAuge), 0, 0);
    }
    else
    {
        abzugX = CL_dc.GetSize().GetWidth()/2;
        abzugY = CL_dc.GetSize().GetHeight()/2;
        /*Kreise*/
        for(int i = 0; i<MAX_ABTEILE; i++)
        {
            if(abteil[i].loesung)
            {
                dc.SetPen(wxPen(wxColour(180, 40, 0), 4));
            }else
            if(i == aktiverPkt)
            {
                dc.SetPen(wxPen(wxColour(20, 140, 0), 1));
            }else
            {
                dc.SetPen(wxPen(wxColour(0, 0, 0), 1));
            }
            dc.DrawCircle((abteil[i].Ort.x() + offsetX)*zeichenFkt + abzugX,
                        (-abteil[i].Ort.y() + offsetY)*zeichenFkt + abzugY,
                        (int)(0.05*zeichenFkt));
        }

        /*Linien*/

        float l_Pfeil = 0.25, b_Pfeil = 0.0625;

        //std::ofstream ausgabe("Paintlog.log", ios_base::trunc);

        for(int i = 0; i<MAX_ABTEILE; i++)
        {
            //ausgabe<<"Station Nr. "<<i<<": "<<&abteil[i]<<"\n";
            for(int j = 0; j<20; j++)
            {
                Station* aktAnhang = abteil[i].Anhang[j];
                if(aktAnhang != NULL)
                {
                    //ausgabe<<"\t"<<"Anhang "<<j<<": "<<aktAnhang<<"\n";
                    if(abteil[i].loesung && abteil[i].lsgWeg[j])
                    {
                        dc.SetPen(wxPen(wxColour(180, 40, 0), 2));
                    }else
                    {
                        dc.SetPen(wxPen(wxColour(255, 255, 255), 1));
                    }
                    /*Pfeilspitze*/
                    Vektor richtung, senkrecht, pfeil_1, pfeil_2, endeLinie;

                    richtung = aktAnhang->Ort - abteil[i].Ort;
                    senkrecht = Vektor(richtung.y(), -richtung.x(), 0);
                    endeLinie = abteil[i].Ort + richtung * (1 - l_Pfeil/richtung.Laenge());
                    pfeil_1 = abteil[i].Ort + richtung * (1 - l_Pfeil/richtung.Laenge()) + senkrecht * (b_Pfeil/senkrecht.Laenge());
                    pfeil_2 = abteil[i].Ort + richtung * (1 - l_Pfeil/richtung.Laenge()) - senkrecht * (b_Pfeil/senkrecht.Laenge());

                    dc.DrawLine((abteil[i].Ort.x() + offsetX)*zeichenFkt + abzugX, (-abteil[i].Ort.y() + offsetY)*zeichenFkt + abzugY,
                                (endeLinie.x() + offsetX)*zeichenFkt + abzugX, (-endeLinie.y() + offsetY)*zeichenFkt + abzugY);

                    dc.DrawLine((pfeil_1.x() + offsetX)*zeichenFkt + abzugX, (-pfeil_1.y() + offsetY)*zeichenFkt + abzugY,
                                (aktAnhang->Ort.x() + offsetX)*zeichenFkt + abzugX, (-aktAnhang->Ort.y() + offsetY)*zeichenFkt + abzugY);
                    dc.DrawLine((pfeil_2.x() + offsetX)*zeichenFkt + abzugX, (-pfeil_2.y() + offsetY)*zeichenFkt + abzugY,
                                (aktAnhang->Ort.x() + offsetX)*zeichenFkt + abzugX, (-aktAnhang->Ort.y() + offsetY)*zeichenFkt + abzugY);
                    dc.DrawLine((pfeil_1.x() + offsetX)*zeichenFkt + abzugX, (-pfeil_1.y() + offsetY)*zeichenFkt + abzugY,
                                (pfeil_2.x() + offsetX)*zeichenFkt + abzugX, (-pfeil_2.y() + offsetY)*zeichenFkt + abzugY);
                }else{
                    //ausgabe<<"\t"<<" Anhang "<<j<<": "<<"ist NULL\n";
                    break;
                }
            }
            //ausgabe<<"\n\n";
        }
        /*Text*/
        dc.SetPen(wxPen(wxColour(0, 0, 0), 1));
        for(int i = 0; i<MAX_ABTEILE; i++)
        {
            dc.DrawText(wxString::Format("%d", i+1), (abteil[i].Ort.x() + offsetX)*zeichenFkt + abzugX,
                        (-abteil[i].Ort.y() + offsetY)*zeichenFkt + abzugY);
        }

        /*Kamera darstellen*/
        dc.SetBrush(wxBrush(wxColor(128, 50, 200)));

        Vektor KameraPos = dasAuge->HoleOrt();
        dc.DrawCircle((KameraPos.x() + offsetX)*zeichenFkt + abzugX,
                    (-KameraPos.y() + offsetY)*zeichenFkt + abzugY,
                    (int)(0.1*zeichenFkt));

        Vektor BlickRichtung = dasAuge->HoleBlickRichtung();
        dc.DrawLine((KameraPos.x() + offsetX)*zeichenFkt + abzugX, (-KameraPos.y() + offsetY)*zeichenFkt + abzugY,
                    (KameraPos.x()+BlickRichtung.x() + offsetX)*zeichenFkt + abzugX,
                    (-KameraPos.y()-BlickRichtung.y() + offsetY)*zeichenFkt + abzugY);

        dc.DrawCircle((mPos.x() + offsetX)*zeichenFkt + abzugX,
                    (-mPos.y() + offsetY)*zeichenFkt + abzugY,
                    (int)(0.1*zeichenFkt));
    }
    return;
}

void GUI_NetFrame::OnClose(wxCloseEvent &event)
{
    timer.Stop();
    Destroy();
}

void GUI_NetFrame::OnQuit(wxCommandEvent &event)
{
    timer.Stop();
    Destroy();
}

void GUI_NetFrame::OnAbout(wxCommandEvent &event)
{
    GUI_NetOptions* Optionen = new GUI_NetOptions(this, _("Optionen"), g_intervall, g_verbAbst, g_loserAbst,
                                                  g_daempfung, g_ruettelruck, (double)(2*halbesAuge), kameraGeschw);
    Optionen->Show();
    return;
}

void GUI_NetFrame::OnKeyDown(wxKeyEvent& event)
{
    if(event.GetKeyCode()==WXK_F4)
    {
        if(timer.IsRunning())
        {
            timer.Stop();
        }else{
            timer.Start(TIMERTICK);
        }
    }
    if(event.GetKeyCode()==WXK_F12)
    {
        bool wasRunning = false;
        if(timer.IsRunning())
        {
            timer.Stop();
            wasRunning = true;
        }
        if(!mouselook)
        {
            offsetX = 0;
            offsetY = 0;
            for(int i = 0; i<MAX_ABTEILE; i++)
            {
                offsetX += abteil[i].Ort.x();
                offsetY += abteil[i].Ort.y();
            }
            offsetX/=MAX_ABTEILE;
            offsetY/=MAX_ABTEILE;
            Refresh();
        }else
        {
            logSchreiben("Liste der Abteile:\n");
            float blickX, blickY, blickZ;
            blickX = blickY = blickZ =0.0;
            for(int i = 0; i<MAX_ABTEILE; i++)
            {
                blickX += abteil[i].Ort.x();
                blickY += abteil[i].Ort.y();
                blickZ += abteil[i].Ort.z();

                //logSchreiben(abteil[i].Ort.x(), 3);logSchreiben(" | ");
                //logSchreiben(abteil[i].Ort.y(), 3);logSchreiben(" | ");
                //logSchreiben(abteil[i].Ort.z(), 3);logSchreiben("\n");
            }
            blickX/=MAX_ABTEILE;
            blickY/=MAX_ABTEILE;
            blickZ/=MAX_ABTEILE;

            //logSchreiben("Blickrichtung:\n");
            //logSchreiben(blickX, 3);logSchreiben(" | ");
            //logSchreiben(blickY, 3);logSchreiben(" | ");
            //logSchreiben(blickZ, 3);logSchreiben("\n");

            dasAuge->SetzeBlickRichtung(Vektor(blickX, blickY, blickZ));
            //logSchreiben("Kamera Standort:\n");
            //logSchreiben(dasAuge->HoleOrt().x(), 3);logSchreiben(" | ");
            //logSchreiben(dasAuge->HoleOrt().y(), 3);logSchreiben(" | ");
            //logSchreiben(dasAuge->HoleOrt().z(), 3);logSchreiben("\n");
            //logSchreiben("Kamera Normale:\n");
            //logSchreiben(dasAuge->HoleBlickRichtung().x(), 3);logSchreiben(" | ");
            //logSchreiben(dasAuge->HoleBlickRichtung().y(), 3);logSchreiben(" | ");
            //logSchreiben(dasAuge->HoleBlickRichtung().z(), 3);logSchreiben("\n");
            Refresh();
        }
        if(wasRunning)timer.Start(TIMERTICK);
    }
    if(event.GetKeyCode()=='+')zeichenFkt+=1;
    if(event.GetKeyCode()=='-')zeichenFkt-=1;
    if(zeichenFkt<1)zeichenFkt=1;
    SetStatusText(wxString::Format("Zeichenfaktor: %.1f", zeichenFkt), 2);

    if(event.GetKeyCode()==WXK_F5)AmGitterRuetteln(g_ruettelruck);
    if(event.GetKeyCode()==WXK_F6)Z_Nullen();

    if(event.GetKeyCode() == 'P')
    {
        if(pktSchiebenAktiv == true)
        {
            pktSchiebenAktiv = false;
            aktiverPkt = -1;
        }else{
            pktSchiebenAktiv = true;
        }
    }

    /*Kamerabewegungen*/
    if(event.GetKeyCode() == WXK_F7)
    {
        if(mouselook)
        {
            mouselook = false;
            SetCursor(wxCURSOR_ARROW);
        }else
        {
            mouselook = true;
            SetCursor(wxCURSOR_BLANK);
        }
    }

    if(event.GetKeyCode() == WXK_F8)
    {
        if(anaglypheLook)
        {
            anaglypheLook = false;
        }else
        {
            anaglypheLook = true;
        }
    }

    if(event.GetKeyCode() == 'W')KameraVor = true;
    if(event.GetKeyCode() == 'S')KameraZurueck = true;
    if(event.GetKeyCode() == 'D')KameraRechts = true;
    if(event.GetKeyCode() == 'A')KameraLinks = true;
    if(event.GetKeyCode() == WXK_SPACE)KameraHoch = true;
    if(event.GetKeyCode() == WXK_CONTROL)KameraRunter = true;

    int Fkt = 1;
    if(event.ShiftDown())Fkt = 5;
    dasAuge->Verschieben(kameraGeschw*Fkt*(KameraRechts-KameraLinks), kameraGeschw*Fkt*(KameraVor-KameraZurueck), kameraGeschw*Fkt*(KameraHoch-KameraRunter));

    Refresh();
    return;
}

void GUI_NetFrame::OnKeyUp(wxKeyEvent& event)
{
    if(event.GetKeyCode() == 'W')KameraVor = false;
    if(event.GetKeyCode() == 'S')KameraZurueck = false;
    if(event.GetKeyCode() == 'D')KameraRechts = false;
    if(event.GetKeyCode() == 'A')KameraLinks = false;
    if(event.GetKeyCode() == WXK_SPACE)KameraHoch = false;
    if(event.GetKeyCode() == WXK_CONTROL)KameraRunter = false;

    Refresh();
    return;
}

void GUI_NetFrame::Z_Nullen(void)
{
    for(int i=0; i<MAX_ABTEILE; i++)
    {
        abteil[i].Ort.SetKoordinaten(2, 0);
        abteil[i].Zuwachs.SetKoordinaten(2, 0);
    }
    return;
}

void GUI_NetFrame::OnMouseClick(wxMouseEvent& event)
{
    wxClientDC dc(this);
    AlteMousePosition = event.GetLogicalPosition(dc);
    SetStatusText(wxString::Format("MousePosition: %d / %d", AlteMousePosition.x, AlteMousePosition.y), 2);
    if(event.RightDown())
    {
        SetStatusText(wxString::Format("B/H: %d / %d", dc.GetSize().GetWidth(), dc.GetSize().GetHeight()));
        /*Verschub und Skalierung*/
        float abzugX = dc.GetSize().GetWidth()/2;
        float abzugY = dc.GetSize().GetHeight()/2;

        /*Vektor*/ mPos = Vektor(AlteMousePosition.x-abzugX-offsetX*zeichenFkt, -(AlteMousePosition.y-abzugY-offsetY*zeichenFkt), 0);
        mPos /= zeichenFkt;
        aktiverPkt = FindeNaechstesAbteil(mPos);
        Refresh();
    }
    return;
}

void GUI_NetFrame::OnMouseMove(wxMouseEvent& event)
{
    if(event.Dragging() && event.LeftIsDown())
    {
        wxClientDC dc(this);
        wxPoint NeueMousePosition = event.GetLogicalPosition(dc);

        offsetX += ((NeueMousePosition.x-AlteMousePosition.x)/zeichenFkt);
        offsetY += ((NeueMousePosition.y-AlteMousePosition.y)/zeichenFkt);

        AlteMousePosition = NeueMousePosition;
        if(!(timer.IsRunning()))Refresh();
    }
    if(event.Dragging() && event.RightIsDown())
    {
        wxClientDC dc(this);
        wxPoint NeueMousePosition = event.GetLogicalPosition(dc);

        Vektor verschub = Vektor((NeueMousePosition.x-AlteMousePosition.x), -(NeueMousePosition.y-AlteMousePosition.y), 0);
        verschub /= zeichenFkt;
        abteil[aktiverPkt].Ort += verschub;

        AlteMousePosition = NeueMousePosition;
        if(!(timer.IsRunning()))Refresh();
    }
    if(mouselook)
    {
        wxClientDC dc(this);
        wxPoint NeueMousePosition = event.GetLogicalPosition(dc);
        dasAuge->Drehen(-(-NeueMousePosition.y+AlteMousePosition.y)*0.0005, (-NeueMousePosition.x+AlteMousePosition.x)*0.0005);
        WarpPointer(AlteMousePosition.x, AlteMousePosition.y);
        if(!(timer.IsRunning()))Refresh();
    }
    event.Skip();
    return;
}

void GUI_NetFrame::OnMouseWheel(wxMouseEvent& event)
{
    if(mouselook)
    {
        if(event.GetWheelRotation() < 0)
        {
            dasAuge->InkrAbstand(-1);
            if(dasAuge->HoleAbstand() < 1)dasAuge->SetzeAbstand(1.0);
        }else{
            dasAuge->InkrAbstand(1.0);
        }
        SetStatusText(wxString::Format("Mattscheibeabstand: %.1f", dasAuge->HoleAbstand()), 2);
    }
    event.Skip();
    return;
}

void GUI_NetFrame::OnTimer(wxTimerEvent& event)
{
    float ergebnis = EineRundeRechnen();
    if(ergebnis<0.001)timer.Stop();
    if(!(GetClientSize().GetWidth() <= 0) && !(GetClientSize().GetHeight() <= 0))
    {
        SetStatusText("Anderungsrate: " + wxString::Format("%.5f", ergebnis), 1);
        Refresh();
    }
    return;
}

void GUI_NetFrame::OnEraseBackground(wxEraseEvent& event)
{
    return;
}

float GUI_NetFrame::EineRundeRechnen(void)
{
    for(int i=0; i<MAX_ABTEILE-1; i++)
    {
        for(int j=i+1; j<MAX_ABTEILE; j++)
        {
            if(i!=j)
            {
                EntfernungsZuwachs(i, j, abteil, g_intervall, g_verbAbst, g_loserAbst);
            }
        }
    }
    float aenderungsRate=0;
    for(int i=0; i<MAX_ABTEILE; i++)
    {
        float tempFloat = EntfernungZurechnen(i, abteil, g_daempfung);
        if(tempFloat>aenderungsRate)
        {
            aenderungsRate = tempFloat;
        }
    }
    return aenderungsRate;
}

void GUI_NetFrame::AmGitterRuetteln(float maximum)
{
    float ruck_1, ruck_2, ruck_3;
    for(int i=0; i<MAX_ABTEILE; i++)
    {
        ruck_1 = (RAND_MAX - 2*rand())*maximum/RAND_MAX;
        ruck_2 = (RAND_MAX - 2*rand())*maximum/RAND_MAX;
        ruck_3 = (RAND_MAX - 2*rand())*maximum/RAND_MAX;
        abteil[i].Zuwachs = abteil[i].Zuwachs + Vektor(ruck_1, ruck_2, ruck_3);
    }
    return;
}

int GUI_NetFrame::FindeNaechstesAbteil(Vektor& tPos)
{
    float abstand = (abteil[0].Ort - tPos).ProjLaenge(2);
    int ergebnis = 0;

    for(int i=1; i<MAX_ABTEILE; i++)
    {

        if(abstand > (abteil[i].Ort - tPos).ProjLaenge(2))
        {
            abstand = (abteil[i].Ort - tPos).ProjLaenge(2);
            ergebnis = i;
        }
    }
    return ergebnis;
}

void GUI_NetFrame::SetzeAugenAbstand(double wert)
{
    halbesAuge=(float)wert;
    if(!(timer.IsRunning()))Refresh();
    return;
}

void GUI_NetFrame::SetzeKameraGeschw(double wert)
{
    kameraGeschw=wert;
    return;
}

void GUI_NetFrame::logSchreiben(const char*msg)
{
    ofstream Logbuch;
    Logbuch.open("Debug.log",ios_base::out|ios_base::app);
    if(Logbuch.good())
    {
        Logbuch<<msg;
        Logbuch.close();
    }
    return;
}

void GUI_NetFrame::logSchreiben(const float msg, int i)
{
    ofstream Logbuch;
    Logbuch.open("Debug.log",ios_base::out|ios_base::app);
    if(Logbuch.good())
    {
        Logbuch.setf( ios::fixed, ios::floatfield );
        Logbuch.precision(i);
        Logbuch<<msg;
        Logbuch.close();
    }
    return;
}

void GUI_NetFrame::logSchreiben(const void *msg)
{
    ofstream Logbuch;
    Logbuch.open("Debug.log",ios_base::out|ios_base::app);
    if(Logbuch.good())
    {
        Logbuch<<msg;
        Logbuch.close();
    }
    return;
}


/*GUI_NetOptions*/
BEGIN_EVENT_TABLE(GUI_NetOptions, wxFrame)
    EVT_CLOSE(GUI_NetOptions::OnClose)
    EVT_MENU(idOptionQuit, GUI_NetOptions::OnQuit)
    EVT_TEXT_ENTER(idIntervall, GUI_NetOptions::OnParameterEingabe)
    EVT_TEXT_ENTER(idVerbAbst, GUI_NetOptions::OnParameterEingabe)
    EVT_TEXT_ENTER(idLoserAbst, GUI_NetOptions::OnParameterEingabe)
    EVT_TEXT_ENTER(idDaempfung, GUI_NetOptions::OnParameterEingabe)
    EVT_TEXT_ENTER(idRuettelruck, GUI_NetOptions::OnParameterEingabe)
    EVT_TEXT_ENTER(idAuge, GUI_NetOptions::OnParameterEingabe)
    EVT_TEXT_ENTER(idKameraGeschw, GUI_NetOptions::OnParameterEingabe)
END_EVENT_TABLE()

GUI_NetOptions::GUI_NetOptions(GUI_NetFrame *frame, const wxString& title, double intervall, double verbAbst,
                               double loserAbst, double daempfung, double ruettelruck, double Auge, double kameraGeschw)
    : wxFrame(frame, -1, title, wxDefaultPosition, wxDefaultSize)
{
    Mama = frame;

    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idOptionQuit, _("&Quit\tAlt-F4"), _("Optionen schließen"));
    mbar->Append(fileMenu, _("&Optionen"));

    SetMenuBar(mbar);

    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(_("Berechnungsoptionen einstellen"),0);
    SetStatusText(_("wird nach schließen angewendet"), 1);

    /*Eingabefelder vorbereiten*/
	wxColor TxtColor =  wxColor(200,200,200);
	wxColor backgrc = GetBackgroundColour();
	char Zeile = 0;
	/*Intervall*/
        wxTextCtrl* NAME_Intervall = new wxTextCtrl(this, -1, _("Annäherungsrate"), wxPoint(55, (Zeile * 20 + 10)),
                                            wxSize(180, 15), wxTE_READONLY|wxTE_LEFT|wxNO_BORDER,
                                            wxDefaultValidator, _("NAME_Intervall"));
        NAME_Intervall->SetBackgroundColour(backgrc);
        m_intervall = new aruDblTxtCtrl(this, idIntervall, wxString::Format("%.3f", intervall), wxPoint(5, (Zeile++ * 20 + 5)),
                                            wxSize(45, 20), wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL,
                                            wxDefaultValidator, _("TXT_intervall"));
        m_intervall->SetBackgroundColour(TxtColor);

    /*Verbundener Abstand*/
        wxTextCtrl* NAME_VerbAbst = new wxTextCtrl(this, -1, _("verbundener Abstand"), wxPoint(55, (Zeile * 20 + 10)),
                                            wxSize(180, 15), wxTE_READONLY|wxTE_LEFT|wxNO_BORDER,
                                            wxDefaultValidator, _("NAME_VerbAbst"));
        NAME_VerbAbst->SetBackgroundColour(backgrc);
        m_verbAbst = new aruDblTxtCtrl(this, idVerbAbst, wxString::Format("%.3f", verbAbst), wxPoint(5, (Zeile++ * 20 + 5)),
                                            wxSize(45, 20), wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL,
                                            wxDefaultValidator, _("TXT_VerbAbst"));
        m_verbAbst->SetBackgroundColour(TxtColor);

    /*loser Abstand*/
        wxTextCtrl* NAME_loserAbst = new wxTextCtrl(this, -1, _("loser Abstand"), wxPoint(55, (Zeile * 20 + 10)),
                                            wxSize(180, 15), wxTE_READONLY|wxTE_LEFT|wxNO_BORDER,
                                            wxDefaultValidator, _("NAME_loserAbst"));
        NAME_loserAbst->SetBackgroundColour(backgrc);
        m_loserAbst = new aruDblTxtCtrl(this, idLoserAbst, wxString::Format("%.3f", loserAbst), wxPoint(5, (Zeile++ * 20 + 5)),
                                            wxSize(45, 20), wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL,
                                            wxDefaultValidator, _("TXT_loserAbst"));
        m_loserAbst->SetBackgroundColour(TxtColor);

    /*daempfung*/
        wxTextCtrl* NAME_daempfung = new wxTextCtrl(this, -1, _("Dämpfung"), wxPoint(55, (Zeile * 20 + 10)),
                                            wxSize(180, 15), wxTE_READONLY|wxTE_LEFT|wxNO_BORDER,
                                            wxDefaultValidator, _("NAME_daempfung"));
        NAME_daempfung->SetBackgroundColour(backgrc);
        m_daempfung = new aruDblTxtCtrl(this, idDaempfung, wxString::Format("%.3f", daempfung), wxPoint(5, (Zeile++ * 20 + 5)),
                                            wxSize(45, 20), wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL,
                                            wxDefaultValidator, _("TXT_daempfung"));
        m_daempfung->SetBackgroundColour(TxtColor);

    /*ruettelruck*/
        wxTextCtrl* NAME_ruettelruck = new wxTextCtrl(this, -1, _("Rüttelruck"), wxPoint(55, (Zeile * 20 + 10)),
                                            wxSize(180, 15), wxTE_READONLY|wxTE_LEFT|wxNO_BORDER,
                                            wxDefaultValidator, _("NAME_ruettelruck"));
        NAME_ruettelruck->SetBackgroundColour(backgrc);
        m_ruettelruck = new aruDblTxtCtrl(this, idRuettelruck, wxString::Format("%.3f", ruettelruck), wxPoint(5, (Zeile++ * 20 + 5)),
                                            wxSize(45, 20), wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL,
                                            wxDefaultValidator, _("TXT_ruettelruck"));
        m_ruettelruck->SetBackgroundColour(TxtColor);

    /*Augenabstand*/
        wxTextCtrl* NAME_Auge = new wxTextCtrl(this, -1, _("Augenabstand"), wxPoint(55, (Zeile * 20 + 10)),
                                            wxSize(180, 15), wxTE_READONLY|wxTE_LEFT|wxNO_BORDER,
                                            wxDefaultValidator, _("NAME_Auge"));
        NAME_Auge->SetBackgroundColour(backgrc);
        m_Auge = new aruDblTxtCtrl(this, idAuge, wxString::Format("%.3f", Auge), wxPoint(5, (Zeile++ * 20 + 5)),
                                            wxSize(45, 20), wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL,
                                            wxDefaultValidator, _("TXT_Auge"));
        m_Auge->SetBackgroundColour(TxtColor);

    /*Kamerageschwindigkeit*/
        wxTextCtrl* NAME_KameraGeschw = new wxTextCtrl(this, -1, _("Kamera Geschwindigkeit"), wxPoint(55, (Zeile * 20 + 10)),
                                            wxSize(180, 15), wxTE_READONLY|wxTE_LEFT|wxNO_BORDER,
                                            wxDefaultValidator, _("NAME_KameraGeschw"));
        NAME_KameraGeschw->SetBackgroundColour(backgrc);
        m_KameraGeschw = new aruDblTxtCtrl(this, idKameraGeschw, wxString::Format("%.3f", kameraGeschw), wxPoint(5, (Zeile++ * 20 + 5)),
                                            wxSize(45, 20), wxTE_PROCESS_ENTER|wxTE_RIGHT|wxBORDER_SIMPLE|wxTE_NOHIDESEL,
                                            wxDefaultValidator, _("TXT_Auge"));
        m_KameraGeschw->SetBackgroundColour(TxtColor);

        m_intervall->SetNachfolger(m_verbAbst);
        m_intervall->SetVorgaenger(m_Auge);
        m_verbAbst->SetNachfolger(m_loserAbst);
        m_verbAbst->SetVorgaenger(m_intervall);
        m_loserAbst->SetNachfolger(m_daempfung);
        m_loserAbst->SetVorgaenger(m_verbAbst);
        m_daempfung->SetNachfolger(m_ruettelruck);
        m_daempfung->SetVorgaenger(m_loserAbst);
        m_ruettelruck->SetNachfolger(m_Auge);
        m_ruettelruck->SetVorgaenger(m_daempfung);
        m_Auge->SetNachfolger(m_KameraGeschw);
        m_Auge->SetVorgaenger(m_ruettelruck);
        m_KameraGeschw->SetNachfolger(m_intervall);
        m_KameraGeschw->SetVorgaenger(m_Auge);
}

GUI_NetOptions::~GUI_NetOptions()
{

}

void GUI_NetOptions::OnClose(wxCloseEvent &event)
{
    ParentUpdaten();
    Destroy();
}

void GUI_NetOptions::OnQuit(wxCommandEvent &event)
{
    ParentUpdaten();
    Destroy();
}

void GUI_NetOptions::OnParameterEingabe(wxCommandEvent& event)
{
    ParentUpdaten();
    return;
}

void GUI_NetOptions::ParentUpdaten()
{
    Mama->SetzeDaempfung(m_daempfung->HoleWert());
    Mama->SetzeIntevall(m_intervall->HoleWert());
    Mama->SetzeLoserAbst(m_loserAbst->HoleWert());
    Mama->SetzeRuettelRuck(m_ruettelruck->HoleWert());
    Mama->SetzeVerbAbst(m_verbAbst->HoleWert());
    Mama->SetzeAugenAbstand(m_Auge->HoleWert()/2);
    Mama->SetzeKameraGeschw(m_KameraGeschw->HoleWert());
    return;
}
