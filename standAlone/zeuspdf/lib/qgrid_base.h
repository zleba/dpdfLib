#ifndef QGRID_BASE_H_
#define QGRID_BASE_H_

#include <cstdio>
#include <cmath>

using namespace std;

//#include "emsg.h"
#include "realdef.h"

#ifndef NO_XDR
#include "xdr2cc.h"
#endif

//#pragma pack(push,4)
class qgrid_base_t {
  protected:
    bool OK;
  public:
    int npt, it0, it_c, it_b, it_t;
    real_type QQstart, QQlo, QQhi, tStep, t0, tlo, thi;
    int *nFlavors;
    real_type *tval;

    bool isOK() {return OK;}
    real_type tVal(real_type QQ) { return log(QQ);}
    real_type qqVal(real_type t) { return exp(t);}

    qgrid_base_t() : OK(false) {
      nFlavors = NULL;
      tval =NULL;
    }
    void show();
    int nIniFlavors() {return nFlavors[it0];}
    real_type getQQ(int iq) {return qqVal(tval[iq]);}
    bool Save(FILE* df);
    bool Load(FILE* df);
#ifndef NO_XDR
    bool RW(XDRio_t& xdr);
#endif

};
//#pragma pack(pop)

#endif
