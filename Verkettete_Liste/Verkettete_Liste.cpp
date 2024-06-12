#include "Verkettete_Liste.h"
/*
/*Listenelement*//*
template<class T> Listenelement<T>::Listenelement(){
  Element = 0;
  this->SetNachfolger(0);
  this->SetVorgaenger(0);
}

template<class T> Listenelement<T>::Listenelement(T* XY){
  Element = XY;
  this->SetNachfolger(0);
  this->SetVorgaenger(0);
}

template<class T> T* Listenelement<T>::GetElement(void){
  return(Element);
}

template<class T> void Listenelement<T>::SetNachfolger(Listenelement* NeuerNachfolger){
  Nachfolger = NeuerNachfolger;
  return;
}

template<class T> Listenelement<T>* Listenelement<T>::GetNachfolger(void){
  return(Nachfolger);
}

template<class T> void Listenelement<T>::SetVorgaenger(Listenelement* NeuerVorgaenger){
  Vorgaenger = NeuerVorgaenger;
  return;
}

template<class T> Listenelement<T>* Listenelement<T>::GetVorgaenger(void){
  return(Vorgaenger);
}

template<class T> Listenelement<T>::~Listenelement(){
  if(Vorgaenger){
    Vorgaenger->SetNachfolger(Nachfolger);
  }
  if(Nachfolger){
    Nachfolger->SetVorgaenger(Vorgaenger);
  }
}
/*Ende Listenelement*/

/*Liste*//*
template<class T> Liste<T>::Liste(){
  erstesElement = 0;
  aktuellesElement = 0;
}

template<class T> Liste<T>::~Liste()
{
    ListeLeeren();
}

template<class T> Listenelement<T>* Liste<T>::GetErstesElement(void){
    aktuellesElement = erstesElement;
    return(aktuellesElement);
}

template<class T> Listenelement<T>* Liste<T>::GetNaechstesElement(void){
    aktuellesElement = aktuellesElement->GetNachfolger();
    return(aktuellesElement);
}

template<class T> T* Liste<T>::GetAktuellenInhalt(void){
    if(aktuellesElement!=0){
        return(aktuellesElement->GetElement());
    }else{
        return(NULL);
    }
}

template<class T> void Liste<T>::Hinzufuegen(T* XY){
  Listenelement<T>* neuesElement = new Listenelement<T>(XY);
  neuesElement->SetNachfolger(erstesElement);
  neuesElement->SetVorgaenger(0);
  erstesElement = neuesElement;
  return;
}

template<class T> void Liste<T>::Entfernen(T* XY){
    if(XY==GetErstesElement()->GetElement())
    {
        Listenelement<T>* SuchElement = GetErstesElement();
        erstesElement = SuchElement->GetNachfolger();
        if(SuchElement == aktuellesElement) aktuellesElement = SuchElement->GetNachfolger();
        delete SuchElement;
        return;
    }
    for(Listenelement<T>* SuchElement=GetErstesElement(); SuchElement; SuchElement=GetNaechstesElement())
    {
        if(SuchElement->GetElement() == XY){
            if(SuchElement == aktuellesElement){
                if(SuchElement->GetNachfolger()!= 0){
                    aktuellesElement = SuchElement->GetNachfolger();
                }else if(SuchElement->GetVorgaenger()!= 0){
                    aktuellesElement = SuchElement->GetVorgaenger();
                }
            }
            delete SuchElement;
            break;
        }
    }
  return;
}

template<class T> void Liste<T>::ListeLeeren()
{
    while(erstesElement)
    {
        Entfernen(erstesElement->GetElement());
    }
    return;
}
*/
