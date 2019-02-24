#ifndef _XDR2CC_H_
#define _XDR2CC_H_

#include <cstdio>
#include <cstdlib>

//----------------------------------------------------
class XDRio_t {
  void* xdr;
  public:
  enum opmode {W,R} dir;
  XDRio_t(FILE* f, opmode mode);
  ~XDRio_t() {if(xdr) free(xdr); xdr=NULL;}
  bool isReading() {return dir;}
  bool RWchar(char* p);
  bool RWuchar(unsigned char* p);
  bool RWint(int* p);
  bool RWdouble(double* p);
  bool RWintv(int* p, unsigned int nelem);
  bool RWdoublev(double* p, unsigned int nelem);
  bool RWcstr(char**sp, unsigned int maxlen);
};

#endif
