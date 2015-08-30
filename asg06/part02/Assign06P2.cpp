#include <iostream>
#include <cstdlib>
using namespace std;

long rCalcPoly(int x, const int c[], int degree);
void rShowPoly(int x, const int c[], int degree, long value);
void rShowPolyAux(int x, const int c[], int degree, long value, int n);

int main()
{
   int coeff[] = {-1, 8, -4, 7, 0, -3, -6, 2, 5};

   rShowPoly( -2, coeff, 0, rCalcPoly(-2, coeff, 0) );
   rShowPoly( -1, coeff, 0, rCalcPoly(-1, coeff, 0) );
   rShowPoly( 0, coeff, 0, rCalcPoly(0, coeff, 0) );
   rShowPoly( 1, coeff, 0, rCalcPoly(1, coeff, 0) );
   rShowPoly( 2, coeff, 0, rCalcPoly(2, coeff, 0) );
   cout << endl;
   rShowPoly( -2, coeff, 1, rCalcPoly(-2, coeff, 1) );
   rShowPoly( -1, coeff, 1, rCalcPoly(-1, coeff, 1) );
   rShowPoly( 0, coeff, 1, rCalcPoly(0, coeff, 1) );
   rShowPoly( 1, coeff, 1, rCalcPoly(1, coeff, 1) );
   rShowPoly( 2, coeff, 1, rCalcPoly(2, coeff, 1) );
   cout << endl;
   rShowPoly( -2, coeff, 3, rCalcPoly(-2, coeff, 3) );
   rShowPoly( -1, coeff, 3, rCalcPoly(-1, coeff, 3) );
   rShowPoly( 0, coeff, 3, rCalcPoly(0, coeff, 3) );
   rShowPoly( 1, coeff, 3, rCalcPoly(1, coeff, 3) );
   rShowPoly( 2, coeff, 3, rCalcPoly(2, coeff, 3) );
   cout << endl;
   rShowPoly( -2, coeff, 5, rCalcPoly(-2, coeff, 5) );
   rShowPoly( -1, coeff, 5, rCalcPoly(-1, coeff, 5) );
   rShowPoly( 0, coeff, 5, rCalcPoly(0, coeff, 5) );
   rShowPoly( 1, coeff, 5, rCalcPoly(1, coeff, 5) );
   rShowPoly( 2, coeff, 5, rCalcPoly(2, coeff, 5) );
   cout << endl;
   rShowPoly( -2, coeff, 8, rCalcPoly(-2, coeff, 8) );
   rShowPoly( -1, coeff, 8, rCalcPoly(-1, coeff, 8) );
   rShowPoly( 0, coeff, 8, rCalcPoly(0, coeff, 8) );
   rShowPoly( 1, coeff, 8, rCalcPoly(1, coeff, 8) );
   rShowPoly( 2, coeff, 8, rCalcPoly(2, coeff, 8) );
   cout << endl;

   return EXIT_SUCCESS;
}

long rCalcPoly(int x, const int c[], int degree) 
{
   if( degree < 0 ) 
      return (long) 0;
   else 
      return c[0] + x*rCalcPoly(x, c+1, degree-1);
}   

void rShowPoly(int x, const int c[], int degree, long value) 
{
   rShowPolyAux(x, c, degree, value, degree);
}

void rShowPolyAux(int x, const int c[], int degree, long value, int n)
{
   // recurse to base case (polynomial with degree 0):
   if( n >= 0 ) rShowPolyAux(x, c, degree, value, n-1); 

   // start collapsing the stack frames:
   if( n < 0 ) return;
   
   // print the one-time leader: 
   if( n == 0) 
   {
      cout << "p(x,n) = p(" << x << ", " << degree << ") = ";   
   }

   // print each individual term of the polynomial (one per frame):
   cout << ( c[n] >= 0 ? " +" : " " ) << c[n] << "x^" << n ; 
   
   // print the one-time trailer:
   if( n == degree )      
      cout << "= " << value << endl;         
}
