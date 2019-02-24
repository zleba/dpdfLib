//#include <cmath>
#include "xgrid_base.h"
#include "emsg.h"

//using namespace std;

//=================================================
bool xgrid_base_t::Save(FILE* df) {
  int nb= sizeof(xgrid_base_t);
  fwrite(&nb, sizeof(int) , 1, df);
  fwrite(this, nb, 1, df);
  fwrite(x, sizeof(x[0]), nx, df);
  fwrite(d, sizeof(d[0]), nx, df);
  return OK=1;
}

//=================================================
bool xgrid_base_t::Load(FILE* df) {
  int nb;
  fread(&nb, sizeof(int) , 1, df);
  require(nb == sizeof(xgrid_base_t), "Bad sizeof(xgrid_base_t)");
  fread(this, sizeof(xgrid_base_t), 1, df);
  x = new real_type[nx];
  fread(x, sizeof(x[0]), nx, df);
  d = new real_type[nx];
  fread(d, sizeof(d[0]), nx, df);
  return OK=1;
}

#ifndef NO_XDR
//=================================================
bool xgrid_base_t::RW(XDRio_t& xdr) {
  #define XDR_R(a) if(!xdr.RWdouble(&a)) return 0;
  #define XDR_I(a) if(!xdr.RWint(&a)) return 0;

  OK = 0;
  XDR_I(nx)
  XDR_I(smode)

  XDR_R(xlo)
  XDR_R(xcrit)

  if(xdr.isReading()) {
    try {
      x = new real_type[nx];
      d = new real_type[nx];
    }
    catch(...) { emsg(1, "xgrid_t: could not allocate x, d. Bye ..."); }
  }
  if(!xdr.RWdoublev(x, nx)) return 0;
  if(!xdr.RWdoublev(d, nx)) return 0;

  return OK=1;
  #undef XDR_I
  #undef XDR_R
}

#endif
