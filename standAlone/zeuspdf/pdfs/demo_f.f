*  Author: Wojtek Slominski
*  Date: 2009-12-18 22:22
*  Description:
*  an example program using ZDPDF.
*  For each Q^2 given in QQvals
*  the program prints all xPom*PDFs

      program demo
      IMPLICIT none
      DOUBLE PRECISION xPom
      INTEGER Nq,Nx
      PARAMETER (Nq = 4)
      PARAMETER (Nx = 200)
      DOUBLE PRECISION QQvals(Nq), xleft,f(0:6),QQ,x,dx
      INTEGER iq, ix, ip
      DATA QQvals/6.d0,20.d0,60.d0,200.d0/
      DATA xleft/1.d-4/

      CALL LoadGrid('../grids/zeusD_SJ',0)
      PRINT 1006
      xPom = 0.003
      dx = (1.d0 - xleft)/(Nx-1)
      do iq=1,Nq
        QQ = QQvals(iq)
        print 1007, QQ
        do ix=1,Nx
          x = xleft + (ix-1)*dx
c          CALL dpdf3(xPom, x, QQ, f,1) ! Pomeron+Reggeon
          CALL dpdf3p(xPom, x, QQ, f,1) ! Pomeron only
          PRINT 1008,x,(xPom*f(ip),ip=0,6)
        end do
        print*
      end do
 1006 FORMAT ('# columns: x  xg  xd  xu  xs  xc  xb  xt')
 1007 FORMAT ('# Q^2 = ',G10.4)
 1008 FORMAT (F6.4,1X,8(1PE11.4,1X))

      stop
      end

