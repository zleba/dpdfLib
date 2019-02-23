#include <fstream>
#include <vector>
#include <cmath>
#include <iostream>
#include <cassert>

using namespace std;


void Pars(int ifit, int ipdf, double &a0pom, double &nmes);
static double getNorm(double a0, double ap, double b0);

char fit = 'B'; 

int main()
{

    //Read it

    vector<double> xGrid(100), q2Grid; 
    if(fit == 'A') q2Grid.resize(79); //Fit A
    else           q2Grid.resize(76); //Fit B


    int nmem, ndef;

    ifstream inFile;
    if(fit == 'A') inFile.open("a.data");
    else           inFile.open("b.data");

    inFile >> nmem >> ndef;
    for(int i = 0; i < xGrid.size(); ++i) {
        inFile >> xGrid[i];
        xGrid[i] = exp(xGrid[i]); //removing log
    }
    for(int i = 0; i < q2Grid.size(); ++i)
        inFile >> q2Grid[i];

    vector<vector<vector<vector<double>>>> tab(nmem+1);
    
    for(int nm = 0; nm <= nmem; ++nm) {
        tab[nm].resize(8);
        for(int jval = 0; jval < 8; ++jval) {
            tab[nm][jval].resize(q2Grid.size());
            for(int iq = 0; iq < q2Grid.size(); ++iq) {
                tab[nm][jval][iq].resize(xGrid.size());
                for(int ix = 0; ix < xGrid.size(); ++ix)
                    inFile >> tab[nm][jval][iq][ix];
            }
        }
    }


    //int nm = 0;
    //Write it 
    for(int nm = 0; nm <= nmem; ++nm) {
        string snm = (nm < 10) ? "0"+ to_string(nm) : to_string(nm);
        ofstream outFile("../../DPDFsets/H1_DPDF_2006"+string(1,fit)+"_NLO_pom/H1_DPDF_2006"+string(1,fit)+"_NLO_pom_00"+ snm+".dat");

        outFile << "PdfType: central" << endl;
        outFile << "Format: lhagrid1" << endl;
        outFile << "FluxType: Regge" << endl;


        double a0pom, nmes;
        int ifit = (fit - 'A') + 1;
        Pars(ifit, nm, a0pom, nmes);

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
        outFile << "  -3 -2 -1  1  2  3  21" << endl;

        for(int ix = 0; ix < xGrid.size(); ++ix) {
            for(int iq = 0; iq < q2Grid.size(); ++iq) {
                //cout << tab[nm][0][iq][ix] << " ";

                double pdf[13];
                pdf[6-6] = 0.0;
                pdf[5+6] = 0.0;
                pdf[6-5] = tab[nm][7-1][iq][ix];
                pdf[5+5] = tab[nm][7-1][iq][ix];
                pdf[6-4] = tab[nm][6-1][iq][ix];
                pdf[5+4] = tab[nm][6-1][iq][ix];
                pdf[6-3] = tab[nm][5-1][iq][ix];
                pdf[5+3] = tab[nm][5-1][iq][ix];
                pdf[6-2] = tab[nm][4-1][iq][ix];
                pdf[5+2] = tab[nm][3-1][iq][ix]+tab[nm][4-1][iq][ix];                      
                pdf[6-1] = tab[nm][2-1][iq][ix];
                pdf[5+1] = tab[nm][1-1][iq][ix]+tab[nm][2-1][iq][ix];
                pdf[5+7] = tab[nm][8-1][iq][ix];

                //for(int k = 0; k < 12; ++k)
                    //outFile << pdf[k]<<" ";
                for(int k = 3; k < 3+6; ++k)
                    outFile << pdf[k]<<"  ";


                outFile <<pdf[12] << endl;

            }
        }
        outFile << "---" << endl;
        outFile.close();
    }

    return 0;
}


void Pars(int ifit, int ipdf, double &a0pom, double &nmes)
{
      if (ifit == 1) { //! fit a

      if(ipdf >= 0 && ipdf < 17) { 
        a0pom=1.11824;
        nmes=0.00169662;
      }
      else if (ipdf == 17) {
        a0pom=1.11843;
        nmes=0.00169287;
      }
      else if (ipdf == 18) {
        a0pom=1.11804;
        nmes=0.00170004;
      }
      else if (ipdf == 19) {
        a0pom=1.11687;
        nmes=0.00169226;
      }
      else if (ipdf == 20) {
        a0pom=1.11854;
        nmes=0.00168107;
      }
      else if (ipdf == 21) {
        a0pom=1.11833;
        nmes=0.0016943;
      }
      else if (ipdf == 22) {
        a0pom=1.11819;
        nmes=0.00169873;
      }
      else if (ipdf == 23) {
        a0pom=1.11936;
        nmes=0.00169188;
      }
      else if (ipdf == 24) {
        a0pom=1.11734;
        nmes=0.00170108;
      }
      else if (ipdf == 25) {
        a0pom=1.14722;
        nmes=0.00159724;
      }
      else if (ipdf == 26) {
        a0pom=1.10871;
        nmes=0.00170391;
      }
      else if (ipdf == 27) {
        a0pom=1.11546;
        nmes=0.000976152;
      }
      else if (ipdf == 28) {
        a0pom=1.12169;
        nmes=0.00299953;
      }
      else if (ipdf == 29) {
        a0pom=1.11657;
        nmes=0.00131429;
      }
      else if (ipdf == 30) {
        a0pom=1.12065;
        nmes=0.00245839;
      }
      else if (ipdf == 31) {
        a0pom=1.11691;
        nmes=0.00179416;
      }
      else if (ipdf == 32) {
        a0pom=1.11855;
        nmes=0.00165973;
      }
      else {
        cout <<"[H12006flux] ipdf out of range:',ipdf" << endl;
      }

      } else if (ifit == 2) { //! fit B

      if(ipdf >= 0 && ipdf < 15) { 
        a0pom=1.11101;
        nmes=0.00139764;
      }
      else if (ipdf == 15) {
        a0pom=1.11236;
        nmes=0.00142534;
      }
      else if (ipdf == 16) {
        a0pom=1.10965;
        nmes=0.00137153;
      }
      else if (ipdf == 17) {
        a0pom=1.10988;
        nmes=0.00140738;
      }
      else if (ipdf == 18) {
        a0pom=1.11129;
        nmes=0.00138136;
      }
      else if (ipdf == 19) {
        a0pom=1.11102;
        nmes=0.00139576;
      }
      else if (ipdf == 20) {
        a0pom=1.11101;
        nmes=0.00139999;
      }
      else if (ipdf == 21) {
        a0pom=1.11212;
        nmes=0.00139274;
      }
      else if (ipdf == 22) {
        a0pom=1.11009;
        nmes=0.00140232;
      }
      else if (ipdf == 23) {
        a0pom=1.13989;
        nmes=0.00129396;
      }
      else if (ipdf == 24) {
        a0pom=1.10147;
        nmes=0.00140518;
      }
      else if (ipdf == 25) {
        a0pom=1.10927;
        nmes=0.000811837;
      }
      else if (ipdf == 26) {
        a0pom=1.11307;
        nmes=0.00244171;
      }
      else if (ipdf == 27) {
        a0pom=1.10997;
        nmes=0.00108895;
      }
      else if (ipdf == 28) {
        a0pom=1.11245;
        nmes=0.0020084;
      }
      else if (ipdf == 29) {
        a0pom=1.10772;
        nmes=0.00134241;
      }
      else if (ipdf == 30) {
        a0pom=1.1146;
        nmes=0.00147194;
      }
    }
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
