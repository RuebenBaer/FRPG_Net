#ifndef __KAMERA_3D_H
#define __KAMERA_3D_H

#include "Vektor/Vektor.h"
#include <cmath>

#define PI 3.14159265

struct _3D_Linie
{
    Vektor m_anfang, m_ende;
};

class Kamera
{
private:
    Vektor sPkt, n0, r0, h0; /*Standpunkt, Normale, Rechts, Hoch*/
    double w_Hoehe, w_Seite; /*Hoehenwinkel zur waagerechten, Seitenwinkel zur X-Achse*/
    double msAbstand, _FOV/*Cosinus des Winkels*/;
    double gDet, detax, detay, detaz, detbx, detby, detbz, detcx, detcy, detcz; /*Matrizenvariablen*/
    void Ausrichten(void);
public:
    Kamera(Vektor& v_Standpunkt, double v_hoehenwinkel, double v_seitenwinkel, double v_FOV, double v_Abstand);
    ~Kamera();
    double FOV(void)const{return (acos(_FOV)*180/PI);}
    void FOV(double v_FOV);
    void Verschieben(double, double, double);/*nach rechts, vorne, oben relativ zur Kamerablickrichtung*/
    void Drehen(double, double);/*Winkelaenderung gegen die waagerechte und gegen die X-Achse*/
    Vektor Aufnahme(const Vektor& realPkt);/*Umrechnung eines dreidimensionalen Punkts in ein zweidimensionales Abbild*/
    const Vektor HoleOrt(void);
    const Vektor HoleBlickRichtung(void);
    void SetzeBlickRichtung(const Vektor);
    const double HoleAbstand(void){return msAbstand;};
    void SetzeAbstand(double);
    void InkrAbstand(double);
};


#endif // __KAMERA_3D_H
