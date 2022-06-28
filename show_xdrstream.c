/*
  Projections to retrieve an XDR stream of data and calculate profit.
  Copyright (C) 2022  Lester Vecsey

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <rpc/xdr.h>

int main(int argc, char *argv[]) {

  XDR *xdrs;
  
  double profit;
  double revenue;
  double margin;
  double costs;

  bool_t bret;

  long int num_xdrs;

  long int num_days;

  long int *profits_dollars;
  long int *profits_cents;

  long int *revenues_dollars;
  long int *revenues_cents;

  long int *margin_dollars;
  long int *margin_cents;

  long int *costs_dollars;
  long int *costs_cents;    

  num_days = argc>1 ? strtol(argv[1], NULL, 10) : -1;

  if (num_days < 0) {
    printf("Please specify number of days.\n");
    return -1;
  }

  {

    profits_dollars = malloc(sizeof(long int) * num_days);
    profits_cents = malloc(sizeof(long int) * num_days);

    revenues_dollars = malloc(sizeof(long int) * num_days);
    revenues_cents = malloc(sizeof(long int) * num_days);

    margin_dollars = malloc(sizeof(long int) * num_days);
    margin_cents = malloc(sizeof(long int) * num_days);        

    costs_dollars = malloc(sizeof(long int) * num_days);
    costs_cents = malloc(sizeof(long int) * num_days);        

  }
  
  num_xdrs = 1;
  
  xdrs = malloc(sizeof(XDR) * num_xdrs);
  if (xdrs == NULL) {
    perror("malloc");
    return -1;
  }

  xdrstdio_create(xdrs, stdin, XDR_DECODE);

  xdr_vector(xdrs, revenues_dollars, num_days, sizeof(long int), xdr_long);
  xdr_vector(xdrs, revenues_cents, num_days, sizeof(long int), xdr_long);

  xdr_vector(xdrs, costs_dollars, num_days, sizeof(long int), xdr_long);
  xdr_vector(xdrs, costs_cents, num_days, sizeof(long int), xdr_long);

  xdr_vector(xdrs, margin_dollars, num_days, sizeof(long int), xdr_long);
  xdr_vector(xdrs, margin_cents, num_days,  sizeof(long int), xdr_long);

  {
    
    long int dayno;

    for (dayno = 0; dayno < num_days; dayno++) {
    
      profits_cents[dayno] = (100 * revenues_dollars[dayno] + revenues_cents[dayno]) - (100 * costs_dollars[dayno] + costs_cents[dayno]) + (100 * margin_dollars[dayno] + margin_cents[dayno]);

      profits_dollars[dayno] = profits_cents[dayno] / 100;
      profits_cents[dayno] -= (profits_dollars[dayno] * 100);

    }
      
    xdr_destroy(xdrs);

  }

  {

    long int dayno;
  
    for (dayno = 0; dayno < num_days; dayno++) {

      printf("%ld, %ld, %ld, %ld, %ld, %ld, %ld\n", dayno, revenues_dollars[dayno], revenues_cents[dayno], costs_dollars[dayno], costs_cents[dayno], margin_dollars[dayno], margin_cents[dayno]);
 
    }

  }
  
  return 0;

}

