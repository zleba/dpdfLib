// Program to test LHAPDF6 PDF behaviour by writing out their values at lots of x and Q points
// Note: the OpenMP directives are there as an example. In fact, in this case OpenMP slows things
// down because of the need to make the stream operations critical!


#ifndef DPDFset_H
#define DPDFset_H

#include "LHAPDF/LHAPDF.h"
#include <iostream>
#include <fstream>
#include <functional>


//using namespace LHAPDF;
//using namespace std;

class DPDF {

    public:
        DPDF(LHAPDF::PDF *pdf_);

        double 	zfzQ2xpt(int id, double z, double q2, double xp, double tAbs) const;
        double 	zfzQxpt(int id, double z, double q2, double xp, double tAbs) const;
        double 	zfzQ2xp(int id, double z, double q2, double xp, double tAbsMin, double tAbsMax) const;
        double 	zfzQxp(int id, double z, double q2, double xp, double tAbsMin, double tAbsMax) const;

        void 	zfzQ2xp (double z, double q2, double xp, double tAbsMin, double tAbsMax, std::vector<double> &zfz) const;

        double 	flux(double xp, double tAbs) const;
        double 	fluxInt(double xp, double tAbsMin, double tAbsMax) const;

        LHAPDF::PDF *getPDF() { return pdf;}

    private:
        LHAPDF::PDF *pdf;
        double norm;
        std::function<double(double,double)>         flx;    //Flux function
        std::function<double(double,double,double)>  flxInt; //Integrated flux
};

class DPDFset {

    public:
        DPDFset(std::string sn1, std::string sn2="", std::string sn3="") { init(sn1, sn2, sn3); }
        DPDFset() {}
        void init(std::string sn1, std::string sn2="", std::string sn3="");
        
        //xfxQ2(int id, double x, double q2, double xpom, double t);

        double 	zfzQ2xpt(int iMem, int id, double z, double q2, double xp, double tAbs) const;
        double 	zfzQxpt(int iMem, int id, double z, double q2, double xp, double tAbs) const;
        double 	zfzQ2xp(int iMem, int id, double z, double q2, double xp, double tAbsMin, double tAbsMax) const;
        double 	zfzQxp(int iMem, int id, double z, double q2, double xp, double tAbsMin, double tAbsMax) const;

        void 	zfzQ2xp (int iMem, double z, double q2, double xp, double tAbsMin, double tAbsMax, std::vector<double> &zfz) const;


        LHAPDF::PDF *getPDF(int iRegge, int iMem)  { return dpdfs[iRegge][iMem].getPDF(); }
        DPDF *getDPDF(int iRegge, int iMem) { return &dpdfs[iRegge][iMem]; }
        
        size_t size() const {if(dpdfs.size() > 0) return dpdfs[0].size();  else return 0;}

    private:
        std::vector<std::string> setNames;
        //Vector over regge trajectories & error eigen vectors
        std::vector<std::vector<DPDF>> dpdfs;


};

#endif
