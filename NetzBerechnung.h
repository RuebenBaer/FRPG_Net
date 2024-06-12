#ifndef __NETZBERECHNUNG_H
#define __NETZBERECHNUNG_H

#include <iostream>
#include <fstream>
#include "Vektor/Vektor.h"

struct Station
{
    int Nr;
    Vektor Ort, Zuwachs, Projektion;
    Station* Anhang[20];
    bool gesetzt;
    bool loesung;
    bool lsgWeg[20];
};
float Entfernung(Station a, Station b);
bool HatAnhang(int a, Station b);
void EntfernungsZuwachs(int a_Station, int b_Station, Station* netz, double intervall, double verAbst, double loserAbst);
float EntfernungZurechnen(int nr_Station, Station* , double daempfung);
int StrukturGroesse(const char *dateiName);
bool StrukturEinlesen(const char *dateiName, Station* netz, int netzGroesse);
void OrteEinlesen(const char* zeile, int start, Station* netz, int stationsNummer, std::ofstream &ausgabe);
void OrteAuslesen(Station* netz);

#endif // __NETZBERECHNUNG_H
