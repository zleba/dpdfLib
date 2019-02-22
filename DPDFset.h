// Program to test LHAPDF6 PDF behaviour by writing out their values at lots of x and Q points
// Note: the OpenMP directives are there as an example. In fact, in this case OpenMP slows things
// down because of the need to make the stream operations critical!


#ifndef DPDFset_H
#define DPDFset_H

#include "LHAPDF/LHAPDF.h"
#include <iostream>
#include <fstream>
#include <functional>


using namespace LHAPDF;
using namespace std;

class DPDF {

    public:
        DPDF(PDF *pdf_);

        double 	zfzQ2xpt(int id, double z, double q2, double xp, double tAbs) const;
        double 	zfzQxpt(int id, double z, double q2, double xp, double tAbs) const;
        double 	zfzQ2xp(int id, double z, double q2, double xp, double tAbsMin, double tAbsMax) const;
        double 	zfzQxp(int id, double z, double q2, double xp, double tAbsMin, double tAbsMax) const;

    private:
        PDF *pdf;
        double norm;
        std::function<double(double,double)>         flux;    //Flux function
        std::function<double(double,double,double)>  fluxInt; //Integrated flux

};

class DPDFset {

    public:
        DPDFset(string sn1, string sn2="", string sn3="");
        
        //xfxQ2(int id, double x, double q2, double xpom, double t);

        double 	zfzQ2xpt(int iMem, int id, double z, double q2, double xp, double tAbs) const;
        double 	zfzQxpt(int iMem, int id, double z, double q2, double xp, double tAbs) const;
        double 	zfzQ2xp(int iMem, int id, double z, double q2, double xp, double tAbsMin, double tAbsMax) const;
        double 	zfzQxp(int iMem, int id, double z, double q2, double xp, double tAbsMin, double tAbsMax) const;





    private:
        vector<string> setNames;
        vector<vector<DPDF>> dpdfs;
        //const LHAPDF::PDFSet set("CT10nlo");
        //vector<PDF*> pomPDF;
        //vector<PDF*> regPDF; //posibility to have alternative reg-fluxes in sys shifts


};

#endif
