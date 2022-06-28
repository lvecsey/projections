
all : gen_xdrstream show_xdrstream projections gen_plotfile

test : all
	./gen_xdrstream 5 < source_data.csv > inputstream.xdr
	./gen_plotfile 5 < ./inputstream.xdr > output_projections.dat
	gnuplot < output_projections.plot
	@echo You should now be able to view output_projections.png

