/*
  Name: demo_c.cc
  Author: Wojtek Slominski
  Date: 2009-12-18 22:22
  Description:
    an example program using ZDPDF.
    For each Q^2 given in QQvals
    the program prints all xPom*z*PDFs
*/

#include <iostream>
//#include <cstdlib>
//#include <unistd.h>

using namespace std;
#include <string>
#include "gZDPDF.h" 

//==========================================================
int main(int argc, char *argv[]) {
  const int Nq=4, Nx = 200;
  double QQvals[Nq]= {6,20,60,200};
  const double xleft=1e-4;
  const double xPom = 0.03;
  double f[7],QQ = pow(3.010956e+00,2),z = 4.664286e-01, dx;
  LoadGrid("../grids/zeusD_SJ");

  QQ = 10.8499;
  z  = 0.607857;

  Set_tmin(-1);
  ZeusDpdf3Pom(xPom, z, QQ, f); //--- Pomeron only
  //ZeusPom(z, QQ, f); //--- Pomeron only
  //Pom(z, QQ, f); //--- Pomeron only
 // printf("%6.4f",z);
  for(int ip=0; ip <= 6; ip++) printf(" %11.4e", f[ip]);
  printf("\n");
  
  return 0;


  puts("# columns: z  xg  xd  xu  xs  xc  xb  xt");
  dx = (1 - xleft)/(Nx-1);
  for(int iq=0; iq < Nq; iq++) {
    QQ = QQvals[iq];
    printf("# Q^2 = %.4g\n", QQ);
    for(int ix=0; ix < Nx; ix++) {
      z = xleft + ix*dx;
      //ZeusDpdf3(xPom, z, QQ, f); //--- Pomeron+Reggeon
      ZeusDpdf3Pom(xPom, z, QQ, f); //--- Pomeron only
      printf("%6.4f",z);
      for(int ip=0; ip <= 6; ip++) printf(" %11.4e", xPom*f[ip]);
      printf("\n");
    }
    printf("\n");
  }

  return 0;
} 
