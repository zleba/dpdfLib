

      SUBROUTINE STRPRIV(X,MUF, xpom, tcut, XPQ)
c ******************************************************************   
c   This routine provides the interface to the H12007JetsDPDF
c   Access through PDFLIB private structure function
c   Need common block with xpomeron, delta_xpomeron (see code below)
c
c INPUT:
c                     X   = x-value of parton   
c                     MUF = QCD scale in GeV
c
c OUTPUT:       UPV,DNV,USEA,DSEA,STR,CHM,BOT,TOP,GL
c
c
c F.-P. Schilling, S. Schaetzel, M.Mozer, K. Cerny
c ******************************************************************   

      IMPLICIT NONE

      DOUBLE PRECISION X,MUF, tcut       
      DOUBLE PRECISION UPV,DNV,USEA,DSEA,STR,CHM,BOT,TOP,GL 

      real rx,rmuf,rxpom
      

      double precision BetaGluonPom,BetaSingletPom
      double precision BetaGluonReg,BetaSingletReg

      integer i
      integer dpdf,dpdfset,dpdffit,dpdfvar,dpdfopt,idpdf
      logical first
      integer icycle      
      real g,s,c,err
      real fluxP,fluxR

      real h12007jetsdpdf_gluon,
     &     h12007jetsdpdf_singlet,
     &     h12007jetsdpdf_charm
      external h12007jetsdpdf_gluon,
     &         h12007jetsdpdf_singlet,
     &         h12007jetsdpdf_charm
      real h12007jetsdpdf_prcoeff
      external h12007jetsdpdf_prcoeff
      integer ityp

c      logical first
c      data first /.true./
c      save first


      Double Precision xpq
      DIMENSION XPQ(-6:6)


c====================================================================
c common block: xpomeron (=xpom), delta_xpomeron (=scal)
c               xpom and xpom bin-width to scale the x-section
c====================================================================
c fill from steering
      Double Precision scal,xpom
c      common/adster/scal,xpom
      data icycle /0/
      save icycle,ityp


c========= for pdflib Owens pion pdf===================
      DOUBLE PRECISION SEA      
c======================================================



      GL   = 0.d0
      DNV  = 0.d0
      UPV  = 0.d0       
      DSEA = 0.d0
      USEA = 0.d0
      STR  = 0.d0
      CHM  = 0.d0
      BOT  = 0.d0
      TOP  = 0.d0

      DO I=-6,6
         XPQ(I)=0.D0
      ENDDO

      rx=x
      rxpom=xpom
      rmuf=muf


c===== Pomeron or Reggeon ======
       ityp = 1 ! Pomeron
c      ityp = 2 ! Reggeon
c================================      



      if (first) then
        first=.false.
        icycle=0
          g=h12007jetsdpdf_gluon  (x,muf*muf,icycle)
          s=h12007jetsdpdf_singlet(x,muf*muf,icycle)
          c=h12007jetsdpdf_charm  (x,muf*muf,'',icycle) 
      endif



c================================
c      get IP / IR-Flux
c================================
      ityp = 1
      fluxP=h12007jetsdpdf_prcoeff(ityp,rxpom,tcut)
      ityp = 2
      fluxR=h12007jetsdpdf_prcoeff(ityp,rxpom,tcut)



c=============================================
c       multiply flux by pdfs
c=============================================
                         !===============!
c      IF(ityp.eq.1) THEN !    Pomeron 
                         !===============!
        g=h12007jetsdpdf_gluon  (x,muf*muf,icycle)
        s=h12007jetsdpdf_singlet(x,muf*muf,icycle)
        c=h12007jetsdpdf_charm  (x,muf*muf,'',icycle) 

        xpq(0)=g      *fluxP      ! correct gluon desnisty for flux
        xpq(1)=s/6.d0 *fluxP      !
        xpq(2)=s/6.d0 *fluxP      !  singlet -> quark-density
        xpq(3)=s/6.d0 *fluxP      ! 
        xpq(4)=c*9.d0/8.d0 *fluxP ! structurfn -> density

        do i=-6,6
           xpq(i)=xpq(i)
        enddo

        UPV  = 0
        DNV  = 0
        USEA = XPQ(1)
        DSEA = XPQ(2)
        STR  = XPQ(3)
        CHM  = XPQ(4)
        BOT  = XPQ(5)
        TOP  = XPQ(6)
        GL   = XPQ(0)
        XPQ(-1) = XPQ(1)
        XPQ(-2) = XPQ(2)
        XPQ(-3) = XPQ(3)
        XPQ(-4) = XPQ(4)
                               !====================================!
c      ELSE IF(ityp.eq.2) THEN  !   Reggeon a`la PDFLIB Owens(2,1,1)
                               !====================================!
        !write(*,*) 'OWENS - X,MUF2,flux,scal',X,MUF**2,flux,scal       
        CALL STROWP1(X,MUF,UPV,DNV,SEA,STR,CHM,GL) ! routine from PDFLIB
c        UPV  = UPV * fluxR
c        DNV  = DNV * fluxR
c        USEA = SEA * fluxR
c        DSEA = SEA * fluxR
c        STR  = STR * fluxR
c        CHM  = CHM * fluxR
c        BOT  = 0
c        TOP  = 0
c        GL   = GL  * fluxR

C Adding reggeon
c       XPQ(0) = XPQ(0)+GL*fluxR
c       XPQ(-1)= XPQ(-1)+SEA*fluxR
c       XPQ(-2)= XPQ(-2)+SEA*fluxR
c       XPQ(1)= XPQ(-1)+(SEA+DNV)*fluxR
c       XPQ(2)= XPQ(-2)+(SEA+UPV)*fluxR

c       XPQ(-3)= XPQ(-3)+STR*fluxR
c       XPQ(3) = XPQ(3)+STR*fluxR
c       XPQ(4) = XPQ(4)+CHM*fluxR
c       XPQ(-4)= XPQ(-4)+CHM*fluxR
      
c      ENDIF 


      RETURN
      END



#if 1

c======================================================================
c
c subroutine STROWP1(X,SCALE,UPV,DNV,SEA,STR,CHM,GL) 
c taken from PDFLIB provides pion structure function acc.
c to Owens Set 1 ... (type=2, group=1, set=1)
c
c=====================================================================


c======================================================
c#include "pdf/pilot.h" ! include upacked below
#if 1
#endif
#if !defined(CERNLIB_SINGLE)
#ifndef CERNLIB_DOUBLE
#define CERNLIB_DOUBLE
#endif
#endif
c======================================================

      SUBROUTINE STROWP1(X,SCALE,UPV,DNV,SEA,STR,CHM,GL)
C :::::::::  OWENS SET 1 PION STRUCTURE FUNCTION  :::::::::

c#include "pdf/impdp.inc" ! unpacked below ============
#if defined(CERNLIB_DOUBLE)
      IMPLICIT DOUBLE PRECISION(A-H,O-Z)
#endif
#if defined(CERNLIB_SINGLE)
C     IMPLICIT DOUBLE PRECISION(A-H,O-Z)
#endif
c======================================================

#if defined(CERNLIB_DOUBLE)
      DOUBLE PRECISION DGAMMA
#endif


c#include "pdf/expdp.inc"! unpacked below =============
#if defined(CERNLIB_DOUBLE)
      DOUBLE PRECISION
#endif
#if defined(CERNLIB_SINGLE)
      REAL
#endif
c======================================================
     +       COW(3,5,4),TS(6),XQ(9)


C...Expansion coefficients for up and down valence quark distributions.
      DATA ((COW(IP,IS,1),IS=1,5),IP=1,3)/
     1  4.0000D-01,  7.0000D-01,  0.0000D+00,  0.0000D+00,  0.0000D+00,
     2 -6.2120D-02,  6.4780D-01,  0.0000D+00,  0.0000D+00,  0.0000D+00,
     3 -7.1090D-03,  1.3350D-02,  0.0000D+00,  0.0000D+00,  0.0000D+00/
C...Expansion coefficients for gluon distribution.
      DATA ((COW(IP,IS,2),IS=1,5),IP=1,3)/
     1  8.8800D-01,  0.0000D+00,  3.1100D+00,  6.0000D+00,  0.0000D+00,
     2 -1.8020D+00, -1.5760D+00, -1.3170D-01,  2.8010D+00, -1.7280D+01,
     3  1.8120D+00,  1.2000D+00,  5.0680D-01, -1.2160D+01,  2.0490D+01/
C...Expansion coefficients for (up+down+strange) quark sea distribution.
      DATA ((COW(IP,IS,3),IS=1,5),IP=1,3)/
     1  9.0000D-01,  0.0000D+00,  5.0000D+00,  0.0000D+00,  0.0000D+00,
     2 -2.4280D-01, -2.1200D-01,  8.6730D-01,  1.2660D+00,  2.3820D+00,
     3  1.3860D-01,  3.6710D-03,  4.7470D-02, -2.2150D+00,  3.4820D-01/
C...Expansion coefficients for charm quark sea distribution.
      DATA ((COW(IP,IS,4),IS=1,5),IP=1,3)/
     1  0.0000D+00, -2.2120D-02,  2.8940D+00,  0.0000D+00,  0.0000D+00,
     2  7.9280D-02, -3.7850D-01,  9.4330D+00,  5.2480D+00,  8.3880D+00,
     3 -6.1340D-02, -1.0880D-01, -1.0852D+01, -7.1870D+00, -1.1610D+01/

       DATA ZEROD/0.D0/, ONED/1.D0/, SIXD/6.D0/
       DATA ALAM/0.2D0/, Q02/4.D0/, QMAX2/2.D3/
C...Pion structure functions from Owens.
C...Allowed variable range: 4 GeV^2 < Q^2 < approx 2000 GeV^2.

C...Determine set, Lambda and s expansion variable.
        Q2 = SCALE*SCALE
        Q2IN = MIN( QMAX2,MAX( Q02,Q2))
        SD = LOG( LOG( Q2IN/ALAM**2)/ LOG( Q02/ALAM**2))

C...Calculate structure functions.
        DO 240 KFL=1,4
        DO 230 IS=1,5
  230   TS(IS)=COW(1,IS,KFL)+COW(2,IS,KFL)*SD+
     &  COW(3,IS,KFL)*SD*SD
        IF(KFL.EQ.1) THEN
#if defined(CERNLIB_SINGLE)
          DENOM = GAMMA(TS(1))*GAMMA(TS(2)+ONED)/GAMMA(TS(1)+TS(2)+ONED)
#endif
#if defined(CERNLIB_DOUBLE)
          DENOM = DGAMMA(TS(1))*DGAMMA(TS(2)+ONED)/
     +                                          DGAMMA(TS(1)+TS(2)+ONED)
#endif
          XQ(KFL)=X**TS(1)*(1.-X)**TS(2)/DENOM
        ELSE
          XQ(KFL)=TS(1)*X**TS(2)*(1.-X)**TS(3)*(1.+TS(4)*X+TS(5)*X**2)
        ENDIF
  240   CONTINUE

C...Put into output arrays.
        UPV = XQ(1)
        DNV = XQ(1)
        SEA = XQ(3)/SIXD
        STR = XQ(3)/SIXD
        CHM = XQ(4)
        BOT = ZEROD
        TOP = ZEROD
        GL  = XQ(2)
C
        RETURN
        END
#endif
