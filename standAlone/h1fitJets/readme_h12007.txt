===================================================================
===================================================================
==                                                               ==
==       README for H1 2007 DPDF Fit Jets Parameterization       ==
==                                                               ==
===================================================================
===================================================================

                     Version 1.0, 04/06/2009

                        H1 Collaboration

                        Contact persons:

         Matthias Mozer - mmozer@vub.ac.be
         Karel Cerny    - kcerny@ipnp.troja.mff.cuni.cz



                       === www source ===
https://www-h1.desy.de/h1/www/publications/htmlsplit/DESY-07-115.long.html


These files provide a fortran parameterisation of the results of the
NLO DGLAP QCD fits labelled 'H1 2007 DPDF Fit Jets' from the paper below.
Please reference this paper when using this parametrisation.

  H1 Collaboration, A. Aktas et al., "Dijet Cross Sections and Parton     
  Densities in Diffractive DIS at HERA", DESY-07-115, Aug 2007. 33pp.    
  Published in JHEP 0710:042,2007. e-Print: arXiv:0708.3217 [hep-ex]                                 



                    === Details of the fit ===
                                                                            
       The DPDFs are parameterised at a starting scale = 2.5 GeV^2                 
       available for :    1.960 <= Q2 <=  2000.0   GeV^2                         
                 and :    0.001 <= x  <=  0.932603                               
                                                                             
       for x outside limits, the closest limit is assumed                   
                    f (x > xmax, q2)  = f (xmax, q2)                          
                    f (x < xmin, q2)  = f (xmin, q2)                       
       for q2 outside limits, the closest limit is assumed                  
                    f (x, q2 > q2max) = f (x, q2max)                      
                    f (x, q2 < q2min) = f (x, q2min)                      

                       
                      === Files provided ===

 readme_h12007.txt                      This readme file
 h12007jetsdpdf_prcoeff.f		Pomeron and Reggeon fluxes
 h12007jetsdpdf_singlet.f		Quark singlet pdf paramterisation
 h12007jetsdpdf_gluon.f			Gluon pdf paramterisation
 h12007jetsdpdf_charm.f			F2 charm parameterisatoin
 h12007jetsdpdf_singlet.data		Grid data for singlet pdf
 h12007jetsdpdf_gluon.data		Grid data for gluon pdf
 h12007jetsdpdf_charm.data		Grid data for F2 charm
 h12007jetsdpdf_pdflib_interface.f	pdflib adaptor function


                          === Usage ===

For details on the arguments of the available routines, please also
look at the comments provided in the .f files. References are used
to the above publication.


a) Flux factor 
--------------

    f_{IP/p}(xpom)  or  f_{IR, p}(xpom) ... see pages 13, 14 of the paper

Subroutine h12007jetsdpdf_prcoeff(int type ,real xpom) provides the Pomeron
or Reggeon flux factor for a given xpom, i.e.:

                      type = 1  =>  Pomeron
                      type = 2  =>  Reggeon

The fluxes are integrated over the following 't' range (GeV^2):

             tmin = -1  
             tmax = -(mp*xpom)**2 / (1 - xpom)

where mp is the proton rest mass.


b) Parton distributions
-----------------------

The Pomeron parton distributions in the form of
   
        pdf_IP =  z * f_{i/IP}(z,Q^2), i ... singlet, gluon, charm

are provided by the functions:

            h12007jetsdpdf_singlet(xin,q2in,icycle),
            h12007jetsdpdf_gluon  (xin,q2in,icycle),
            h12007jetsdpdf_charm  (xin,q2in,icycle),

where: 'xin'    is the parton four-momentum fraction
                (often called beta or zpomeron)
       'q2in'   is the factorization scale squared 
       'icycle' MUST BE SET TO ZERO by default. 

Only the flux for the Reggeon component is provided here, the parton densities must be
provided separately.  The fit used Owens set 1 - PDFLIB-Owens(2,1,1).


c) Final instructions
---------------------

Eventually, in order to obtain proton diffractive PDFs, the Pomeron/Reggeon parton
distributions have to be multiplied by the Pomeron/Reggeon flux factor.

If a cross section integrated over xpomeron is calculated, then also the
flux * pdf should multiplied by the xpomeron-bin-width,
 
DPDF_diff = (flux_IP * pdf_IP * xpom_width) + (flux_IR * pdf_IR * xpom_width)

where all terms are provided by the H1 2007 Fit Jets, except for the pdf_IR
which needs to be provided separately. An example is given in section d) below.



d) A minimal example of PDFLIB use of the fit.
----------------------------------------------

Although not supported anymore an example how to use the fit by means
of use of PDFLIB is given in:
            
                  h12007jetsdpdf_pdflib_interface.F

 
