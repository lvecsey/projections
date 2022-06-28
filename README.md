
*Overview*

Convert an *XDR* stream of revenue data including costs and margins, and calculate the profit for each set of day values provided. Plot to a bar chart with *gnuplot* as a *png* file. Data is originally taken from a *CSV* file, a sample of which is included.

*Dependencies*

```console
sudo apt-get install gnuplot
```

*Binaries*

```console
make all
```

*Input CSV (data)*

There are a certain number of days (or rows) in the CSV file which is a parameter that needs to be passed to the rest of the programs.

*Generating input stream*

```console
make gen_xdrstream
./gen_xdrstream 5 < source_data.csv > inputstream.xdr
```

*Running the projections*

```console
make projections
./projections 5 < ./inputstream.xdr
```

The program will output profit values for each day.

*Plotting the data*

Specify the number of days (rows) of CSV data to process with the *gen_plotfile* command.

```console
make gen_plotfile
./gen_plotfile 5 < ./inputstream.xdr > output_projections.dat
gnuplot < output_projections.plot
```

You should now have a output_projections.png file to be displayed with an image viewer.


