#include "Kamera_3D.h"
#include "Vektor/Vektor.h"
#include "Matrix/Matrix.h"

Kamera::Kamera(Vektor& v_Standpunkt, double v_hoehenwinkel, double v_seitenwinkel, double v_FOV, double v_Abstand)
{
    sPkt = v_Standpunkt; /*Standpunkt*/

    w_Hoehe = v_hoehenwinkel;
    w_Seite = v_seitenwinkel;

    double cos_a, cos_b, sin_a, sin_b;
    cos_a = cos(w_Hoehe);
    sin_a = sin(w_Hoehe);
    cos_b = cos(w_Seite);
    sin_b = sin(w_Seite);

    n0.SetKoordinaten(cos_a*cos_b, cos_a*sin_b, sin_a);
    Ausrichten();
    /*h0.SetKoordinaten(-sin_a*cos_b, -sin_a*sin_b, cos_a);
    r0.SetKoordinaten(sin_b, -cos_b, 0.0);*/

    _FOV = v_FOV;
    msAbstand = v_Abstand;
}

Kamera::~Kamera()
{

}

void Kamera::FOV(double v_FOV)
{
    _FOV = v_FOV;
    return;
}

void Kamera::Verschieben(double rechts, double vorne, double oben) /*negative werte verschieben nach links, hinten oder unten*/
{
    sPkt = sPkt + (r0 * rechts + n0 * vorne + h0 * oben);
    return;
}

void Kamera::Drehen(double delta_alpha, double delta_beta) /*Hoehenwinkel, Seitenwinkel*/
{
    w_Hoehe += delta_alpha;
    w_Seite += delta_beta;

    double cos_a, cos_b, sin_a, sin_b;
    //cos_a = cos(w_Hoehe);
    //sin_a = sin(w_Hoehe);
    cos_a = cos(delta_alpha);
    sin_a = sin(delta_alpha);
    double grundRichtung = sqrt(pow(n0.x(), 2)+pow(n0.y(), 2));

    Matrix drM(3, 3);
        drM.wert(0, 0, cos_a);
        drM.wert(0, 1, 0.0);
        drM.wert(0, 2, sin_a);

        drM.wert(1, 0, 0);
        drM.wert(1, 1, 1);
        drM.wert(1, 2, 0.0);

        drM.wert(2, 0, -sin_a);
        drM.wert(2, 1, 0.0);
        drM.wert(2, 2, cos_a);
    Matrix n0Quer(3, 1);
        n0Quer.wert(0, 0, grundRichtung);
        n0Quer.wert(1, 0, 0.0);
        n0Quer.wert(2, 0, n0.z());

    n0Quer = drM * n0Quer;

    n0.SetKoordinaten(0, (n0.x()/grundRichtung) * n0Quer.wert(0, 0));
    n0.SetKoordinaten(1, (n0.y()/grundRichtung) * n0Quer.wert(0, 0));
    n0.SetKoordinaten(2, n0Quer.wert(2, 0));
    n0 /=n0.Laenge();

    //cos_b = cos(w_Seite);
    //sin_b = sin(w_Seite);
    cos_b = cos(delta_beta);
    sin_b = sin(delta_beta);

    drM.wert(0, 0, cos_b);
    drM.wert(0, 1, -sin_b);
    drM.wert(0, 2, 0.0);

    drM.wert(1, 0, sin_b);
    drM.wert(1, 1, cos_b);
    drM.wert(1, 2, 0.0);

    drM.wert(2, 0, 0.0);
    drM.wert(2, 1, 0.0);
    drM.wert(2, 2, 1);

    n0Quer.wert(0, 0, n0.x());
    n0Quer.wert(1, 0, n0.y());
    n0Quer.wert(2, 0, n0.z());

    n0Quer = drM * n0Quer;

    n0.SetKoordinaten(0, n0Quer.wert(0, 0));
    n0.SetKoordinaten(1, n0Quer.wert(1, 0));
    n0.SetKoordinaten(2, n0Quer.wert(2, 0));

    n0 /= n0.Laenge();
    Ausrichten();
    return;
}

void Kamera::Ausrichten(void)
{
    r0 = n0.Kreuz(Vektor(0.0, 0.0, 1.0));
    r0 /= r0.Laenge();
    h0 = r0.Kreuz(n0);
    return;
}

Vektor Kamera::Aufnahme(const Vektor& realPkt)
{
    Vektor sehStrahl = sPkt;
    sehStrahl = sehStrahl - realPkt;
    double xRueck, yRueck, zRueck;

    gDet = r0.x() * h0.y() * sehStrahl.z() - sehStrahl.x() * h0.y() * r0.z()
            + h0.x() * sehStrahl.y() * r0.z() - r0.x() * sehStrahl.y() * h0.z()
            + sehStrahl.x() * r0.y() * h0.z() - h0.x() * r0.y() * sehStrahl.z();

    if(gDet == 0)return Vektor(0, 0, -1);

    detax = h0.y() * sehStrahl.z() - sehStrahl.y() * h0.z();
    detay = h0.z() * sehStrahl.x() - sehStrahl.z() * h0.x();
    detaz = h0.x() * sehStrahl.y() - sehStrahl.x() * h0.y();

    detbx = sehStrahl.y() * r0.z() - r0.y() * sehStrahl.z();
    detby = sehStrahl.z() * r0.x() - r0.z() * sehStrahl.x();
    detbz = sehStrahl.x() * r0.y() - r0.x() * sehStrahl.y();

    detcx = r0.y() * h0.z() - h0.y() * r0.z();
    detcy = r0.z() * h0.x() - h0.z() * r0.x();
    detcz = r0.x() * h0.y() - h0.x() * r0.y();

    Vektor delta_n0 = n0 * (-msAbstand);

    xRueck = delta_n0.x() * detax + delta_n0.y() * detay + delta_n0.z() * detaz;
    yRueck = delta_n0.x() * detbx + delta_n0.y() * detby + delta_n0.z() * detbz;
    zRueck = delta_n0.x() * detcx + delta_n0.y() * detcy + delta_n0.z() * detcz;

    return(Vektor(xRueck/gDet, yRueck/gDet, zRueck/gDet));
}

const Vektor Kamera::HoleOrt(void)
{
    return(sPkt);
}

const Vektor Kamera::HoleBlickRichtung(void)
{
    return(n0);
}

void Kamera::SetzeBlickRichtung(const Vektor bR)
{
    n0 = bR;
    n0 -= sPkt;
    n0 /= n0.Laenge();
    Ausrichten();
    return;
}

void Kamera::SetzeAbstand(double wert)
{
    if(wert>0)
    {
        msAbstand = wert;
    }
}

void Kamera::InkrAbstand(double wert)
{
    msAbstand += wert;
    if(msAbstand<1)
    {
        msAbstand = 1;
    }
}
