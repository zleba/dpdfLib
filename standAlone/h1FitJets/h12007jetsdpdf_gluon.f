**********************************************************************
c  H1 2007 Jets DPDF: h12007jetsdpdf_gluon density parameterisation               *
c--------------------------------------------------------------------*
c Reference: Dijet Cross Sections and Parton Densities in            *
c Diffractive DIS at HERA.                                           *
c H1 Collaboration (A. Aktas et al.). DESY-07-115, Aug 2007. 33pp.   *
c Published in JHEP 0710:042,2007.                                   *
c e-Print: arXiv:0708.3217 [hep-ex]                                  *
c                                                                    *
c--------------------------------------------------------------------*
c This routine returns the pomeron h12007jetsdpdf_gluon density (z*pdf(z,Q^2))    *
c                                                                    *
c  Input: xin:    z-pom (parton momentum fraction)                   *
c         q2in:   scale^2 where dpdf's are evaluated (in GeV^2)      *
c         icycle: legacy parameter, set to 0                         *
c                                                                    *
c  evolution has been made starting at q2_input =      2.500         *
c  available for :       1.960 <= q2 <=    2000.000                  *
c            and :  0.001000 <= x <=  0.932603                       *
c*                                                                   *
c   for x outside limits, the closest limit                          *
c        is assumed : f2(x>xmax,q2)=f2(xmax,q2)                      *
c                     f2(x<xmin,q2)=f2(xmin,q2)                      *
c  for q2 outside limits, the closest limit                          *
c        is assumed : f2(x,q2>q2max)=f2(x,q2max)                     *
c                     f2(x,q2<q2min)=f2(x,q2min)                     *
c*                                                                   *
c  comments, etc... to C. Pascaud or F. Zomer or M. Mozer            *
**********************************************************************
       real FUNCTION  h12007jetsdpdf_gluon(xin,q2in,icycle)
      implicit real*8 (a-h,o-z)
       PARAMETER(n_bin_q2= 88)
       PARAMETER(n_bin_x=100)
      REAL*4 xl_bin(n_bin_x),q2l_bin(n_bin_q2) 
      REAL*4 f(n_bin_x,n_bin_q2)               
cKAREL      CHARACTER*(40) name                         
cKAREL
      CHARACTER*(800) name, lhapdfDir
cKAREL
      data   ilSAVE/-1/                            
      SAVE
      if(ilSAVE.ne.icycle) THEN                   
cKAREL
      call getenv('LHAPDF_DATA_PATH', lhapdfDir)
      name =trim(lhapdfDir)//trim('/../standAlone/h1FitJets/h12007jetsdp&
     &df_gluon.data')

c     name='h1fitJets/h12007jetsdpdf_gluon.data'
      
cKAREL
      if(icycle.ne.0) THEN
cKAREL       WRITE(name,282)'h12007jetsdpdf_gluon.data',icycle
      ENDIF
      open(unit=28,file=name,status='old')  
      ilSAVE=icycle                                
282   FORMAT(a,'_',i3.3,'.data')
280   FORMAT(6e13.7)                               
281   FORMAT(6e13.5)                               
c      read(28,*)
c      read(28,280,end=9)(xl_bin(i),i=1,n_bin_x)    
      read(28,*)(xl_bin(i),i=1,n_bin_x)    
      read(28,281)(q2l_bin(i),i=1,n_bin_q2)        
      do i=1,n_bin_q2                              
      q2l_bin(i)=log(q2l_bin(i))                   
!     CALL vzero(f(1,i),n_bin_x)                   
      do j=1,nbin_x
         f(j,i) = 0.0
      enddo
      enddo
      do i=1,n_bin_q2                              
      read(28,*)(f(k,i),k=1,n_bin_x)             
      enddo                                        
9     close(28)                                    
      endif                                        
c      q2in = qin*qin
      x=log(xin)                                   
      y=log(q2in)                                  
      DO i=2,n_bin_x                               
        IF(x.LT.xl_bin(i))  goto 1                 
        IF(xl_bin(i).ge.0.)  goto 1                
      ENDDO                                        
      i=n_bin_x                                    
    1 i=i-1                                        
      DO j=2,n_bin_q2                              
        IF(y.LT.q2l_bin(j))  GOTO 2                
      ENDDO                                        
      j=n_bin_q2                                   
    2 j=j-1                                        
      dx=xl_bin(i+1)-xl_bin(i)                     
      xd=(x-xl_bin(i))/dx                          
      dy=q2l_bin(j+1)-q2l_bin(j)                   
      yd=(y-q2l_bin(j))/dy                         
      h12007jetsdpdf_gluon=f(i,j)+xd*(f(i+1,j)-f(i,j))
     &+yd*(f(i,j+1)-f(i,j))                        
     &+xd*yd*(f(i+1,j+1)+f(i,j)-f(i+1,j)-f(i,j+1)) 
      END                                          
