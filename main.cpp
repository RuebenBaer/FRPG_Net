#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include "Vektor\Vektor.h"

using namespace std;

struct Station
{
    int Nr;
    Vektor Ort, Zuwachs;
    int Anhang[20];
};

float Entfernung(Station a, Station b)
{
    return((a.Ort-b.Ort).Laenge());
}

bool HatAnhang(int a, Station b)
{
    for(int i=0; i<20; i++)
    {
        if(b.Anhang[i] == a)return true;
        if(b.Anhang[i] < 0)return false;
    }
    return false;
}

void EntfernungsZuwachs(int a_Station, int b_Station, Station* netz)
{
    float distanz = Entfernung(netz[a_Station], netz[b_Station]);
    float delta;
    //cout<<a_Station<<" -> "<<b_Station<<": "<<distanz<<endl;
    Vektor a = netz[a_Station].Ort;
    Vektor b = netz[b_Station].Ort;
    Vektor c;

    if(distanz == 0)
    {
        c = Vektor(0.5, 0.5, 0);
        netz[a_Station].Zuwachs += c;
        return;
    }else{
        delta = (distanz-1)*0.01/(2*distanz);/*(distanz-1)*0.5/(2*distanz);*/
    }

    if(HatAnhang(b_Station, netz[a_Station]))
    {
        c = (b-a);
        c *= delta;
        netz[a_Station].Zuwachs += c;
        netz[b_Station].Zuwachs -=c;
    }else{
        if(distanz<1.0)
        {
            c = (b-a);
            c *= delta;
            netz[a_Station].Zuwachs += c;
            netz[b_Station].Zuwachs -=c;
        }
    }
    return;
}

bool EntfernungZurechnen(int nr_Station, Station* netz)
{
    if(netz[nr_Station].Zuwachs.Laenge()>0.01)
    {
        netz[nr_Station].Ort += netz[nr_Station].Zuwachs;
        netz[nr_Station].Zuwachs *= 0.8;
        return 1;
    }
    return 0;
}

bool StrukturEinlesen(char *dateiName, Station* netz)
{
    ifstream datei;
    char zeile[128];
    int zahl, stationsNummer;
    bool anhaengen;

    datei.open(dateiName);
    if(datei.good())
    {
        while(!datei.eof())
        {
            zahl = 0;
            anhaengen = false;

            datei.get(zeile, 128, '\n');
            for(int i=0; zeile[i]!='\0'; i++)
            {
                if((zeile[i]>47)&&(zeile[i]<58))
                {
                    zahl = zahl*10 + zeile[i]-48;
                }
                if((zeile[i]<'0')||(zeile[i]>'9')||(i == (int)strlen(zeile)-1))
                {
                    if(anhaengen == false)
                    {
                        stationsNummer = zahl-1;
                        //printf("Station: %d\nAnhang: ", zahl);
                    }else{
                        for(int laeufer = 0; laeufer<20; laeufer++)
                        {
                            if((netz[stationsNummer].Anhang[laeufer] == -1)||(netz[stationsNummer].Anhang[laeufer] == zahl-1))
                            {
                                printf("Eintrag: %d \n", zahl);
                                netz[stationsNummer].Anhang[laeufer] = zahl-1;
                                break;
                            }else{
                                printf("belegt: %d \n", laeufer);
                            }
                        }
                        //printf(" %d ", zahl);
                    }
                    zahl = 0;
                    anhaengen = true;
                }
            }
            //printf("\n\n");
            while(datei.peek()=='\n')datei.ignore();
        }
    }
    datei.close();
    return 1;
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
                if(netz[i].Anhang[vG] > -1)
                {
                    int nach = netz[i].Anhang[vG];
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

int main(int argc, char *argv[])
{
    Station abteil[400];
    for(int i = 0; i<400; i++)
    {
        for(int j = 0; j<20; j++)
        {
            abteil[i].Anhang[j] = -1;
        }
    }
    if(argc>0)
    {
        StrukturEinlesen(argv[1], abteil);
    }
    for(int i = 0; i<400; i++)
    {
        printf("Station: %d\nAnhang: ", i+1);
        for(int j = 0; j<20; j++)
        {
            if(abteil[i].Anhang[j] < 0)break;
            printf(" %d ", abteil[i].Anhang[j]+1);
        }
        printf("\n\n");
    }

    for(int i=0; i<20; i++)
    {
        for(int j=0; j<20; j++)
        {
            abteil[i+20*j].Ort = Vektor(i, j, 0);
            abteil[i+20*j].Zuwachs = Vektor(0, 0, 0);
        }
    }

    int ende, grenze;
    grenze = 100;
    do
    {
        ende = 0;
        for(int i=0; i<400; i++)
        {
            for(int j=0; j<400; j++)
            {
                if(i!=j)
                {
                    EntfernungsZuwachs(i, j, abteil);
                }
            }
        }
        for(int i=0; i<400; i++)
        {
            if(EntfernungZurechnen(i, abteil)) ende++;
        }
        if(ende<=grenze)
        {
            grenze-=10;
            cout<<"."<<flush;
            //printf("%d Aenderungen\n", ende);
            OrteAuslesen(abteil);
        }
    }while(ende > 0);

    cout<<"\nFertig\n";
    system("PAUSE");
    return 0;
}
