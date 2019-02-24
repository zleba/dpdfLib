/*
  Author: Wojtek Slominski
  Date: 2009-12-18 22:22
  Description:
    For each Q^2 given in QQvals
    the program prints g,d,c,b xPom*z*PDFs and their errors.
    This code uses 'global' functions from "gZDPDF.h"
*/

#include <iostream>
#include <cstdlib>
#include <unistd.h>

using namespace std;
#include <string>
#include "gZDPDF.h"
#include "oFDstream.h"

#define Nerr 9

const int nMomErr=4;
double Err[nMomErr];
const int Nq=4;
double QQval[Nq] = {6,20,60,200};

#define nActPartons 4
int ActPind[] = {0,1,4,5}; //--- g,d,c,b

#define FILL_PDFS ZeusDpdf3Pom

inline double Sqr(double x) {return x*x;}

//==========================================================
string File_Name(const string& fn, bool fullpath=false) {
  int spos, dpos,n;
  if(fullpath) spos = 0;
  else {
    spos = fn.find_last_of('/');
    if(spos == string::npos) spos = fn.find_last_of('\\');
    if(spos == string::npos) spos = 0; else spos++;
  }
  dpos = fn.find_last_of('.');
  n = dpos-spos;
  if(n <= 0) n = string::npos;
  return fn.substr(spos, n);
}

//==========================================================
void fErr(double xPom, double z, double QQ, double f[]) {
  double dfa[N_FLAVORS+1];
  int j,a;
  for(j=0; j < N_FLAVORS; j++) f[j] = 0;
  for(a=1; a <= Nerr; a++) {
    SelectGrid(a);
    FILL_PDFS(xPom, z, QQ, dfa);
    for(j=0; j < nActPartons; j++) f[ActPind[j]] += Sqr(dfa[ActPind[j]]);
  }
  for(j=0; j < nActPartons; j++) f[ActPind[j]] = sqrt(f[ActPind[j]]);
}

//==========================================================
void MkErr(const string& fname, double xPom) {
  cout << "MkErr: saving pdfs to "<< fname << endl;
  oFDstream Vec(fname.c_str());
  Vec.comment(" pdferr");
  Vec.TimeStamp();
  string Tit("x,xg,xd,xc,xb,xDg,xDd,xDc,xDb");
  Vec.ColTitles(Tit.c_str());

  int i,k,iq;
  const int Nx = 400;
  double x,df[N_FLAVORS+1];
  const double xleft=1e-4;
  double dx = (1 - xleft)/(Nx-1);
  for(iq=0; iq < Nq; iq++) {
    double QQ = QQval[iq];
    cout << "QQ = "<< QQ << endl;
    Vec << "#$QQ= " << QQ << endl;
    for(k = 0; k < Nx; k++) {
      x = xleft + k*dx;
      //cout << "x = "<< x << endl;
      Vec.wr(x);
      SelectGrid(0);
      FILL_PDFS(xPom, x, QQ, df);
      for(i=0; i < nActPartons; i++) Vec.wr(xPom*df[ActPind[i]]);
      fErr(xPom, x, QQ, df);
      for(i=0; i < nActPartons; i++) Vec.wr(xPom*df[ActPind[i]]);
      Vec << endl;
    }
    Vec << endl;
  }
}

//==========================================================
//==========================================================
int main(int argc, char *argv[]) {
  if(argc < 2) {
    cout << "Grid file name missing" << endl;
    exit(1);
  }
  string Label = argv[1];
  for(int a=-Nerr; a <= Nerr; a++) LoadGrid(Label,a);
  SetDeltaMode(1);
  MkErr(File_Name(Label)+".dat", 0.003);
  return 0;
}
