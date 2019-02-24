#ifndef TBLIPOL_H_
#define TBLIPOL_H_

#include "realdef.h"
int FindIndex(real_type xx[], int xx_size, real_type x);
int FindIndexR(real_type xx[], int xx_size, real_type x);
real_type Ipol1(real_type x, real_type xx[], real_type yy[], int npt, int pol_deg=2);
real_type Ipol2(real_type x, real_type y,
  real_type xx[], real_type yy[], real_type** zz, int nptx, int npty
  , int pol_degx=2, int pol_degy=2);

#endif
