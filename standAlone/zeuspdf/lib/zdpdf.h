/*
  Author: Wojtek Slominski
  Date: 2009-12-17 22:22
  Description:
    main class for ZEUS diffractive PDFs 2009.
*/

#ifndef _ZDPDF08_HDR
#define _ZDPDF08_HDR

#define _ZDPDF08_VER 210
//#define DEF_LABEL "zeusSJ"

#include <iostream>

//using namespace std;
#include "qgrid_base.h"
#include "xgrid_base.h"
//#include "realdef.h"

#ifndef NO_XDR
  #include "GrdHeader.h"
  //#define GRD_EXT ".xdr"
#else
  #include "GrdHeaderBin.h"
  //#define GRD_EXT ".bin"
#endif

#include "flux.h"

typedef real_type** Distr_t;
#define N_FLAVORS 6
#define N_PDFS 4

//-------------------------------
class zdpdf_t {
  Distr_t fi_a[N_FLAVORS+1];
  int alocNf;
  double Reggeon_factor;
  int PionOrder;
  bool GridsLoaded;
  int Verbose;
  char CommentChar;
  GrdHeader_t header;
  //FILE* GridFile;
  string Label;
  int ErrInd;

  void AllocWksp1(int n);
  void AllocWksp();
  void DeAllocWksp();
  void ClearAll();
  void ReadGrid(XDRio_t&, int pn);
  void LoadGrids();
  void LoadGridsToText();

  protected:
    Distr_t *fi;
  public:
    static int QQrelax;
    qgrid_base_t Qgrid;
    xgrid_base_t Xgrid;
    Flux_t Pflux;//(1.118, 0, 7.2, -1);
    Flux_t Rflux;//(0.55, 0.9, 2, -1);

    zdpdf_t(const string& Lbl, int ierr=0) {
      Verbose = 0;
      CommentChar = '#';
      GridsLoaded = 0;
      Label = Lbl;
      ErrInd = ierr;
      LoadGrids();
      //QQrelax = 0;
    }

    void SetOpts(const string& opt);
    void ShowInfo();
    void ClipQQ(bool y=1) {QQrelax = y;}

    real_type Pom1(real_type zP, real_type QQ, int pn, int xpow=1);
    void Pom(real_type zP, real_type QQ, real_type f[], int xpow=1);
    real_type fxP1(real_type xP, real_type zP, real_type QQ, int pn, int xpow=1) {
      return Pflux.f(xP)*Pom1(zP, QQ, pn, xpow);
    }
    void fxP(real_type xP, real_type zP, real_type QQ, real_type f[], int xpow=1);
    void pi0xf(real_type x, real_type QQ, real_type f[]);
    void fxR(real_type xP, real_type zP, real_type QQ, real_type f[], int xpow=1);
    void xDPDF3(real_type xP, real_type zP, real_type QQ, real_type f[], int xpow=1);
};

#endif
