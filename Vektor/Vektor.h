/*  RUZmBI - "Rechnerunterstütztes Zeichnungsprogramm mit Benutzerinterface"
    Hauptzweck ist Einlesen von 3D-Punktwolken und Linien (werden als Bruchkanten interpretiert) aus dxf-Dateien, Vernetzen der Punkte und Erzeugung von Höhenlinien.
    Verwaltet mehrere Teilzeichnungen ("Layer"), erlaubt das Erzeugen von Punkten, Linien, Flächen und speichern als dxf-Datei.
    Copyright (C) 2016-2019  Ansgar Rütten

    Modul Dreidimensionaler Vektor mit Rechenoperationen für Vektoren

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

#ifndef __Vektor_
#define __Vektor_

bool IstGleich(double, double);

class Vektor{
  private:
    double* Koordinate;
  public:
    /*Konstruktoren*/
    Vektor();
    Vektor(const Vektor&);
    Vektor(double, double, double);
    Vektor(double*);
    /*Destruktor*/
    ~Vektor();
    /*Koordinaten fuellen*/
    void SetKoordinaten(double, double, double);
    void SetKoordinaten(double*);
    void SetKoordinaten(int, double);/*Index, Wert*/
    /*Koordinaten auslesen*/
    double x(void)const{return(Koordinate[0]);};
    double y(void)const{return(Koordinate[1]);};
    double z(void)const{return(Koordinate[2]);};
    double GetKoordinaten(int i)const{return(Koordinate[i%3]);};
    /*Vektorrechnungen*/
    const Vektor operator+(const Vektor&) const;
    Vektor& operator+=(const Vektor&);
    const Vektor operator-(const Vektor&) const;
    Vektor& operator-=(const Vektor&);
    const Vektor operator*(double) const;
    Vektor& operator*=(double);
    Vektor operator/(double);
    Vektor& operator/=(double);
    double operator*(const Vektor&);
    double operator*=(const Vektor&);
    Vektor& operator=(const Vektor&);
    bool operator==(const Vektor&) const;
    Vektor Kreuz(const Vektor&);
    double Laenge(void) const;
    double ProjLaenge(int) const;
    Vektor drehen(double alpha, double beta=0, double gamma=0);
  };

#endif
