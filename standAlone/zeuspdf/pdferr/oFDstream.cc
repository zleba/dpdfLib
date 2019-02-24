#include "oFDstream.h"

  //========================================
  oFDstream::oFDstream(const char* fn, int prec, char cc) {
    open(fn);
    //require(good(), "Cannot open %s", fn);
    setf(ios::scientific);// | ios::internal);
    precision(precis=prec);
    CmntChar = cc;
  }
