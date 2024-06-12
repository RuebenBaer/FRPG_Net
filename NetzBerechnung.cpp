#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <cmath>
#include "NetzBerechnung.h"
#include "Vektor\Vektor.h"

using namespace std;

float Entfernung(Station a, Station b)
{
    return((a.Ort-b.Ort).Laenge());
}

bool HatAnhang(Station& a, Station& b)
{
    for(int i=0; i<20; i++)
    {
        if(b.Anhang[i] == &a)
        {
            return true;
        }
        if(b.Anhang[i] == NULL)
        {
            return false;
        }
    }
    return false;
}

void EntfernungsZuwachs(int a_Station, int b_Station, Station* netz, double intervall, double verAbst, double loserAbst)
{

    float distanz = Entfernung(netz[a_Station], netz[b_Station]);
    float delta, gamma;
    //cout<<a_Station<<" -> "<<b_Station<<": "<<distanz<<endl;
    Vektor a = netz[a_Station].Ort;
    Vektor b = netz[b_Station].Ort;
    Vektor c;

    if(distanz == 0)
    {
        c = Vektor(0.01, 0.01, 0);
        netz[a_Station].Zuwachs += c;
        netz[b_Station].Zuwachs -= c;
        return;
    }else{
        delta = 2*(distanz-verAbst)*intervall/((distanz));
        //delta = (distanz-verAbst)*(distanz-verAbst)*intervall/(2*distanz);
        //delta *= (1 - 2 * (distanz<verAbst));
        gamma = (distanz-loserAbst)*intervall/(distanz);
        //gamma *= (1-2*(distanz<loserAbst));
    }

    if(HatAnhang(netz[b_Station], netz[a_Station]))
    {
        c = (b-a);
        c *= delta;
        netz[a_Station].Zuwachs += c;
        netz[b_Station].Zuwachs -=c;
    }else{
        if(distanz<loserAbst)
        {
            c = (b-a);
            c *= gamma;
            netz[a_Station].Zuwachs += c;
            netz[b_Station].Zuwachs -=c;
        }
    }
	
    return;
}

float EntfernungZurechnen(int nr_Station, Station* netz, double daempfung)
{
    float delta = netz[nr_Station].Zuwachs.Laenge();

    netz[nr_Station].Zuwachs *= (1-daempfung);
    netz[nr_Station].Ort += netz[nr_Station].Zuwachs;

    return delta;
}

int StrukturGroesse(const char *dateiName)
{
    ifstream datei;
    char zeile[128];
    int zahl, gesZahl;

    gesZahl=0;

    datei.open(dateiName);
    if(datei.good())
    {
        while(!datei.eof())
        {
            zahl = 0;

            datei.get(zeile, 128, '\n');
            for(int i=0; zeile[i]!='\0'; i++)
            {
                if((zeile[i]>47)&&(zeile[i]<58))
                {
                    zahl = zahl*10 + zeile[i]-48;
                }
                if(zeile[i]=='@')break;
                if((zeile[i]<'0')||(zeile[i]>'9')||(i == (int)strlen(zeile)-1))
                {
                    if(zahl > gesZahl)gesZahl = zahl;
                    zahl = 0;
                }
            }
            while(datei.peek()=='\n')datei.ignore();
        }
    }
    datei.close();
    return gesZahl;

}

bool StrukturEinlesen(const char *dateiName, Station* netz, int netzGroesse)
{
    ifstream datei;
    ofstream ausgabe("EinleseLog.log");
    char zeile[1024];
    int zahl, stationsNummer;
    bool anhaengen, hatOrte;

    hatOrte = 0;

    datei.open(dateiName);
    if(datei.good())
    {
        while(!datei.eof())
        {
            zahl = stationsNummer = 0;
            anhaengen = false;

            datei.get(zeile, 1023, '\n');
            for(int i=0; zeile[i]!='\0'; i++)
            {
                if((zeile[i]>47)&&(zeile[i]<58))
                {
                    zahl = zahl*10 + zeile[i]-48;
                }
                if((zeile[i]<'0')||(zeile[i]>'9')||(i == (int)strlen(zeile)-1))
                {
                    if(zahl-1 > netzGroesse)break;
                    if(zeile[i]=='@')
                    {
                        ausgabe<<"Station "<<stationsNummer<<" hat Position\n";
                        OrteEinlesen(zeile, i++, netz, stationsNummer, ausgabe);
                        hatOrte = 1;
                        break;
                    }
                    if(zahl < 1)continue;
                    if(anhaengen == false)
                    {
                        stationsNummer = zahl-1;
                        if(zeile[i]=='*')netz[stationsNummer].loesung = true;
                        ausgabe<<"Station: "<<zahl<<" : "<<&netz[zahl-1]<<"\nAnhang: \n";
                    }else{
                        for(int laeufer = 0; laeufer<20; laeufer++)
                        {
                            if((netz[stationsNummer].Anhang[laeufer] == NULL)||(netz[stationsNummer].Anhang[laeufer] == &netz[zahl-1]))
                            {
                                ausgabe<<"Eintrag: "<<zahl<<" : "<<&netz[zahl-1]<<"\n";
                                netz[stationsNummer].Anhang[laeufer] = &netz[zahl-1];
                                if(zeile[i]=='*')netz[stationsNummer].lsgWeg[laeufer] = true;
                                break;
                            }else{
                                //ausgabe<<"belegt: "<<laeufer<<"\n";
                            }
                        }
                    }
                    zahl = 0;
                    anhaengen = true;
                }
            }
            ausgabe<<"\n\n";
            while(datei.peek()=='\n')datei.ignore();
        }
    }
    datei.close();
    return hatOrte;
}

void OrteEinlesen(const char* zeile, int start, Station* netz, int stationsNummer, ofstream &ausgabe)
{
    bool nachKomma, zahlGefunden;
    int xyzLesen;
    int i = start;
    int negativ = 1;
    double div = 0.1;

    xyzLesen = 0;
    nachKomma = zahlGefunden = false;

    double zahl = 0;
    for(; i<1024; i++)
    {
        if((zeile[i]>47)&&(zeile[i]<58))
        {
            zahlGefunden = true;
            if(nachKomma)
            {
                zahl = zahl + (zeile[i]-48)*div;
                div *= 0.1;
            }else{
                zahl = zahl*10 + zeile[i]-48;
            }
        }

        if(zeile[i]=='-')
        {
            negativ = -1;
        }else
        if(zeile[i]==',')
        {
            nachKomma = true;
        }else
        if(((zeile[i]<'0')||(zeile[i]>'9'))&&zahlGefunden)
        {
            zahlGefunden = false;
            nachKomma = false;
            zahl *= negativ;
            switch(xyzLesen)
            {
            case 0:
                netz[stationsNummer].Ort.SetKoordinaten(0, zahl);
                ausgabe<<stationsNummer<<" x: "<<zahl<<"\n";
                zahl = 0;
                div = 0.1;
                negativ = 1;
                xyzLesen = 1;
                break;
            case 1:
                netz[stationsNummer].Ort.SetKoordinaten(1, zahl);
                ausgabe<<stationsNummer<<" y: "<<zahl<<"\n";
                zahl = 0;
                div = 0.1;
                negativ = 1;
                xyzLesen = 2;
                break;
            default:
                netz[stationsNummer].Ort.SetKoordinaten(2, zahl);
                ausgabe<<stationsNummer<<" z: "<<zahl<<"\n";
                return;
            }
        }
    }
    return;
}

void OrteAuslesen(Station* netz)
{
    ofstream datei;
    datei.open("Ausgabe.txt", ios_base::trunc);
    if(datei.good())
    {
        datei.precision(5);
        for(int i=0; i<400; i++)
        {
            datei<<netz[i].Ort.x()<<"\t"<<netz[i].Ort.y()<<"\t: "<<i<<"\n";
            datei<<netz[i].Ort.x()<<"\t"<<netz[i].Ort.y()<<"\t: "<<i<<"\n\n";

            for(int vG=0; vG<20; vG++)
            {
                if(netz[i].Anhang[vG] != NULL)
                {
                    int nach = netz[i].Anhang[vG]->Nr;
                    datei<<netz[i].Ort.x()<<"\t"<<netz[i].Ort.y()<<"\t: "<<i<<"\n";
                    datei<<netz[nach].Ort.x()<<"\t"<<netz[nach].Ort.y()<<"\t: "<<nach<<"\n\n";
                }else{
                    break;
                }
            }
        }
    }
    datei.close();
    return;
}
