#ifndef _GRD_HDR_BIN
#define _GRD_HDR_BIN

#define HDR_VER 200

struct GrdHeaderBin_t {
  int ver;
  int QCDClib_ver;
  char lbl[64];
  int Ndistr;
  int x_exp;
  int PiOrd;
  int reserved[11];
};
#define HDR_SIZE 128

#endif

