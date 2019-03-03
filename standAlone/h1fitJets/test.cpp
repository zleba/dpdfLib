


extern "C" void strowp1_(double *X, double *SCALE, double *UPV, double *DNV, double *SEA, double *STR, double *CHM, double *GL);

#include <iostream>
using namespace std;

int main()
{
    double X = 0.1, SCALE = 10;
    double UPV, DNV, SEA, STR, CHM, GL;

    strowp1_(&X, &SCALE, &UPV, &DNV, &SEA, &STR, &CHM, &GL);
    //cout << UPV << endl;
    cout << UPV <<" " << DNV <<" "<< SEA <<" " << STR <<" "<< CHM <<" "<< GL << endl;

    return 0;
}
