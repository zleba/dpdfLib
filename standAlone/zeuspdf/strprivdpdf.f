      SUBROUTINE STRPRIV(X,MUF,UPV,DNV,USEA,DSEA,STR,CHM,BOT,TOP,GL)
C ******************************************************************   
C   This routine provides the interface to the DPDFLIB
* INPUT:
C                  X  = x-value of parton   
C                MUF  = QCD scale in GeV
* OUTPUT:  UPV,DNV,USEA,DSEA,STR,CHM,BOT,TOP,GL
C ******************************************************************   
      IMPLICIT NONE

      DOUBLE PRECISION X,MUF 
      DOUBLE PRECISION UPV,DNV,USEA,DSEA,STR,CHM,BOT,TOP,GL 

      integer dpdf,dpdfset,dpdffit,dpdfvar,dpdfopt, i

      logical first
      data first /.true./
      save first

      Double Precision xpq
      DIMENSION XPQ(0:6) ! changed from -6:6 to 0:6 to be in accordance with ZEUS

      Double Precision QCDL4,QCDL5
      COMMON/W50512/ QCDL4,QCDL5

* access xpom, xpombinwidth (=scal), idpdf (PDF id) from steering common block
      Double Precision scal,xpom,deltay
      integer idpdf
      common/adster/scal,xpom,idpdf,deltay

cKC
      integer ifit,ipdf,ipom
      Double Precision f2,fl,c2,cl,flux
      Double Precision z
cKC

cKC
      z = x / xpom
cKC


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

c      write(6,*) 'KAREL: IN STRPRIV FOR ZEUS DPDFS'


ccccccccccccccccccccccccccccc
c     first call
ccccccccccccccccccccccccccccc
      if (first) then
      CALL LoadGrid(
     &'/afs/desy.de/user/z/zlebcr/h1/nlo/'//
     & 'frixione/zeusdpdf/grids/zeusD_SJ',0)
        first = .false.
      endif
      call settmin(-1.d0)



c      call h12006flux(xpom,-1.d0,1,ifit,ipdf,ipom,flux)
c      call h12006pdf(x,muf*muf,ifit,ipdf,xpq,f2,fl,c2,cl)
cccccccccccccccccccccccccccccccccccccccccccccccccccccc
c     ZEUS DPDFs called according to demo program
cccccccccccccccccccccccccccccccccccccccccccccccccccccc
*     write (*,*) 'radek scale ', muf, ' Gev'
      CALL dpdf3 (xpom, x, muf*muf, xpq,1)  ! IP+IR
c     CALL dpdf3p(xpom, x, muf*muf, xpq,1)  ! IP only



*     scaling by x_pomeron bin width and flux
      do i=0,6
         xpq(i)= xpq(i) * scal 
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



c      print *,'STRPRIV: mu2 x gluon', muf*muf, x, gl
      RETURN
      END
