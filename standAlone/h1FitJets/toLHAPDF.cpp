#include <fstream>
#include <vector>
#include <cmath>
#include <iostream>
#include <cassert>

using namespace std;


void Pars(int ifit, int ipdf, double &a0pom, double &nmes);
static double getNorm(double a0, double ap, double b0);


int main()
{

    //Read it

    vector<double> xGrid(100), q2Grid(88); 
    vector<vector<vector<double>>> tab;
    tab.resize(3);

    ifstream inFile;
    vector<string> files = {"h12007jetsdpdf_gluon.data", "h12007jetsdpdf_singlet.data", "h12007jetsdpdf_charm.data"};
    for(int fl = 0; fl < 3; ++fl) {
        inFile.open(files[fl]);

        for(int i = 0; i < xGrid.size(); ++i) {
            inFile >> xGrid[i];
            xGrid[i] = exp(xGrid[i]); //removing log
            //cout << i <<" "<< xGrid[i] << endl;
        }
        for(int i = 0; i < q2Grid.size(); ++i)
            inFile >> q2Grid[i];

        tab[fl].resize(q2Grid.size());
        //return 0;
        for(int iq = 0; iq < q2Grid.size(); ++iq) {
            tab[fl][iq].resize(xGrid.size());
            for(int ix = 0; ix < xGrid.size(); ++ix)
                inFile >> tab[fl][iq][ix];
        }
        inFile.close();
    }


    //Write it 

    ofstream outFile("../../DPDFsets/H1_DPDF_2007Jets_NLO_pom/H1_DPDF_2007Jets_NLO_pom_0000.dat");

    outFile << "PdfType: central" << endl;
    outFile << "Format: lhagrid1" << endl;
    outFile << "FluxType: Regge" << endl;


    double nmes = 0.0013; 
    double a0pom = 1.1037; 
    double ap = 0.06;
    double b0 = 5.5;

    double A =  getNorm(a0pom, ap, b0);

    outFile << "FluxParams: \"A = " << A << ", alpha0 = " << a0pom << ", alphaP = " << ap << ", B0 = " << b0 << "\"" << endl;
    outFile << "---" << endl << scientific;

    outFile.precision(6);

    for(auto x : xGrid)
        outFile << x<<" ";
    outFile << endl;
    for(auto q2 : q2Grid)
        outFile << sqrt(q2) <<" ";
    outFile << endl;

    //outFile << " -6 -5 -4 -3 -2 -1  1  2  3  4  5  6 21" << endl;
    outFile << " -4 -3 -2 -1  1  2  3  4  21" << endl;

    for(int ix = 0; ix < xGrid.size(); ++ix) {
        for(int iq = 0; iq < q2Grid.size(); ++iq) {
            //cout << tab[nm][0][iq][ix] << " ";

            double pdf[13];
            pdf[6-6] = 0.0;
            pdf[5+6] = 0.0;
            pdf[6-5] = 0;
            pdf[5+5] = 0;
            pdf[6-4] = 9./8. * tab[2][iq][ix]; //charm?
            pdf[5+4] = 9./8. * tab[2][iq][ix];
            pdf[6-3] = tab[1][iq][ix]/6;
            pdf[5+3] = tab[1][iq][ix]/6;
            pdf[6-2] = tab[1][iq][ix]/6;
            pdf[5+2] = tab[1][iq][ix]/6;
            pdf[6-1] = tab[1][iq][ix]/6;
            pdf[5+1] = tab[1][iq][ix]/6;
            pdf[5+7] = tab[0][iq][ix]; //gluon

            //for(int k = 0; k < 12; ++k)
            //outFile << pdf[k]<<" ";
            for(int k = 2; k < 3+7; ++k)
                outFile << abs(pdf[k])<<"  ";


            outFile <<abs(pdf[12]) << endl;

        }
    }
    outFile << "---" << endl;
    outFile.close();

    return 0;
}


//Flux int
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

static double getNorm(double a0, double ap, double b0)
{
    double tAbscutNorm = 1;
    double xPomNorm = 0.003;
    const double dm =  rfluxRawInt(a0, ap, b0, xPomNorm,  0, tAbscutNorm);
    double  norm=(1./(xPomNorm*dm)); //xpom * flux normalized to 1 at xpom = 0.003
    return norm;
}
