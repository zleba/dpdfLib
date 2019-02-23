#include <iostream>
#include "DPDFset.h"

using namespace std;

int main()
{

    //DPDFset dpdf("GKG18_DPDF_FitA_NLO_pom");
    DPDFset dpdf("H1_DPDF_2006A_NLO_pom");

    int id = 0;
    double z = 0.1, q2 = 12, xp = 0.01, tAbs = 0.04;
    cout << dpdf.zfzQ2xpt(0, id, z, q2, xp, tAbs) << endl;

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
