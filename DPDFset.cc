// Program to test LHAPDF6 PDF behaviour by writing out their values at lots of x and Q points
// Note: the OpenMP directives are there as an example. In fact, in this case OpenMP slows things
// down because of the need to make the stream operations critical!

#include "LHAPDF/LHAPDF.h"
#include <iostream>
#include <fstream>
#include "DPDFset.h"
#include <string>
#include <sstream>

using namespace LHAPDF;
using namespace std;


DPDFset::DPDFset(string sn1, string sn2, string sn3)
{
    setNames = {sn1, sn2, sn3};
    for(int i = 0; i < setNames.size(); ++i)
        if(setNames[i] == "") {
            setNames.resize(i);
            break;
        }

    for(auto sn : setNames) {
        auto pdfs  = mkPDFs(sn);
        vector<DPDF> dpdfNow;
        for(auto &pdf : pdfs)
            dpdfNow.push_back(DPDF(pdf));
        dpdfs.push_back(dpdfNow);
    }

    //Check that all sizes are equal
    for(auto dpdf : dpdfs)
        if(dpdf.size() != dpdfs[0].size()) {
            cout << "DPDFset not compatible" << endl;
            exit(1);
        }
}

/*
string trim(string s)
{
    int f, l;
    for(f = 0; f < s.size(); ++f)
        if(s[f] != ' ' || s[f] != '\t')
            break;
    for(l = s.size()-1; l >= 0; --l)
        if(s[l] != ' ' || s[l] != '\t')
            break;
    return s.substr(f, l-f);
}
*/

map<string,double> string2map(string str)
{
    vector<string> items;
    stringstream sstr(str);
    while( sstr.good() ) {
        string substr;
        getline(sstr, substr, ',' );
        items.push_back( substr );
    }

    map<string,double> pars;
    for(auto s : items) {
        unsigned eq = s.find('=');
        string val = trim(s.substr(eq+1));
        string tag = trim(s.substr(0, eq));
        pars[tag] = stod(val);
    }
    return pars;
}



double 	DPDF::zfzQ2xpt(int id, double z, double q2, double xp, double tAbs) const
{
    return norm * flux(xp, tAbs) * pdf->xfxQ2(id, z, q2);
}

double 	DPDF::zfzQxpt(int id, double z, double q, double xp, double tAbs) const
{
    return norm * flux(xp, tAbs) * pdf->xfxQ(id, z, q);
}

double 	DPDF::zfzQ2xp(int id, double z, double q2, double xp, double tAbsMin, double tAbsMax) const
{
    return norm * fluxInt(xp, tAbsMin, tAbsMax) * pdf->xfxQ2(id, z, q2);
}

double 	DPDF::zfzQxp(int id, double z, double q, double xp, double tAbsMin, double tAbsMax) const
{
    return norm * fluxInt(xp, tAbsMin, tAbsMax) * pdf->xfxQ(id, z, q);
}


double 	DPDFset::zfzQ2xpt(int iMem, int id, double z, double q2, double xp, double tAbs) const
{
    double pdfVal = 0;
    for(int i = 0; i < dpdfs.size(); ++i)
        pdfVal += dpdfs[i][iMem].zfzQ2xpt(id, z, q2, xp, tAbs);
    return pdfVal;
}

double 	DPDFset::zfzQxpt(int iMem, int id, double z, double q, double xp, double tAbs) const
{
    double pdfVal = 0;
    for(int i = 0; i < dpdfs.size(); ++i)
        pdfVal += dpdfs[i][iMem].zfzQxpt(id, z, q, xp, tAbs);
    return pdfVal;
}

double 	DPDFset::zfzQ2xp(int iMem, int id, double z, double q2, double xp, double tAbsMin, double tAbsMax) const
{
    double pdfVal = 0;
    for(int i = 0; i < dpdfs.size(); ++i)
        pdfVal += dpdfs[i][iMem].zfzQ2xp(id, z, q2, xp, tAbsMin, tAbsMax);
    return pdfVal;
}

double 	DPDFset::zfzQxp(int iMem, int id, double z, double q, double xp, double tAbsMin, double tAbsMax) const
{
    double pdfVal = 0;
    for(int i = 0; i < dpdfs.size(); ++i)
        pdfVal += dpdfs[i][iMem].zfzQxp(id, z, q, xp, tAbsMin, tAbsMax);
    return pdfVal;
}











static double rfluxRawInt(double a0, double ap, double b0,  double x_pom, double tAbsMin, double tAbsMax);
static double rfluxRaw(double a0, double ap, double b0, double x_pom, double tAbs);



DPDF::DPDF(PDF *pdf_)
{
    pdf = pdf_;

    //const LHAPDF::PDFInfo info = pdf->info().get_entry("FluxType");
    if(!pdf->info().has_key("FluxType")) {
        cout << "FluxType is missing for " << pdf->description () << endl;
        exit(1);
    }
    string fluxType = pdf->info().get_entry("FluxType");

    if(!pdf->info().has_key("FluxParams")) {
        cout << "FluxParams is missing for " << pdf->description () << endl;
        exit(1);
    }
    string fluxParams = pdf->info().get_entry("FluxParams");

    if(pdf->info().has_key("Norm")) {
        norm = stod(pdf->info().get_entry("Norm"));
    }
    else {
        norm = 1;
    }


    cout << fluxType << " " << fluxParams << endl;
    map<string,double> pars = string2map(fluxParams);

    //Insert flux
    //H1 like flux factor
    if(fluxType == "ReggeH1") {
        double a0 = pars.at("alpha0");
        double ap = pars.at("alphaP");
        double b0 = pars.at("B");

        double tAbscutNorm = 1;
        double xPomNorm = 0.003;
        const double dm =  rfluxRawInt(a0, ap, b0, xPomNorm,  0, tAbscutNorm);
        double  norm=(1./(xPomNorm*dm)); //xpom * flux normalized to 1 at xpom = 0.003

        flux    = [=](double xPom, double tAbs) {return  norm * rfluxRaw(a0, ap, b0, xPom, tAbs);};
        fluxInt = [=](double xPom, double tAbsMin, double tAbsMax) {return  norm * rfluxRawInt(a0, ap, b0, xPom, tAbsMin, tAbsMax);};
    }
    //General Regge-like flux
    else if(fluxType == "Regge") {
        double A  = pars.at("A");
        double a0 = pars.at("alpha0");
        double ap = pars.at("alphaP");
        double b0 = pars.at("B");

        flux    = [=](double xPom, double tAbs) {return  A * rfluxRaw(a0, ap, b0, xPom, tAbs);};
        fluxInt = [=](double xPom, double tAbsMin, double tAbsMax) {return  A * rfluxRawInt(a0, ap, b0, xPom, tAbsMin, tAbsMax);};
    }
    else {
        cout << "Unknown FluxType : " << fluxType << endl;
        exit(1);
    }
}


static double rfluxRawInt(double a0, double ap, double b0,  double x_pom, double tAbsMin, double tAbsMax)
{
    const double mp = 0.93827231;

    //     calc min. kinematically  allowed t
    double tAbsMinKin = pow(mp*x_pom,2)/(1.-x_pom);
    tAbsMin = max(tAbsMin, tAbsMinKin);
    assert(tAbsMin < tAbsMax);

    //     c*xpom**(-(2apom-1))
    double fl =  exp((2.0*a0-1.)*log(1.0/x_pom));
    double b=(b0+2.0*ap*log(1.0/x_pom));

    //   at fixed t:  exp(Bt)
    //  fl = fl * exp(b*tcut);

    //   t-integrated: (1/B)*[exp(-B*tmax)-exp(-B*tmin)]
    fl = fl * (exp(-tAbsMin*b)-exp(-tAbsMax*b))/b;

    return fl;
}


static double rfluxRaw(double a0, double ap, double b0, double x_pom, double tAbs)
{
    //     c*xpom**(-(2apom-1))
    double fl =  exp((2.0*a0-1.)*log(1.0/x_pom));
    double b=(b0+2.0*ap*log(1.0/x_pom));

    //   at fixed t:  exp(Bt)
    //  fl = fl * exp(b*tcut);
    fl = fl * exp(-b*tAbs);

    return fl;
}


double rfluxInt(double a0, double ap, double b0, double x_pom, double tAbsMin, double tAbsMax)
{
    double tAbscutNorm = 1;
    double xPomNorm = 0.003;
    const double dm =  rfluxRawInt(a0, ap, b0, xPomNorm,  0, tAbscutNorm);
    double  norm=(1./(xPomNorm*dm)); //xpom * flux normalized to 1 at xpom = 0.003

    return  norm * rfluxRawInt(a0, ap, b0, x_pom, tAbsMin, tAbsMax);
}


double rflux(double a0, double ap, double b0, double x_pom, double tAbs)
{
    double tAbscutNorm = 1;
    double xPomNorm = 0.003;
    const double dm =  rfluxRawInt(a0, ap, b0, xPomNorm,  0, tAbscutNorm);
    double  norm=(1./(xPomNorm*dm)); //xpom * flux normalized to 1 at xpom = 0.003

    return  norm * rfluxRaw(a0, ap, b0, x_pom, tAbs);
}
