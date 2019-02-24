#include <iostream>
//#include <iomanip>
//#include <cstdlib>
#include "emsg.h"

using namespace std;

#include "qgrid_base.h"
  
//==============================================
void qgrid_base_t::show() {
  cout << "Q^2 grid: " << npt <<" points, Q^2 factor = "<< qqVal(tStep)
    << endl;
  cout << "Q0^2 = "<< qqVal(tval[it0]) << endl;
  printf("Fixed points at %d %d %d %d\n", it0, it_c, it_b, it_t);
  printf("%3s %10s %6s %s\n", "ind", "Q^2 ", "t  ", "Nf");
  int ind;
  for(ind = 0; ind < npt; ind++)
    printf("%3d %10.3f %6.3f %2d\n", ind, qqVal(tval[ind]), tval[ind], nFlavors[ind]);
#ifdef TEST
	cout << "Init_q: " << npt <<", "<< tlo << " - " << thi << endl;
#endif
  cout << "--------------------------------------" << endl;
}

//=================================================
bool qgrid_base_t::Save(FILE* df) {
  //FILE* df = fopen(fname, "ab");
  int nb= sizeof(qgrid_base_t);
  fwrite(&nb, sizeof(int) , 1, df);
  fwrite(this, nb, 1, df);
  fwrite(nFlavors, sizeof(nFlavors[0]), npt, df);
  fwrite(tval, sizeof(tval[0]), npt, df);
  //fclose(df);
  return 1;
}

//=================================================
bool qgrid_base_t::Load(FILE* df) {
  int nb;
  fread(&nb, sizeof(int) , 1, df);
  require(nb == sizeof(qgrid_base_t), "Bad sizeof(qgrid_base_t)");
  fread(this, sizeof(qgrid_base_t), 1, df);
  //cout << "FILE LINE: " << __FILE__ <<": "<< __LINE__ << endl;
  //cout << "npt: " << npt << hex << npt << endl;
  nFlavors = new int[npt];
  fread(nFlavors, sizeof(nFlavors[0]), npt, df);
  tval = new real_type[npt+4]; //--- + Q0 + 3 heavy flav.
  fread(tval, sizeof(tval[0]), npt, df);
  return OK=1;
}

#ifndef NO_XDR
//=================================================
bool qgrid_base_t::RW(XDRio_t& xdr) {
  #define XDR_R(a) if(!xdr.RWdouble(&a)) return 0;
  #define XDR_I(a) if(!xdr.RWint(&a)) return 0;

  OK = 0;

  XDR_I(npt)
  XDR_I(it0)
  XDR_I(it_c)
  XDR_I(it_b)
  XDR_I(it_t)

  XDR_R(QQstart)
  XDR_R(QQlo)
  XDR_R(QQhi)
  XDR_R(tStep)
  XDR_R(t0)
  XDR_R(tlo)
  XDR_R(thi)

  if(xdr.isReading()) {
    nFlavors = new int[npt];
    tval = new real_type[npt];
  }
  if(!xdr.RWintv(nFlavors, npt)) return 0;
  if(!xdr.RWdoublev(tval, npt)) return 0;

  return OK=1;
}

#endif
