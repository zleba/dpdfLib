/*
  Regge type flux
  ver. 2.00
*/
#ifndef FLUX_H_
#define FLUX_H_

#include <iostream>
#include <cstdio>
#include <cmath>

#ifndef NO_XDR
#include "xdr2cc.h"
#endif

//##################################################
struct Flux_t {
  double a0, b0;
  double a1,tmin;
  double om2a0; //--- 1 - 2*a0
  double cJLAP;

  public:
    //================================================
    void SetParams(double _a0, double _a1, double _b0, double _tmin) {
      SetA0(_a0);
      a1 = _a1;
      b0 = _b0;
      cJLAP = (1 - exp(-b0))/b0;
      tmin = _tmin;
    }

    //================================================
    Flux_t() {}

    //================================================
    Flux_t(double _a0, double _a1, double _b0, double _tmin) {
      SetParams(_a0, _a1, _b0, _tmin);
    }

    //================================================
    void SetA0(double _a0) {
      a0 = _a0;
      om2a0 = 1 - 2*a0;
    }

    //================================================
    void Settmin(double _tmin) {
      tmin = _tmin;
    }

    //================================================
    double f0(double xP) {
      return cJLAP*pow(xP, om2a0);
    }

    //================================================
    double f(double xP) {
      const double mp2 = 0.93827231*0.93827231;
      double tmax = -mp2*xP*xP/(1-xP);
      double b = b0 - 2*a1*log(xP);
      return (exp(b*tmax) - exp(b*tmin))*pow(xP, om2a0)/b;
    }

    //================================================
    void Save(FILE* df) {
      fwrite(&a0, sizeof(double), 1, df);
      fwrite(&b0, sizeof(double), 1, df);
      fwrite(&a1, sizeof(double), 1, df);
      fwrite(&tmin, sizeof(double), 1, df);
    }

    //================================================
    void Load(FILE* df) {
      fread(&a0, sizeof(double), 1, df);
      fread(&b0, sizeof(double), 1, df);
      fread(&a1, sizeof(double), 1, df);
      fread(&tmin, sizeof(double), 1, df);
      //cout <<"a0 = "<< a0 << endl;
      SetParams(a0, a1, b0, tmin);
    }

#ifndef NO_XDR
  #define XDR_R(a) if(!xdr.RWdouble(&a)) return 0;
    //==========================================================
    bool RW(XDRio_t& xdr) {
      XDR_R(a0);
      XDR_R(b0);
      XDR_R(a1);
      XDR_R(tmin);
      if(xdr.isReading()) SetParams(a0, a1, b0, tmin);
      return 1;
    }

  #undef XDR_R
#endif

};

#endif
