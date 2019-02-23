dpdf2006.o: h1pdf2006/h12006flux.o  h1pdf2006/i_2006_fita.o  h1pdf2006/i_2006_fitb.o  h1pdf2006/qcd_2006.o  h1pdf2006Err/h12006flux.err.o  h1pdf2006Err/h12006pdf.o  h1pdf2006Err/lha2006fita.o  h1pdf2006Err/lha2006fitb.o
	#gfortran -c  $^ -o $@
	ld -r  $^ -o $@
