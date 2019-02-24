/*
  Polynomial interpolation
*/

#include <cmath>
//using namespace std;
//#include "realdef.h"
typedef double real_type;
#include "emsg.h"

//#define DEF_POL_DEG 2
//---  = default polynom. degree

//====================================================================
int FindIndex(real_type xx[], int xx_size, real_type x) {
/*
    returns ind: xx[ind] < x <= xx[ind+1] when xx[0] < xx[xx_size-1]
    returns ind: xx[ind] >= x > xx[ind+1] when xx[0] > xx[xx_size-1]
    returns -1 or xx_size-1 when x outside the range
*/
	int hi = xx_size, mid, lo = -1;
	int isgr = (xx[xx_size-1] > xx[0]);
	while (hi-lo > 1) {
		mid = (hi+lo) >> 1;
		if ((x > xx[mid]) == isgr) lo = mid;
		else hi = mid;
	}
	return lo;
}

//====================================================================
int FindIndexR(real_type xx[], int xx_size, real_type x) {
/*
  extends FindIndex to accept x values slightly outside the xx range
  but only by a fraction FRAC of the first/last bin size.
*/
  const double FRAC=0.01;
  if(fabs(x-xx[0]) < FRAC*fabs(xx[1]-xx[0]) ) return 0;
  if(fabs(x-xx[xx_size-1]) < FRAC*fabs(xx[xx_size-2]-xx[xx_size-1]) ) return xx_size-2;
  return FindIndex(xx, xx_size, x);
}

//========================================================================
real_type polint(real_type xa[], real_type ya[], int pol_deg, real_type x) {
  real_type y = 0, c;
  int ii,jj;
  for(ii = 0; ii <= pol_deg; ii++){
    c = 1;
    for(jj = 0; jj <= pol_deg; jj++) if(jj != ii) c *= (x-xa[jj])/(xa[ii]-xa[jj]);
    y += ya[ii]*c;
  }
  return y;
}

//=========================================================================
real_type Ipol1(real_type x, real_type xx[], real_type yy[], int npt, int pol_deg) {
  //--- 1-dim interpolation of arbitrary degree
  int ix;
  require(npt > pol_deg, "*** Ipol1: Too little points to interpolate");
  ix = FindIndexR(xx, npt, x);
  require(ix >= 0 && (ix < npt-1),
    "*** Ipol1: x = %g out of range.", double(x));
  //if(ix==0) cout << xx[ix] << " < " << x << " <= " << xx[ix+1] << endl;
  if(ix >= npt-pol_deg) ix = npt - pol_deg-1;
  return polint(xx + ix, yy + ix, pol_deg, x);
}

//====================================================================
real_type Ipol2(real_type x, real_type y,
  real_type xx[], real_type yy[], real_type** zz, int nptx, int npty
  , int pol_degx, int pol_degy) {
  //--- 2-dim interpolation of arbitrary degree
  real_type *yap;
  real_type zb[pol_degy+1];
  int ix,iy;

  require(nptx > pol_degx, "Ipol2: Too little x points to interpolate");
  require(npty > pol_degy, "Ipol2: Too little y points to interpolate");
  ix = FindIndexR(xx, nptx, x);
  require(ix >= 0 && (ix < nptx-1), "Ipol2: x = %g out of range.", (double)x);
  if(ix >= nptx - pol_degx) ix = nptx - pol_degx -1;
  iy = FindIndexR(yy, npty, y);
  require(iy >= 0 && (iy < npty-1), "Ipol2: y = %g out of range.", (double)y);
  if(iy >= npty - pol_degy) iy = npty - pol_degy -1;

  yap = yy + iy;
  int k;
  for(k = 0; k <= pol_degx; k++) zb[k] = polint(yap, zz[ix+k]+iy, pol_degy, y);
  return polint(xx+ix, zb, pol_degx, x);
}

/*
//=======================================================================
real_type IPfunc2(real_type x, real_type y,
  real_type xx[], real_type yy[], real_type* zz[], int nptx, int npty) {
//--- linear in x, quadratic in y
real_type g[3];
int ix,iy;

  require(nptx >= 2 && npty >= 3, "*** IPfunc2: Too little points to interpolate\n")

  ix = rloc2(xx, nptx, x);
  if(ix == -1 || ix == nptx-1){
    msg(!Extrapolate, "*** IPfunc2: Function at x = %g not available!\n", double(x));
    if(ix == -1) ix++; else ix--;
  }
  //if(ix > nptx-3) ix = nptx - 3;

  iy = rloc2(yy, npty, y);
  if(iy == -1 || iy == npty-1){
    msg(!Extrapolate, "*** IPfunc2: Function at y = %g not available!\n", double(y));
    if(iy == -1) iy++; else iy--;
  }
  if(iy > npty-3) iy = npty - 3;

	int ii;
	for(ii=0; ii<3; ii++) {
		g[ii] =	zz[ix][iy+ii] +
      (zz[ix+1][iy+ii]-zz[ix][iy+ii])/(xx[ix+1]-xx[ix])*(x-xx[ix]);
	}
  return
		 g[0]*(y-yy[iy+1])/(yy[iy]-yy[iy+1])*(y-yy[iy+2])/(yy[iy]-yy[iy+2])
		+g[1]*(y-yy[iy])/(yy[iy+1]-yy[iy])*(y-yy[iy+2])/(yy[iy+1]-yy[iy+2])
		+g[2]*(y-yy[iy])/(yy[iy+2]-yy[iy])*(y-yy[iy+1])/(yy[iy+2]-yy[iy+1]);
}
*/


