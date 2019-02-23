      real function h12007jetsdpdf_prcoeff(ityp,xpom, tcut)

      IMPLICIT REAL*8(a-h,o-z)

      integer ityp
      real xpom,beta,q2

      real*8 normpom,normmes,tcut

      logical first
      data first /.true./
      save first

c     *************************************************************************

c     NOTE: These parameters need adjustment for QCDFIT changes!!!

c meson trajectory unchanged

      parameter (appom=0.06 , b0pom=5.5 )
      parameter (a0mes=0.5    , apmes=0.3  , b0mes=1.6 )

c pomeron trajectory for H1 2007 Jets DPDF
      parameter ( a0pom=1.1037, cmes=0.0013 )

c     *************************************************************************

      dxpom=dble(xpom)

      call h12007jetsdpdf_reggeflux(0.003d0,0.d0,0.d0,a0pom,
     &     0.d0,0.d0,1.d0,tcut,appom,b0pom,1.0d0,dp)
      call h12007jetsdpdf_reggeflux(0.003d0,0.d0,0.d0,a0mes,
     &     0.d0,0.d0,1.d0,tcut,apmes,b0mes,1.0d0,dm)
      normpom= 1./(0.003d0*dp)
      normmes=(1./(0.003d0*dm))*cmes

      call h12007jetsdpdf_reggeflux(dxpom,0.d0,0.d0,a0pom,
     &     0.d0,0.d0,1.d0,tcut,appom,b0pom,normpom,flux)
      flux_pom=flux

      call h12007jetsdpdf_reggeflux(dxpom,0.d0,0.d0,a0mes,
     &     0.d0,0.d0,1.d0,tcut,apmes,b0mes,normmes,flux)
      flux_mes=flux

      if (first) then
        print *,'PRCOEFF: a0pom,appom,b0pom: ',a0pom,appom,b0pom
        print *,'PRCOEFF: a0mes,apmes,b0mes,cmes: ',
     &           a0mes,apmes,b0mes,cmes
      endif

      if (ityp.eq.1) then
         h12007jetsdpdf_prcoeff=real(flux_pom)
      elseif (ityp.eq.2) then
         h12007jetsdpdf_prcoeff=real(flux_mes)
      else
         print *,'PRCOEFF ERROR: ityp=',ityp
         stop
      endif
      
      first=.false.

      return
      end

*******************************************************************************

      subroutine h12007jetsdpdf_reggeflux(x_pom,beta,q2,a0,a1,
     &     a2,stmin,stcut,ap,b0,c,fl)

      implicit none

      real*8 x_pom,beta,q2,a0,a1,a2,stmin,stcut,ap,b0,c,fl
      real*8 tmin,tcut

c     calc min. kinematically  allowed t
      tmin= -(0.93827231D0*x_pom)**2/(1.D0-x_pom)

c     default tcut
      tcut = -1.0d0

c     from steering: if >0 , take tmin from above ...
      if (stmin.lt.0.) tmin=stmin
c     from steering: if >0 , take tcut=-1 from above ...
      if (stcut.lt.0.) tcut=stcut

c     get flux, integrated over t ( tcut ... tmin )
      fl = dexp((2.0d0*a0-1.)*dlog(1.0d0/x_pom))/
     +     (b0+2.0d0*ap*dlog(1.0d0/x_pom))
      fl = fl * (dexp(tmin*(b0+2.0d0*ap*dlog(1.0d0/x_pom)))
     +          -dexp(tcut*(b0+2.0d0*ap*dlog(1.0d0/x_pom))))

      fl = c * fl

      return
      end
