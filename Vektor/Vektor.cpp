#include "Vektor.h"
#include <cmath>

bool IstGleich(double a, double b)
{
    double grenzwert = 1e-10;
    if(std::abs(a) < grenzwert)
    {
        if(std::abs(b) < grenzwert)return true;
    }
    if(std::abs((b/a)-1) < grenzwert)return true;
    return false;
}

Vektor::Vektor(void)
{
    Koordinate = new double[3];
    Koordinate[0]=0;
    Koordinate[1]=0;
    Koordinate[2]=0;
}

Vektor::Vektor(double x, double y, double z)
{
    Koordinate = new double[3];
    Koordinate[0]=x;
    Koordinate[1]=y;
    Koordinate[2]=z;
}

Vektor::Vektor(double* xyz)
{
    Koordinate = new double[3];
    for (int i = 0; i < 3; i++)
    {
        Koordinate[i] = xyz[i];
    }
}

Vektor::Vektor(const Vektor& v)
{
    Koordinate = new double[3];
    for (int i = 0; i < 3; i++)
    {
        Koordinate[i] = v.GetKoordinaten(i);
    }
}

Vektor::~Vektor(void)
{
    delete[]Koordinate;
}

Vektor& Vektor::operator=(const Vektor& v)
{
    if(this==&v)return *this;
    delete []Koordinate;
    Koordinate = new double[3];
    for (int i = 0; i < 3; i++)
    {
        Koordinate[i] = v.GetKoordinaten(i);
    }
    return *this;
}

bool Vektor::operator==(const Vektor& v) const
{
    if(IstGleich(Koordinate[0], v.GetKoordinaten(0)))
    {
        if(IstGleich(Koordinate[1], v.GetKoordinaten(1)))
        {
            if(IstGleich(Koordinate[2], v.GetKoordinaten(2)))
            {
                return true;
            }
        }
    }
    return false;
}

void Vektor::SetKoordinaten(double x, double y, double z)
{
     Koordinate[0]=x;
     Koordinate[1]=y;
     Koordinate[2]=z;
     return;
}

void Vektor::SetKoordinaten(double* xyz)
{
     for(int i=0;i<3;i++)
     {
         Koordinate[i]=xyz[i];
     }
     return;
}

void Vektor::SetKoordinaten(int i, double tF)
{
     Koordinate[i]=tF;
     return;
}

const Vektor Vektor::operator+(const Vektor& AddVektor) const
{
    Vektor Rueckgabe = *this;
    Rueckgabe+=AddVektor;
    return(Rueckgabe);
}

Vektor& Vektor::operator+=(const Vektor& AddVektor)
{
    if(this==&AddVektor)return *this;
    for(int i = 0; i < 3; i++)
    {
        Koordinate[i] += AddVektor.GetKoordinaten(i);
    }
    return *this;
}

const Vektor Vektor::operator*(double Mult)const
{
    Vektor Rueckgabe = *this;
    for(int i = 0; i < 3; i++)
    {
        Rueckgabe.SetKoordinaten(i,(Koordinate[i] * Mult));
    }
    return (Rueckgabe);
}

Vektor& Vektor::operator*=(double Mult)
{
    for(int i = 0; i < 3; i++)
    {
        Koordinate[i] *= Mult;
    }
    return *this;
}

const Vektor Vektor::operator-(const Vektor& SubVektor) const
{
    Vektor Rueckgabe = *this;
    Rueckgabe-=SubVektor;
    return(Rueckgabe);
}

Vektor& Vektor::operator-=(const Vektor& SubVektor)
{
    if(this==&SubVektor)return *this;
    for(int i = 0; i < 3; i++)
    {
        Koordinate[i] -= SubVektor.GetKoordinaten(i);
    }
    return *this;
}

Vektor Vektor::operator/(double Div)
{
    Vektor Rueckgabe = *this;
    Rueckgabe /= Div;
    return(Rueckgabe);
}

#include <iostream>
Vektor& Vektor::operator/=(double Div)
{
    if(Div == 0)
    {
        /*cerr<<"\n"<<"Division durch null!"<<flush;*/
        return *this;
    }
    for(int i = 0; i < 3;i++)
    {
        Koordinate[i] /= Div;
    }
    return *this;
}

double Vektor::operator*(const Vektor& Mult)
{
    return(Koordinate[0]*Mult.GetKoordinaten(0)+Koordinate[1]*Mult.GetKoordinaten(1)+Koordinate[2]*Mult.GetKoordinaten(2));
}

Vektor Vektor::Kreuz(const Vektor& KrVek)
{
    Vektor Rueckgabe;
    Rueckgabe.SetKoordinaten(0,(Koordinate[1] * KrVek.GetKoordinaten(2) - Koordinate[2] * KrVek.GetKoordinaten(1)));
    Rueckgabe.SetKoordinaten(1,(Koordinate[2] * KrVek.GetKoordinaten(0) - Koordinate[0] * KrVek.GetKoordinaten(2)));
    Rueckgabe.SetKoordinaten(2,(Koordinate[0] * KrVek.GetKoordinaten(1) - Koordinate[1] * KrVek.GetKoordinaten(0)));
    return(Rueckgabe);
}

double Vektor::Laenge(void)const
{
    double Laenge;
    Laenge = sqrt(Koordinate[0]*Koordinate[0]+Koordinate[1]*Koordinate[1]+Koordinate[2]*Koordinate[2]);
    return(Laenge);
}

double Vektor::ProjLaenge(int ProjektionsAchse)const
{
    double Laenge;
    int _x, _y;

    _x = (ProjektionsAchse+1)%3;
    _y = (ProjektionsAchse+2)%3;

    Laenge = sqrt(Koordinate[_x]*Koordinate[_x]+Koordinate[_y]*Koordinate[_y]);

    return(Laenge);
}

Vektor Vektor::drehen(double alpha, double beta, double gamma)
{
    Vektor Rueckgabe;
    if(alpha){
        Rueckgabe.SetKoordinaten(0, (cos(alpha)*Koordinate[0]+sin(alpha)*Koordinate[1]));
        Rueckgabe.SetKoordinaten(1, (-sin(alpha)*Koordinate[0]+cos(alpha)*Koordinate[1]));
        Rueckgabe.SetKoordinaten(2, Koordinate[2]);
    }
    if(beta){
        Rueckgabe.SetKoordinaten(0, (cos(beta)*Koordinate[0]-sin(beta)*Koordinate[2]));
        Rueckgabe.SetKoordinaten(1, Koordinate[1]);
        Rueckgabe.SetKoordinaten(2, (sin(beta)*Koordinate[0]+cos(beta)*Koordinate[2]));
    }
    if(gamma){
        Rueckgabe.SetKoordinaten(0, Koordinate[0]);
        Rueckgabe.SetKoordinaten(1, (cos(gamma)*Koordinate[1]+sin(gamma)*Koordinate[2]));
        Rueckgabe.SetKoordinaten(2, (-sin(gamma)*Koordinate[1]+cos(gamma)*Koordinate[2]));
    }
    return Rueckgabe;
}
/*Ende Vektor*/

