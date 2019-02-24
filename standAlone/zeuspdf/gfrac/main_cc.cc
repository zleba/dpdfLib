/*
  Author: Wojtek Slominski
  Date: 2009-12-17 22:22
  Description:
    gluon momentum fraction from the ZEUS DPDFs 2009
    with errors calculated from error grids.
    This code uses zdpdf_t class from "zdpdf.h"
*/

#include <iostream>
#include <cstdlib>
#include <unistd.h>

using namespace std;
#include <string.h>
#include "zdpdf.h"
#include "oFDstream.h"

#define Nerr 9
zdpdf_t *zPdf_[1+2*Nerr], *Pdf;
zdpdf_t **zPdf = zPdf_ +Nerr;
double *wksp; //--- working space allocated in main()

const int nMomErr=4;
double Err[nMomErr];

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
double TblInt2(double xi[], double yi[], int np) {
 double ys;
 int ii;

 if(np<2) return(0.0);
 ys=0;
 for(ii=1; ii < np; ii++) ys += (yi[ii]+yi[ii-1])*(xi[ii]-xi[ii-1]);
 return 0.5*ys;
}

//==========================================================
double pdfInt(int p, int iq) {
  for(int j=0; j < Pdf->Xgrid.nx; j++)
    wksp[j] = Pdf->Pom1(Pdf->Xgrid.x[j], Pdf->Qgrid.getQQ(iq), p);
  return TblInt2(Pdf->Xgrid.x, wksp, Pdf->Xgrid.nx);
}

//==========================================================
double gMomentum(int iq) {
  return pdfInt(0, iq);
}

//==========================================================
double qMomentum(int iq) {
  return 2*(
  #ifdef ASYM_STRNG
    2*pdfInt(1, iq)
    + pdfInt(3, iq)
  #else
    3*pdfInt(1, iq)
  #endif
    + pdfInt(4, iq)
    + pdfInt(5, iq)
  );
}

//==========================================================
void pMomErr(int iq) {
  double xg,xq, exg, exq, exr;
  for(int j=0; j < nMomErr; j++) Err[j] = 0;
  for(int a=1; a <= Nerr; a++) {
    Pdf =  zPdf[a];
    exg = gMomentum(iq);
    exq = qMomentum(iq);
    exr = exg/(exg+exq);
    //cout << exg<<" " << exq<<" " << exr<<endl;
    Pdf =  zPdf[-a];
    exg -= xg=gMomentum(iq);
    exq -= xq=qMomentum(iq);
    exr -= xg/(xg+xq);
    //cout <<"-  "<< exg<<" " << exq<<" " << exr<<endl;
    Err[0] += exg*exg;
    Err[1] += exq*exq;
    Err[2] += Sqr(exg+exq);
    Err[3] += exr*exr;
  }
  for(int j=0; j < nMomErr; j++) Err[j] = sqrt(Err[j])/2;
}

//==========================================================
void MkDat_pmom(const string& fname, double QQmin=0) {
  cout << "MkDat_pmom: saving parton momenta to "<< fname << endl;
  oFDstream Vec(fname.c_str());
  Vec.comment(" gfrac");
  Vec.TimeStamp();
  string Tit("QQ,xg,xq,xa,gfrac,Dxg,Dxq,Dxa,Dgfrac");
  Vec.ColTitles(Tit.c_str());

  int pn,iq = 0;
  double xg, xq;
  int Nq = zPdf[0]->Qgrid.npt;
  for(iq=0; iq < Nq; iq++) {
    Pdf = zPdf[0];
    Vec.wr(Pdf->Qgrid.getQQ(iq));
    xg = gMomentum(iq);
    xq = qMomentum(iq);
    Vec.wr(xg);
    Vec.wr(xq);
    Vec.wr(xg+xq);
    Vec.wr(xg/(xg+xq));
    pMomErr(iq);
    for(int j=0; j < nMomErr; j++) Vec.wr(Err[j]);
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
  for(int a=-Nerr; a <= Nerr; a++) zPdf[a] = new zdpdf_t(Label, a);
  wksp = new double[zPdf[0]->Xgrid.nx];
  MkDat_pmom(File_Name(Label)+".pam");
  return 0;
}
