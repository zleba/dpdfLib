#ifndef XGRID_BASE_H_
#define XGRID_BASE_H_

#include <cstdio>
//#include "emsg.h"
#include "realdef.h"

#ifndef NO_XDR
#include "xdr2cc.h"
#endif

//#pragma pack(push,4)
class xgrid_base_t {
  protected:
    bool OK;
  public:
    int nx, smode;
    real_type xlo, xcrit;
    real_type *x, *d;

    //enum {LIN, LOGLIN, TANH=5};
    xgrid_base_t() : OK(false), nx(0), x(0), d(0) {}
    ~xgrid_base_t() {delete[] x; delete[] d;}
    bool isOK() {return OK;}
    bool Load(FILE* df);
    bool Save(FILE* df);
#ifndef NO_XDR
    bool RW(XDRio_t& xdr);
    //bool Save(XDR* xdr) {return RW(xdr);}
    //bool Load(XDR* xdr) {return RW(xdr);}
#endif
};
//#pragma pack(pop)

#endif
