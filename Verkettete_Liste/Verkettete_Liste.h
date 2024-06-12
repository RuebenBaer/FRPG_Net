#ifndef __Verkettete_Liste_
#define __Verkettete_Liste_
#include <cstdlib>
#include <iostream>

using namespace std;

template<class T>  class Listenelement
{
  private:
    T* Element;
    Listenelement* Nachfolger;
    Listenelement* Vorgaenger;
    double m_Wert;
  public:
    Listenelement();
    Listenelement(const Listenelement&);
    Listenelement(T*);
    ~Listenelement();
    const Listenelement& operator=(const Listenelement&) const;
    void SetNachfolger(Listenelement*);
    void SetVorgaenger(Listenelement*);
    Listenelement* GetNachfolger(void);
    Listenelement* GetVorgaenger(void);
    T* GetElement(void);
    void Wert(double);
    double Wert(void);
};


template<class T> class Liste
{
  private:
      Listenelement<T>* erstesElement;
      Listenelement<T>* aktuellesElement;
      int listenGroesse;
  public:
      Liste();
      Liste(const Liste&);
      ~Liste();
      Liste& operator=(const Liste&);
      Listenelement<T>* GetErstesListenelement(void);
      T* GetErstesElement(void);
      T* GetNaechstesElement(void);
      T* GetAktuellenInhalt(void);
      T* GetNachfolger(T*);
      T* GetVorgaenger(T*);
      void Hinzufuegen(T*);
      void Entfernen(T*);
      void Entfernen(Listenelement<T>*);
      void AktuellesEntfernen(void);
      void ListeLeeren(const char*);
      void ListeLoeschen(const char*);
      int Zaehlen(void);
      void Tauschen(Listenelement<T>*, Listenelement<T>*);
      void Hintenanstellen(Listenelement<T>*, Listenelement<T>*);
      void Voranstellen(Listenelement<T>*, Listenelement<T>*);
};

/*Listenelement*/
template<class T> Listenelement<T>::Listenelement()
{
  Element = 0;
  this->SetNachfolger(0);
  this->SetVorgaenger(0);
}

template<class T> Listenelement<T>::Listenelement(T* XY)
{
  Element = XY;
  this->SetNachfolger(0);
  this->SetVorgaenger(0);
}

template<class T> Listenelement<T>::Listenelement(const Listenelement& Vorlage)
{
    Element = Vorlage.Element;
    this->SetNachfolger(Vorlage.GetNachfolger());
    this->SetVorgaenger(Vorlage.GetVorgaenger());
}

template<class T> Listenelement<T>::~Listenelement()
{
    if(Vorgaenger)
    {
        Vorgaenger->SetNachfolger(Nachfolger);
    }
    if(Nachfolger)
    {
        Nachfolger->SetVorgaenger(Vorgaenger);
    }
}

template<class T> const Listenelement<T>& Listenelement<T>::operator=(const Listenelement& Vorlage) const
{
    if(this==&Vorlage)return *this;
    Element = Vorlage.Element;
    this->SetNachfolger(Vorlage.GetNachfolger());
    this->SetVorgaenger(Vorlage.GetVorgaenger());
    return *this;
}

template<class T> T* Listenelement<T>::GetElement(void)
{
  return(Element);
}

template<class T> void Listenelement<T>::SetNachfolger(Listenelement* NeuerNachfolger)
{
  Nachfolger = NeuerNachfolger;
  return;
}

template<class T> Listenelement<T>* Listenelement<T>::GetNachfolger(void)
{
  return(Nachfolger);
}

template<class T> void Listenelement<T>::SetVorgaenger(Listenelement* NeuerVorgaenger)
{
    Vorgaenger = NeuerVorgaenger;
    return;
}

template<class T> Listenelement<T>* Listenelement<T>::GetVorgaenger(void)
{
  return(Vorgaenger);
}

template<class T> void Liste<T>::Hintenanstellen(Listenelement<T>* Wanderer, Listenelement<T>* Anker)
{
    if(Anker == Wanderer)return;
    if(Wanderer == erstesElement)erstesElement = Wanderer->GetNachfolger();
    if(Anker!=NULL)
    {
        if(Anker->GetNachfolger() == Wanderer)return;

        Listenelement<T> *vw, *nw, *na;
        vw = Wanderer->GetVorgaenger();
        nw = Wanderer->GetNachfolger();
        na = Anker->GetNachfolger();

        if(nw == Anker)
        {
            Anker->SetVorgaenger(vw);
            if(vw != NULL)vw->SetNachfolger(Anker);

            Wanderer->SetNachfolger(na);
            if(na != NULL)na->SetVorgaenger(Wanderer);

            Anker->SetNachfolger(Wanderer);
            Wanderer->SetVorgaenger(Anker);
        }else{

            if(vw != NULL)vw->SetNachfolger(nw);
            if(nw != NULL)nw->SetVorgaenger(vw);

            Wanderer->SetVorgaenger(Anker);
            Wanderer->SetNachfolger(na);
            Anker->SetNachfolger(Wanderer);
            if(na != NULL)na->SetVorgaenger(Wanderer);
        }
    }
    return;
}

template<class T> void Liste<T>::Voranstellen(Listenelement<T>* Wanderer, Listenelement<T>* Anker)
{
    if(Anker == Wanderer)return;
    if(Anker == erstesElement)erstesElement = Wanderer;
    if(Anker!=NULL)
    {
        if(Anker->GetVorgaenger()==Wanderer)return;

        Listenelement<T> *vw, *nw, *va;
        vw = Wanderer->GetVorgaenger();
        nw = Wanderer->GetNachfolger();
        va = Anker->GetVorgaenger();

        if(vw==Anker)
        {
            Anker->SetNachfolger(nw);
            if(nw != NULL)nw->SetVorgaenger(Anker);

            Wanderer->SetVorgaenger(va);
            if(va != NULL)va->SetNachfolger(Wanderer);

            Anker->SetVorgaenger(Wanderer);
            Wanderer->SetNachfolger(Anker);
        }else{
            if(vw != NULL)vw->SetNachfolger(nw);
            if(nw != NULL)nw->SetVorgaenger(vw);

            Wanderer->SetVorgaenger(va);
            Wanderer->SetNachfolger(Anker);

            Anker->SetVorgaenger(Wanderer);
            if(va != NULL)va->SetNachfolger(Wanderer);
        }
    }
    return;
}

template<class T> void Listenelement<T>::Wert(double a)
{
    m_Wert = a;
    return;
}

template<class T> double Listenelement<T>::Wert(void)
{
    return m_Wert;
}

/*Ende Listenelement*/

/*Liste*/
template<class T> Liste<T>::Liste()
{
  erstesElement = 0;
  aktuellesElement = 0;
  listenGroesse = 0;
}

template<class T> Liste<T>::Liste(const Liste& Vorlage)
{
    erstesElement = 0;
    aktuellesElement = 0;
    listenGroesse = 0;
    ListeLeeren("CopyCTOR");
    for(Listenelement<T>* SuchElement=Vorlage.erstesElement; SuchElement; SuchElement=SuchElement->GetNachfolger())
    {
        Hinzufuegen(SuchElement->GetElement());
    }
}

template<class T> Liste<T>::~Liste()
{
    ListeLeeren("DTOR");
    //cout<<"Liste loeschen\n";
}

template<class T> Liste<T>& Liste<T>::operator=(const Liste& Vorlage)
{
    if(this == &Vorlage)return *this;
    erstesElement = 0;
    aktuellesElement = 0;
    listenGroesse = 0;
    ListeLeeren("operator=");
    for(Listenelement<T>* SuchElement=Vorlage.erstesElement; SuchElement; SuchElement=SuchElement->GetNachfolger())
    {
        Hinzufuegen(SuchElement->GetElement());
    }
    return *this;
}

template<class T> T* Liste<T>::GetErstesElement(void)
{
    aktuellesElement = erstesElement;
    return(GetAktuellenInhalt());
}

template<class T> Listenelement<T>* Liste<T>::GetErstesListenelement(void)
{
    return(erstesElement);
}

template<class T> T* Liste<T>::GetNaechstesElement(void)
{
    if(aktuellesElement!=NULL)
    {
        aktuellesElement = aktuellesElement->GetNachfolger();
        return(GetAktuellenInhalt());
    }
    //system("PAUSE");
    return NULL;
}

template<class T> T* Liste<T>::GetAktuellenInhalt(void)
{
    if(aktuellesElement!=0){
        return(aktuellesElement->GetElement());
    }else{
        return(0);
    }
}

template<class T> void Liste<T>::Hinzufuegen(T* XY)
{
  Listenelement<T>* neuesElement = new Listenelement<T>(XY);

  if(erstesElement)erstesElement->SetVorgaenger(neuesElement);

  neuesElement->SetNachfolger(erstesElement);
  erstesElement /*= aktuellesElement */= neuesElement;
  listenGroesse++;
  return;
}

template<class T> void Liste<T>::Entfernen(T* XY)
{
    if(!(erstesElement))return;

    for(Listenelement<T>* SuchElement=erstesElement; SuchElement; SuchElement=SuchElement->GetNachfolger())
    {
        if(SuchElement->GetElement() == XY)
        {
            if(SuchElement == erstesElement)
            {
                erstesElement = SuchElement->GetNachfolger();
            }
            if(SuchElement == aktuellesElement)
            {
                if((SuchElement->GetNachfolger())!= NULL)
                {
                    aktuellesElement = SuchElement->GetNachfolger();
                }else{
                    aktuellesElement = SuchElement->GetVorgaenger();
                }
            }
            delete SuchElement;
            SuchElement = NULL;
            listenGroesse--;
            return;
        }
    }
  return;
}

template<class T> void Liste<T>::Entfernen(Listenelement<T> *XY)
{
    if(!(erstesElement))return;
    if(XY == erstesElement)
    {
        erstesElement = XY->GetNachfolger();
    }
    if(XY == aktuellesElement)
    {
        if((XY->GetNachfolger())!= NULL)
        {
            aktuellesElement = XY->GetNachfolger();
        }else{
            aktuellesElement = XY->GetVorgaenger();
        }
    }
    delete XY;
    XY = NULL;
    listenGroesse--;
    return;
}

template<class T> void Liste<T>::AktuellesEntfernen(void)
{
    if(!(aktuellesElement))return;

    Listenelement<T>* SuchElement = aktuellesElement;

    /*if((aktuellesElement->GetNachfolger())!= 0)
    {
        aktuellesElement = SuchElement->GetNachfolger();
    }else{
        aktuellesElement = SuchElement->GetVorgaenger();
    }*/
    aktuellesElement = SuchElement->GetNachfolger();

    delete SuchElement;
    SuchElement = 0;
    listenGroesse--;
    return;
}

template<class T> void Liste<T>::ListeLeeren(const char* von)
{
    while(erstesElement)
    {
        Entfernen(erstesElement->GetElement());
    }
    return;
}

template<class T> void Liste<T>::ListeLoeschen(const char* von)
{
    while(erstesElement)
    {
        T *Inhalt = erstesElement->GetElement();
        Entfernen(Inhalt);
        delete Inhalt;
    }
    return;
}

template<class T> int Liste<T>::Zaehlen()
{
    /*int Rueckgabe = 0;
    if(erstesElement)
    {
        for(Listenelement<T>* SuchElement=erstesElement; SuchElement; SuchElement=SuchElement->GetNachfolger())
        {
            Rueckgabe++;
            cout<<"Element "<<Rueckgabe<<": "<<SuchElement<<" - Inhalt: "<<SuchElement->GetElement()<<endl;
        }
    }else{
    }*/
    return listenGroesse;
}

template<class T> void Liste<T>::Tauschen(Listenelement<T>* LE_Eins, Listenelement<T>* LE_Zwei)
{
    if(LE_Eins == LE_Zwei)return;
    if((LE_Eins == NULL) ||(LE_Zwei == NULL)) return;

    Listenelement<T> *V1, *N1, *V2, *N2;

    if(LE_Eins->GetNachfolger() == LE_Zwei)
    {
        V1 = LE_Eins->GetVorgaenger();
        N2 = LE_Zwei->GetNachfolger();

        LE_Eins->SetVorgaenger(LE_Zwei);
        LE_Zwei->SetNachfolger(LE_Eins);

        LE_Eins->SetNachfolger(N2);
        LE_Zwei->SetVorgaenger(V1);

        if(V1)V1->SetNachfolger(LE_Zwei);
        if(N2)N2->SetVorgaenger(LE_Eins);
    }
    else if(LE_Zwei->GetNachfolger() == LE_Eins)
    {
        V2 = LE_Zwei->GetVorgaenger();
        N1 = LE_Eins->GetNachfolger();

        LE_Zwei->SetVorgaenger(LE_Eins);
        LE_Eins->SetNachfolger(LE_Zwei);

        LE_Zwei->SetNachfolger(N1);
        LE_Eins->SetVorgaenger(V2);

        if(V2)V2->SetNachfolger(LE_Eins);
        if(N1)N1->SetVorgaenger(LE_Zwei);
    }else
    {
        V1 = LE_Eins->GetVorgaenger();
        N1 = LE_Eins->GetNachfolger();
        V2 = LE_Zwei->GetVorgaenger();
        N2 = LE_Zwei->GetNachfolger();

        LE_Zwei->SetVorgaenger(V1);
        LE_Zwei->SetNachfolger(N1);
        LE_Eins->SetVorgaenger(V2);
        LE_Eins->SetNachfolger(N2);

        if(V1)V1->SetNachfolger(LE_Zwei);
        if(N1)N1->SetVorgaenger(LE_Zwei);
        if(V2)V2->SetNachfolger(LE_Eins);
        if(N2)N2->SetVorgaenger(LE_Eins);
    }
    if(erstesElement == LE_Eins)
    {
        erstesElement = LE_Zwei;
        return;
    }
    if(erstesElement == LE_Zwei)
    {
        erstesElement = LE_Eins;
        return;
    }
    return;
}


template<class T> T* Liste<T>::GetNachfolger(T* ausgangsElement)
{
    T* nachfolger = NULL;

    for(Listenelement<T>* SuchElement=erstesElement; SuchElement; SuchElement=SuchElement->GetNachfolger())
    {
        if(SuchElement->GetElement() == ausgangsElement)
        {
            if(SuchElement->GetNachfolger())
            nachfolger = SuchElement->GetNachfolger()->GetElement();
        }
    }
    return nachfolger;
}

template<class T> T* Liste<T>::GetVorgaenger(T* ausgangsElement)
{
    T* vorgaenger = NULL;

    for(Listenelement<T>* SuchElement=erstesElement; SuchElement; SuchElement=SuchElement->GetNachfolger())
    {
        if(SuchElement->GetElement() == ausgangsElement)
        {
            if(SuchElement->GetVorgaenger())
            vorgaenger = SuchElement->GetVorgaenger()->GetElement();
        }
    }
    return vorgaenger;
}

#endif
