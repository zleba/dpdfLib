/*
  Author: Wojtek Slominski
  Date: 2009-12-17 22:22
  Description:
    main class for ZEUS diffractive PDFs 2009.
*/

#include <cstdio>
#include <cmath>
#include "zdpdf.h"
#include "emsg.h"
#include "tblipol.h"
#include "pion/grvpi.h"
#include <fstream>

using namespace std;

//#define Z_TEST_

typedef real_type* VECTORP;
#define qsum (1+N_FLAVORS)

int zdpdf_t::QQrelax;

//==========================================================
void zdpdf_t::SetOpts(const string& opt) {
  int p;
  if((p=opt.find('c')) != string::npos) {
    if(++p < opt.length()) CommentChar = opt[p];
  }
  if((p=opt.find('v')) != string::npos) Verbose = 1;
}

//==========================================================
void zdpdf_t::AllocWksp1(int n) {
  int pn, iq;
  try {
    for(pn=0; pn <= n; pn++) {
      fi_a[pn] = new VECTORP[Qgrid.npt];
      //cout << fi_a[pn][0] << endl;
      for(iq = 0; iq < Qgrid.npt; iq++) {
        fi_a[pn][iq] = new real_type[Xgrid.nx];
        memset(fi_a[pn][iq],0,Xgrid.nx*sizeof(real_type));
      }
    }
  }
  catch(...) { emsg(1, "AllocWksp1: could not allocate."); }
}

//==========================================================
void zdpdf_t::AllocWksp() {
  alocNf = 1 +N_FLAVORS;
  AllocWksp1(alocNf);
  fi = fi_a;// + N_FLAVORS;
}

//==========================================================
void zdpdf_t::DeAllocWksp() {
  int pn, iq;
  for(pn=0; pn <= alocNf; pn++) {
    for(iq = 0; iq < Qgrid.npt; iq++) delete[] fi_a[pn][iq];
    delete[] fi_a[pn];
  }
  //cout << "DeAlloc_q: 2" << endl;
}

//=================================
void zdpdf_t::ClearAll() {
  int pn, iq;
  for(pn=0; pn <= alocNf; pn++)
    for(iq = 0; iq < Qgrid.npt; iq++)
      memset(fi_a[pn][iq],0,Xgrid.nx*sizeof(real_type));
}

//==========================================================
void zdpdf_t::ShowInfo() {
  cout << CommentChar <<" Grid label: " << header.lbl << endl;
  //cout << "Reggeon_factor "<< Reggeon_factor <<endl;
  //Qgrid.show();
  cout << CommentChar << " Q^2 range = "<< Qgrid.qqVal(Qgrid.tlo) <<" -- "<< Qgrid.qqVal(Qgrid.thi)
    <<" GeV^2" <<endl;
  if(Qgrid.it_c >= 0) cout << CommentChar << " m_c = "<< sqrt(Qgrid.getQQ(Qgrid.it_c))<<" GeV"<<endl;
  if(Qgrid.it_b >= 0) cout << CommentChar << " m_b = "<< sqrt(Qgrid.getQQ(Qgrid.it_b))<<" GeV"<<endl;
  if(Qgrid.it_t >= 0) cout << CommentChar << " m_t = "<< sqrt(Qgrid.getQQ(Qgrid.it_t))<<" GeV"<<endl;
  cout << "Radek " << header.x_exp << endl;
  //cout << "nx "<< Xgrid.nx <<endl;
  cout << CommentChar << " min. x = "<< Xgrid.xlo <<endl;
  cout << CommentChar << " N distr. "<< header.Ndistr <<endl;
  cout << CommentChar << "----------------------------"<< endl;
}

//===========================================================
real_type zdpdf_t::Pom1(real_type zP, real_type QQ, int pn, int xpow) {
  if(pn < 0) pn = -pn;
  real_type t, xfac;
  t = Qgrid.tVal(QQ);
  int it = FindIndex(Qgrid.tval, Qgrid.npt, t);
  if(it < 0 && (QQrelax || fabs(Qgrid.tlo - t) < 1e-4)) {it = 0; t = Qgrid.tlo;}
  if(QQrelax) {
    if(it >= Qgrid.npt-1) {it = Qgrid.npt-1; t = Qgrid.thi;}
  } else
  require((it >= 0) && (it < Qgrid.npt-1),
    "zdpdf_t::Pom1: Q^2 = %lf outside declared Q^2 range [%lf, %lf]",
    (double)QQ, (double)Qgrid.qqVal(Qgrid.tlo), (double)Qgrid.qqVal(Qgrid.thi)
  );

  xfac = pow(zP, xpow-header.x_exp) /(2*M_PI);
  int activeflav = Qgrid.nFlavors[it];
  //--- 2-dim interpolation, lin. in t
  if(pn > activeflav) return 0.0;
  if(pn == 2 || pn == 3) pn = 1;
  return Ipol2(t, zP, Qgrid.tval, Xgrid.x, fi[pn], Qgrid.npt, Xgrid.nx, 1)
			      *xfac;
}

//===========================================================
void zdpdf_t::Pom(real_type zP, real_type QQ, real_type f[], int xpow) {
  //--- sizeof(f) must be 7*real_type
  real_type t, xfac;
  static int highQcount;
  t = Qgrid.tVal(QQ);
  int it = FindIndex(Qgrid.tval, Qgrid.npt, t);
  if(it < 0 && (QQrelax || fabs(Qgrid.tlo - t) < 1e-4)) {it = 0; t = Qgrid.tlo;}
  if(QQrelax) {
    if(it >= Qgrid.npt-1) {
      it = Qgrid.npt-2;
      highQcount++;
      if(highQcount < 10) cout << highQcount <<": "<< Qgrid.qqVal(t);
      t = Qgrid.thi -0.01;
      //t = Qgrid.tval[it]-0.01;
      if(highQcount < 10) cout <<" -> "<< Qgrid.qqVal(t) << endl;
    }
  } else
  require((it >= 0) && (it < Qgrid.npt-1),
    "zdpdf_t::Pom: Q^2 = %lf outside declared Q^2 range [%lf, %lf]",
    (double)QQ, (double)Qgrid.qqVal(Qgrid.tlo), (double)Qgrid.qqVal(Qgrid.thi)
  );

  xfac = pow(zP, xpow-header.x_exp) /(2*M_PI);
  int activeflav = Qgrid.nFlavors[it];
  //--- 2-dim interpolation, lin. in t
  int pn;

  f[0] = xfac*Ipol2(t, zP, Qgrid.tval, Xgrid.x, fi[0], Qgrid.npt, Xgrid.nx,1);
  f[1] = xfac*Ipol2(t, zP, Qgrid.tval, Xgrid.x, fi[1], Qgrid.npt, Xgrid.nx,1);
  cout << "InterpolationG " << Ipol2(t, zP, Qgrid.tval, Xgrid.x, fi[0], Qgrid.npt, Xgrid.nx,1) << " " << fi[0][0][0] <<" "<< fi[0][1][0]<<" "<< fi[0][0][1]<< endl;
  cout << "InterpolationQ " << Ipol2(t, zP, Qgrid.tval, Xgrid.x, fi[1], Qgrid.npt, Xgrid.nx,1) << " " << fi[1][0][0] <<" "<< fi[1][1][0]<<" "<< fi[1][0][1]<< endl;
  cout << "xfac orgQuar" <<  xfac <<" "<< Ipol2(t, zP, Qgrid.tval, Xgrid.x, fi[1], Qgrid.npt, Xgrid.nx,1) << " " << f[1] <<" "<< fi[1][0][0] <<" "<< exp(Qgrid.tval[0]) << " "<< Xgrid.x[0]<< endl;
  f[3] = f[2] = f[1];
  //cout << f[1];
  for(pn = 4; pn <= activeflav; pn++) {
    f[pn] = xfac*Ipol2(t, zP, Qgrid.tval, Xgrid.x, fi[pn], Qgrid.npt, Xgrid.nx,1);
    //cout <<" "<< f[pn];
  }
  //cout << endl;
  for(pn = activeflav+1; pn <= N_FLAVORS; pn++) f[pn] = 0;
}

//================================================================
void zdpdf_t::pi0xf(real_type x, real_type QQ, real_type f[]) {
//--- f[0:N_FLAVORS]
  int k;
  //--- pion
  double xF[N_FLAVORS+1]; //--- g, uval=dval, usea=dsea, s,c,b
  if(PionOrder) GRVpi_HO(x, QQ, xF); else  GRVpi_LO(x, QQ, xF);
  f[0] = xF[0];
  f[1] = f[2] = xF[1]/2 + xF[2];
  for(k=3; k <= 5; k++) f[k] = xF[k];
  if(N_FLAVORS >= 6) f[6] = 0;
}

//================================================================
void zdpdf_t::fxP(real_type xP, real_type zP, real_type QQ,
    real_type f[], int xpow) {
  real_type flux = Pflux.f(xP);
  cout <<"Radek xp " << xP << " : " << flux << endl;
  Pom(zP, QQ, f, xpow);
  cout <<"Radek zp QQ " << zP <<" "<<QQ <<" : "<< f[0] <<" "<< f[1] <<" "<< f[2] <<" "<< f[3] <<" "<< f[4] <<  endl;
  #ifdef Z_TEST_
    for(int pn = 0; pn <= N_FLAVORS; pn++) cout << f[pn] << endl;
  #endif
  for(int pn = 0; pn <= N_FLAVORS; pn++) f[pn] *= flux;
}

//================================================================
void zdpdf_t::fxR(real_type xP, real_type zP, real_type QQ,
    real_type f[], int xpow) {
  real_type flux = Rflux.f(xP);
  real_type cx = pow(zP, xpow-1);
  pi0xf(zP, QQ, f);
  for(int pn = 0; pn <= N_FLAVORS; pn++) f[pn] *= Reggeon_factor*cx*flux;
}

//================================================================
void zdpdf_t::xDPDF3(real_type xP, real_type zP, real_type QQ,
    real_type f[], int xpow) {
  real_type fP[N_FLAVORS+1], fR[N_FLAVORS+1];
  fxP(xP, zP, QQ, fP, xpow);
  fxR(xP, zP, QQ, fR, xpow);
  for(int pn = 0; pn <= N_FLAVORS; pn++) f[pn] = fP[pn] + fR[pn];
}
