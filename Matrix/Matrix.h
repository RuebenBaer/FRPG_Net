#ifndef _Matrizenrechnung_
#define _Matrizenrechnung_

#include <cstdlib>

class Matrix
{
	private:
	   int tDim_x;
	   double *tElement, *tInvElement;
       int tDim_y;
       void InhaltDrehen(void);
	public:
	   Matrix(int,int);
	   Matrix(const Matrix&);
	   ~Matrix(void);
	   Matrix& operator=(const Matrix&);
	   bool MInverse(void);
	   double wert(int, int)const;
	   void wert(int, int, double);
	   double inv_wert(int, int)const;
	   void inv_wert(int, int, double);
	   void printScreen(void)const;
	   void EinheitsMatrix(void);
	   Matrix operator*(const Matrix&);
	   int Dimension(int)const;
};

#endif
