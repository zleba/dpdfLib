#include <iostream>
#include "DPDFset.h"

using namespace std;

extern "C" void h12006pdf_(double *z, double *q2, int *ifit, int *ipdf, double *xpq, double *f2, double *fl, double *c2, double *cl);
extern "C" void qcd_2006_(double *z, double *q2, int *ifit, double *XPQ, double *F2, double *FL, double *C2, double *CL);

extern "C" void  h12006fluxerr_(double *xpom, double *t, int *Int, int *ifit, int *ipdf, int *ipom, double *flux);

extern "C" void  strpriv_(double *X, double *MUF, double *xpom, double *tcut, double *XPQ); //scale muf in GeV

void check2006()
{
    //DPDFset dpdf("GKG18_DPDF_FitA_NLO_pom");
    DPDFset dpdf("H1_DPDF_2006B_NLO_pom");

    int id = 1;
    double z = 1.072267e-01, q2 = pow(2.236068e+01,2), xp = 0.01, tAbs = 0.04;
    //double z = 9.999953e-04, q2 = pow(1.322876e+00,2), xp = 0.01, tAbs = 0.04;
    //cout << dpdf.zfzQ2xpt(0, id, z, q2, xp, tAbs) << endl;



    for(int imem =0; imem < dpdf.size(); ++imem) {
        //double newVal = dpdf.getPDF(0,imem)->xfxQ2(id, z, q2);
        double newVal = dpdf.zfzQ2xpt(imem, id, z, q2, xp, 0.3);
        //H1 standAlone
        int ifit = 2, ipdf =imem;
        double xpq[13], f2[2], fl[2], c2[2], cl[2];
        h12006pdf_(&z, &q2, &ifit, &ipdf, xpq, f2, fl, c2, cl);
        //qcd_2006_(&z, &q2, &ifit, xpq, f2, fl, c2, cl);

        int Int = 0, ipom = 1;
        double t = -0.3;
        double flux;
        h12006fluxerr_(&xp, &t, &Int, &ifit, &ipdf, &ipom, &flux);
        xpq[5] *= flux;

        cout << imem<<" "<<newVal<< " "<< xpq[5] << " | " << xpq[5]/newVal << endl;
        //cout << imem<<" "<< flux / dpdf.getDPDF(0,imem)->fluxInt(xp, 0, 0.3) << endl;
    }
}


void check2007()
{
    //DPDFset dpdf("GKG18_DPDF_FitA_NLO_pom");
    DPDFset dpdf("H1_DPDF_2007Jets_NLO_pom");

    for(int id = -4; id <= 4; ++id) {
        double z = 4.037018e-02, q2 = pow(5.656854e+00,2), xp = 0.01, tAbs = 0.3;
        //double z = 9.999953e-04, q2 = pow(1.322876e+00,2), xp = 0.01, tAbs = 0.04;
        //cout << dpdf.zfzQ2xpt(0, id, z, q2, xp, tAbs) << endl;

        int imem = 0;
        //double newVal = dpdf.getPDF(0,imem)->xfxQ2(id, z, q2);
        double newVal = dpdf.zfzQ2xp(imem, id, z, q2, xp, 0, tAbs);

        //H1 standAlone
        double xpq[13];
        double q = sqrt(q2);
        double tcut = -tAbs;
        strpriv_(&z, &q, &xp, &tcut, xpq); //scale muf in GeV

        //qcd_2006_(&z, &q2, &ifit, xpq, f2, fl, c2, cl);

        cout << imem<<" "<<newVal<< " "<< xpq[6+id] << " | " << xpq[6+id]/newVal << endl;
    }
    //cout << imem<<" "<< flux / dpdf.getDPDF(0,imem)->fluxInt(xp, 0, 0.3) << endl;
}











int main()
{
    check2007();


    return 0;
    const LHAPDF::PDFSet set("GKG18_DPDF_FitA_NLO_pom");
    cout << "SetDesc: " << set.get_entry("SetDesc") << endl;
    cout << "Data version: " << set.get_entry("DataVersion") << endl;
    const LHAPDF::PDFInfo info("GKG18_DPDF_FitA_NLO_pom", 0);
    if (info.has_key("FluxParams")) cout << "FluxParams: " << info.get_entry("FluxParams") << endl;
    if (info.has_key("FluxType")) cout << "FluxType: " << info.get_entry("FluxType") << endl;

    //if (info.has_key("PdfType")) cout << "PdfType: " << info.get_entry("PdfType") << endl;
    //if (info.has_key("PdfType")) cout << "PdfType: " << info.get_entry("Format") << endl;
    //if (info.has_key("PdfType")) cout << "PomFlux: " << info.get_entry("PomFlux") << endl;


    return 0;
}
