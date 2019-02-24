#include <cstdio>
#include "zdpdf.h"
#include "emsg.h"
#include <fstream>

#define LG_ERR_MSG "LoadGrid error"

#ifndef NO_XDR
#include "xdr2cc.h"
#define XDR_R(a) require(xdr.RWdouble(&a),LG_ERR_MSG);
#define XDR_I(a) require(xdr.RWint(&a), LG_ERR_MSG);
//#define GRD_EXT ".xdr"

using namespace std;

static double DBLstamp=1234567/4.0;
static int INTstamp=12345;
static char* Magic="XDRG";

//==========================================================
void zdpdf_t::ReadGrid(XDRio_t& xdr, int pn) {
  int iq;
  double x;
  //cout << "pn = "<< pn << endl;
  XDR_I(iq)
  require(iq==pn, "Qgrid read: bad parton index");
  for(iq = 0; iq < Qgrid.npt; iq++) {
    //cout << iq << endl;
    //cout << Xgrid.nx << endl;
    require(xdr.RWdoublev(fi[pn][iq], Xgrid.nx), LG_ERR_MSG);
    //cout << fi[pn][iq] << endl;
    //cout << fi[pn][iq][0] << endl;
  }
  XDR_R(x)
  XDR_I(iq)
  //require(iq==INTstamp && fabs(x-DBLstamp)<1e-10, "LoadGrids: chk failed.");
  require(iq==INTstamp && x==DBLstamp, "LoadGrid: check failed.");
}

//==========================================================
void zdpdf_t::LoadGridsToText() {
  require(sizeof(real_type) == sizeof(double), "real_type must be double");

  int ErrBack = ErrInd;
  for(ErrInd = -0; ErrInd <= 0; ++ErrInd) { 
  
  string gname(Label);
  if(ErrInd) {
    char ts[8];
    if(ErrInd > 0) sprintf(ts, "_P%02d", ErrInd);
    else sprintf(ts, "_M%02d", -ErrInd);
    gname += ts;
  }
  gname += ".xdr";
  const char* fn = gname.c_str();
  //puts("====> Reading...");
  FILE* gf=fopen(fn, "rb");
  require(gf, "Cannot open '%s'.", fn);
  char mg[4];
  fread(mg, 4, 1, gf);
  require(!memcmp(mg, Magic, 4),"Bad grid file.");

  //require(XDRcreate(&xdrs, gf, XDR_DECODE), "XDR creation failed.");
  XDRio_t xdr(gf, XDRio_t::R);
  require(header.RW(xdr), "Header read failed");
  if(Verbose) printf("Grid version %d.%d\nGrid label: '%s'\n", header.Ver, header.SubVer, header.lbl);
  //cout << "Grid label: " << header.lbl << endl;
  require(Pflux.RW(xdr), LG_ERR_MSG);
  require(Rflux.RW(xdr), LG_ERR_MSG);
  XDR_R(Reggeon_factor)
  require(Qgrid.RW(xdr), "Qgrid read failed.");
  if(Verbose) Qgrid.show();
  require(Xgrid.RW(xdr), "Xgrid read failed.");
  int pn, iq;
  //cout <<"Ndistr  "<< header.Ndistr << endl;
  //cout << "Qgrid.npt " << Qgrid.npt << endl;
  AllocWksp();
  ReadGrid(xdr, 0);
  ReadGrid(xdr, 1);
  ReadGrid(xdr, 4);
  ReadGrid(xdr, 5);

  fclose(gf);
  GridsLoaded = true;
  if(Verbose) cout << "Grids loaded." << endl;

  int fId =  2 * abs(ErrInd) - (ErrInd > 0);

  char fName[500];
  sprintf(fName, "../../../DPDFsets/ZEUS_DPDF_2009SJ_NLO_pom/ZEUS_DPDF_2009SJ_NLO_pom_00%02d.dat", fId);
  cout << fName << endl;
  ofstream outFile(fName);

  if(fId == 0)
      outFile << "PdfType: central" << endl;
  else
      outFile << "PdfType: error" << endl;
  outFile << "Format: lhagrid1" << endl;
  outFile << "FluxType: Regge" << endl;
  double A = 1, a0pom = Pflux.a0, ap = Pflux.a1, b0 = Pflux.b0;
  outFile << "FluxParams: \"A = " << A << ", alpha0 = " << a0pom << ", alphaP = " << ap << ", B0 = " << b0 << "\"" << endl;
  outFile << "---" << endl << scientific;
  outFile.precision(6);
  
  //for(int fl = 3; fl <= 5; ++fl) {

      for(int i = 0; i < Xgrid.nx; ++i) {
         outFile << Xgrid.x[i] << " ";
         if(!ErrInd) cout <<"x: "<< i <<" "<< Xgrid.x[i] <<   endl;
      }
      outFile << endl;
      for(int i = 0; i < Qgrid.npt; ++i) {
         //cout << sqrt(exp(Qgrid.tval[i])) << " "<< Qgrid.nFlavors[i]  << endl;
         //if(Qgrid.nFlavors[i] == fl || (i > 0 && Qgrid.nFlavors[i-1] == fl))
             outFile << sqrt(exp(Qgrid.tval[i])) <<  " ";
         if(!ErrInd) cout <<"t: "<< i <<" "<< exp(Qgrid.tval[i]) <<" "<< Qgrid.nFlavors[i] <<  endl;
      }
      //exit(0);
      outFile << endl;
      outFile << " -5 -4 -3 -2 -1  1  2  3  4  5  21" << endl;

      for(int iq = 0; iq < Qgrid.npt; ++iq) {
         //if(Qgrid.nFlavors[iq] == fl || (iq > 0 && Qgrid.nFlavors[iq-1] == fl)) {
            for(int ix = 0; ix < Xgrid.nx; ++ix) {
               //cout << iq << " "<< ix << endl;
               //cout << exp(Qgrid.tval[iq]) <<" "<< Xgrid.x[ix] <<" : " <<  fi[0][iq][ix] << " "<<  fi[1][iq][ix] <<" " << fi[4][iq][ix] <<" "<<  fi[5][iq][ix] <<  endl;

               double g = fi[0][iq][ix] / (2*M_PI);
               double q = fi[1][iq][ix] / (2*M_PI);
               double c = Qgrid.nFlavors[iq] >= 4 ? fi[4][iq][ix]/(2*M_PI) : 0;
               double b = Qgrid.nFlavors[iq] >= 5 ? fi[5][iq][ix]/(2*M_PI) : 0;

               outFile << b <<" "<< c <<" "<< q <<" "<< q <<" "<< q<<" "<<q <<" "<< q <<" "<< q << " "<< c <<" "<< b <<" "<< g << endl;

            }
         //}
      }
      outFile << "---" << endl;
  //}

  outFile.close();

  if(!ErrInd) cout << "Pom " << Pflux.a0<< " "<< Pflux.a1 <<" "<< Pflux.b0 << endl;
  if(!ErrInd) cout << "Reg " << Rflux.a0<< " "<< Rflux.a1 <<" "<< Rflux.b0 << endl;
  if(!ErrInd) cout << "Reg factor " << Reggeon_factor << endl;
  if(!ErrInd) cout << "Radek End of loading of " << fn << endl;

  int iiq = 11;
  int iix = 218;
  double g = fi[0][iiq][iix] / (2*M_PI);
  double q = fi[1][iiq][iix] / (2*M_PI);
  double c = fi[4][iiq][iix] / (2*M_PI);
  double b = fi[5][iiq][iix] / (2*M_PI);
  if(!ErrInd) cout << "Helenka " << g <<" "<< q <<" "<< c << " "<< b << endl;

  }



  //exit(0);



  //Calc.x_exp = header.x_exp; //--- ??? pdfs are PDFs
  PionOrder = header.PiOrd;
  ErrInd = ErrBack;
}




//==========================================================
void zdpdf_t::LoadGrids() {

  LoadGridsToText();

  require(sizeof(real_type) == sizeof(double), "real_type must be double");

  string gname(Label);
  if(ErrInd) {
    char ts[8];
    if(ErrInd > 0) sprintf(ts, "_P%02d", ErrInd);
    else sprintf(ts, "_M%02d", -ErrInd);
    gname += ts;
  }
  gname += ".xdr";
  const char* fn = gname.c_str();
  //puts("====> Reading...");
  FILE* gf=fopen(fn, "rb");
  require(gf, "Cannot open '%s'.", fn);
  char mg[4];
  fread(mg, 4, 1, gf);
  require(!memcmp(mg, Magic, 4),"Bad grid file.");

  //require(XDRcreate(&xdrs, gf, XDR_DECODE), "XDR creation failed.");
  XDRio_t xdr(gf, XDRio_t::R);
  require(header.RW(xdr), "Header read failed");
  if(Verbose) printf("Grid version %d.%d\nGrid label: '%s'\n", header.Ver, header.SubVer, header.lbl);
  //cout << "Grid label: " << header.lbl << endl;
  require(Pflux.RW(xdr), LG_ERR_MSG);
  require(Rflux.RW(xdr), LG_ERR_MSG);
  XDR_R(Reggeon_factor)
  require(Qgrid.RW(xdr), "Qgrid read failed.");
  if(Verbose) Qgrid.show();
  require(Xgrid.RW(xdr), "Xgrid read failed.");
  int pn, iq;
  //cout <<"Ndistr  "<< header.Ndistr << endl;
  //cout << "Qgrid.npt " << Qgrid.npt << endl;
  AllocWksp();
  ReadGrid(xdr, 0);
  ReadGrid(xdr, 1);
  ReadGrid(xdr, 4);
  ReadGrid(xdr, 5);

  fclose(gf);
  GridsLoaded = true;
  if(Verbose) cout << "Grids loaded." << endl;


  //Calc.x_exp = header.x_exp; //--- ??? pdfs are PDFs
  PionOrder = header.PiOrd;
}

#else

//==========================================================
void zdpdf_t::ReadGrid(int pn) {
  int iq;
  double x;
  //cout << "pn = "<< pn << endl;
  for(iq = 0; iq < Qgrid.npt; iq++) {
    //cout << iq << endl;
    //cout << Pdf->fi[pn][iq] << endl;
    //cout << Pdf->fi[pn][iq][0] << endl;
    //cout << Xgrid.nx << endl;
    fread(fi[pn][iq], sizeof(real_type), Xgrid.nx, GridFile);
  }
  fread(&x, sizeof(double), 1, GridFile);
  fread(&iq, sizeof(int), 1, GridFile);
  require(iq==12345 && fabs(x-1.23456)<1e-10, "LoadGrids: chk failed.");
}

//==========================================================
void zdpdf_t::LoadGrids(const char* fn) {
  GridFile = fopen(fn,"rb");
  require(GridFile, "Cannot open %s", fn);
  cout << "Loading grids from: " << fn << endl;
  fread(&header, sizeof(header), 1, GridFile);
  cout << "Grid label: " << header.lbl << endl;
  Pflux.Load(GridFile);
  Rflux.Load(GridFile);
  fread(&Reggeon_factor, sizeof(double), 1, GridFile);
  #ifdef Z_TEST_
    cout << "Reggeon_factor "<< Reggeon_factor <<endl;
  #endif
  if(header.ver > 100) PionOrder = header.PiOrd;
  //if(header.ver > 100) fread(&PionOrder, sizeof(int), 1, GridFile);
  Qgrid.Load(GridFile);
  #ifdef Z_TEST_
    Qgrid.show();
    cout << "ofs: "<< hex << ftell(GridFile) <<dec <<endl;
    cout << "sizeof(Xgrid): "<< sizeof(Xgrid) <<endl;
  #endif
  Xgrid.Load(GridFile);
  #ifdef Z_TEST_
    cout << "nx "<< Xgrid.nx <<endl;
    cout << "xlo "<< Xgrid.xlo <<endl;
    cout << "header.Ndistr "<< header.Ndistr <<endl;
    cout << "ofs: "<< hex << ftell(GridFile) <<dec <<endl;
  #endif
  AllocWksp();
  int pn, iq;
  if(header.ver < 200) {
    for(pn=0; pn < header.Ndistr; pn++)
      for(iq = 0; iq < Qgrid.npt; iq++) {
      #ifdef Z_TEST_
        double v;
        fread(&v, sizeof(real_type), 1, GridFile);
        cout << pn <<" "<< iq <<" "<< hex << ftell(GridFile) <<dec <<"  "<< v << endl;
        fread(fi_a[pn][iq]+1, sizeof(real_type), Xgrid.nx-1, GridFile);
        cout << pn <<" "<< iq <<" "<< fi_a[pn][iq][1] << endl;
      #else
        fread(fi_a[pn][iq], sizeof(real_type), Xgrid.nx, GridFile);
      #endif
      }
    double x;
    fread(&x, sizeof(double), 1, GridFile);
    fread(&iq, sizeof(int), 1, GridFile);
    require(iq==12345 && fabs(x-M_PI)<1e-10, "LoadGrids: chk failed.");
  } else {
    ReadGrid(0);
    ReadGrid(1);
    ReadGrid(4);
    ReadGrid(5);
  }
  fclose(GridFile);
  GridsLoaded = true;
}
#endif
