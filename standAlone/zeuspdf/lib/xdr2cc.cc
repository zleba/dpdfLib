#include "xdr2cc.h"

#ifdef __cplusplus
#include <cstdio>
//#include <string>
extern "C" {
#else
  #include <stdio.h>
#endif

#include <rpc/types.h>
#include <rpc/xdr.h>

#ifdef __cplusplus
}
#endif

#define XDR_PTR ((XDR*)xdr)

//===================================
XDRio_t::XDRio_t(FILE* f, opmode mode) {
  xdr = NULL;
  xdr = malloc(sizeof(XDR));
  if(!xdr || !f) throw "XDRio_t creation failed";
  dir = mode;
  xdrstdio_create(XDR_PTR, f, (xdr_op)mode);
}

//===================================
bool XDRio_t::RWchar(char* p) {return xdr_char(XDR_PTR,p);}
bool XDRio_t::RWuchar(unsigned char* p) {return xdr_u_char(XDR_PTR,p);}
bool XDRio_t::RWint(int* p) {return xdr_int(XDR_PTR,p);}
bool XDRio_t::RWdouble(double* p) {return xdr_double(XDR_PTR,p);}
bool XDRio_t::RWcstr(caddr_t *p, u_int maxlen) {return xdr_string(XDR_PTR,p,maxlen);}

bool XDRio_t::RWintv(int* p, unsigned int nelem) {
  int k;
  for(k=0; k < nelem; k++,p++) if(!xdr_int(XDR_PTR,p)) return 0;
  return 1;
}

bool XDRio_t::RWdoublev(double* p, unsigned int nelem) {
  int k;
  for(k=0; k < nelem; k++,p++) if(!xdr_double(XDR_PTR,p)) return 0;
  return 1;
}
