#ifndef _OFDSTREAM_H_
#define _OFDSTREAM_H_

#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
using namespace std;
#include <string.h>

//*****************************************************************
class oFDstream : public ofstream {
  //char oname[512];
  char CmntChar;
  int precis;

  public:

  //========================================
  oFDstream(const char* fn, int prec=4, char cc='#');

  //========================================
  ~oFDstream(){
    *this << "\n" << CmntChar << "EOF" <<endl;
    close();
  }

  //========================================
  void comment(const char* s) {
    *this << CmntChar << s << endl;
  }

  //========================================
  oFDstream& wr(double v) {
    *this << setw(precis+8) << v;
    return *this;
  }

  //========================================
  oFDstream& wr(long double v) {
    *this << setw(precis+8) << v;
    return *this;
  }

  //========================================
  void TimeStamp() {
    time_t czas;
    time(&czas);
    //--- ctime puts '\n' after date
    *this << CmntChar <<" " << ctime(&czas);// << flush;
  }

  //========================================
  void ColTitles(const char* tits) {
    char* ctits = new char[strlen(tits)+1];
    strcpy(ctits, tits);
    char *tp, *ctp=ctits;
    *this << CmntChar << " columns: " << ctits << endl;

    *this << CmntChar;
    int Ncol=0;
    int cw = precis+8;
    while(tp = strtok(ctp,",")) {
      Ncol++;
      width(cw - 5*(ctp != NULL));
      *this << tp;// << flush;
      ctp = NULL;
    }
    *this << endl;
    int j,k;
    *this << "#";
    for(j=1; j <= Ncol; j++) {
      width(cw - 5*(j==1));
      *this << j;
    }
    *this << endl;
  }
};

#endif
