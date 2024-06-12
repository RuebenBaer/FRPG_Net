#include <iostream>
#include "Matrix.h"
using namespace std;

/*************************
Implementierung von Matrix
*************************/

Matrix::Matrix(int x_dim, int y_dim)
{
    tDim_x = x_dim;
    tDim_y = y_dim;
    tElement = new double[tDim_x * tDim_y];
    tInvElement = new double[tDim_x * tDim_y];
}


Matrix::Matrix(const Matrix& Vorlage)
{
    //delete []tElement;
    //delete []tInvElement;
    tDim_x = Vorlage.tDim_x;
    tDim_y = Vorlage.tDim_y;
    tElement = new double[tDim_x * tDim_y];
    tInvElement = new double[tDim_x * tDim_y];
}

Matrix::~Matrix()
{
    delete []tElement;
    delete []tInvElement;
}

Matrix& Matrix::operator=(const Matrix& Vorlage)
{
    if(this==&Vorlage)return *this;

    delete []tElement;
    delete []tInvElement;

    tDim_x = Vorlage.tDim_x;
    tDim_y = Vorlage.tDim_y;
    tElement = new double[tDim_x * tDim_y];
    tInvElement = new double[tDim_x * tDim_y];

    for(int i=0; i<(Vorlage.tDim_x*Vorlage.tDim_y); i++)
    {
        tElement[i] = Vorlage.tElement[i];
        tInvElement[i] = Vorlage.tInvElement[i];
    }
    return *this;
}

Matrix Matrix::operator*(const Matrix& MultMatr)
{
    Matrix Rueckgabe = Matrix(tDim_x, MultMatr.Dimension(1));
    if(MultMatr.Dimension(0)!=tDim_y)
    {
        cerr<<"Dimensionen Stimmen nicht! Keine Multiplikation moeglich.";
        return Rueckgabe;
    }
    /*cout<<"This:\n";
    printScreen();
    cout<<"MultMatr:\n";
    MultMatr.printScreen();*/

    int DimRueck_x=Rueckgabe.Dimension(0);
    int DimRueck_y=Rueckgabe.Dimension(1);
    double tempWert;

    /*cout<<"DimRueck_x: "<<DimRueck_x<<endl;
    cout<<"DimRueck_y: "<<DimRueck_y<<"\n"<<endl;*/

    for(int i=0; i<DimRueck_x; i++)
    {
        for(int j=0; j<DimRueck_y; j++)
        {
            tempWert=0;
            for(int k=0; k<DimRueck_x; k++)
            {
                //cout<<wert(i, k)<<"*"<<MultMatr.wert(k, j)<<" + ";
                tempWert+=(wert(i, k)*MultMatr.wert(k, j));
            }
            //cout<<"\n";
            Rueckgabe.wert(i, j, tempWert);
        }//cout<<"\n";
    }
    return Rueckgabe;
}

double Matrix::wert(int Reihe, int Spalte)const
{
    return (tElement[Reihe * tDim_y + Spalte]);
}

void Matrix::wert(int Reihe, int Spalte, double wert)
{
    tElement[Reihe * tDim_y + Spalte] = wert;
    return;
}

double Matrix::inv_wert(int Reihe, int Spalte)const
{
    return (tInvElement[Reihe * tDim_y + Spalte]);
}

void Matrix::inv_wert(int Reihe, int Spalte, double wert)
{
    tInvElement[Reihe * tDim_y + Spalte] = wert;
    return;
}

int Matrix::Dimension(int Dim)const
{
    if(Dim ==0)return tDim_x;
    return tDim_y;
}

void Matrix::EinheitsMatrix(void)
{
    for(int i=0;i<tDim_x;i++)
    {
        for(int j=0;j<tDim_y;j++)
        {
            wert(i,j,(i==j));
        }
    }
}

bool Matrix::MInverse(void)
{
    double Fkt;
    for(int i=0;i<tDim_x;i++)
    {
        for(int j=0;j<tDim_y;j++)
        {
            inv_wert(i,j,(i==j));
        }
    }

    for(int i=0;i<tDim_x;i++)
    {
        /*Nuller in der Diagonale entfernen*/
        if(wert(i,i)==0)
        {
            for(int j=0;j<tDim_x;j++)
            {
                if(j!=i)
                {
                    for(int k=0;k<tDim_y;k++)
                    {
                        wert(i,k,(wert(i,k)+wert(j,k)));
                        inv_wert(i,k,(inv_wert(i,k)+inv_wert(j,k)));
                    }
                }
                if(wert(i,i)!=0)
                {
                    break;
                }
            }
            if(wert(i,i)==0)
            {
                return 0;
            }
        }
    }
    /*vordere Haelfte der Matrix nullen*/
    for(int i=0;i<tDim_x;i++)
    {
        for(int j=1+i;j<tDim_x;j++)
        {
            Fkt=wert(j,i)/wert(i,i);
            for(int k=0;k<tDim_y;k++)
            {
                wert(j,k,(wert(j,k)-wert(i,k)*Fkt));
                inv_wert(j,k,(inv_wert(j,k)-inv_wert(i,k)*Fkt));
            }
            /*printScreen();*/
        }
    }

    //hintere Haelfte der Matrix nullen
    for(int i=(tDim_x-1);i>-1;i--)
    {
        for(int j=i-1;j>=0;j--)
        {
            Fkt=(wert(j,i)/wert(i,i));
            for(int k=0;k<tDim_y;k++)
            {
                wert(j,k,(wert(j,k)-wert(i,k)*Fkt));
                inv_wert(j,k,(inv_wert(j,k)-inv_wert(i,k)*Fkt));
            }
            /*printScreen();*/
        }
        Fkt=(1/wert(i,i));
        for(int k=0;k<tDim_y;k++)
        {
            wert(i,k,(wert(i,k)*Fkt));
            inv_wert(i,k,(inv_wert(i,k)*Fkt));
        }
        /*printScreen();*/
    }
    InhaltDrehen();
    return 1;
}

void Matrix::InhaltDrehen(void)
{
    double tempDbl;
    for(int i=0;i<tDim_x;i++)
    {
        for(int j=0;j<tDim_y;j++)
        {
            tempDbl=inv_wert(i,j);
            wert(i,j,inv_wert(i,j));
            inv_wert(i,j,tempDbl);
        }
    }
}

void Matrix::printScreen(void)const
{
    for(int Reihe=0; Reihe<tDim_x; Reihe++)
    {
        cout<<endl;
        for(int Spalte=0; Spalte<tDim_y; Spalte++)
        {
            cout<<tElement[Reihe * tDim_y + Spalte]<<" ";
        }
    }
    cout<<endl;
    return;
}
