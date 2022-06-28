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

#include <stdint.h>
#include <rpc/xdr.h>

typedef struct {

  long int day;
  long int revenue_dollars;
  long int revenue_cents;
  long int cost_dollars;
  long int cost_cents;
  long int margin_dollars;
  long int margin_cents;

} source_csv;

int main(int argc, char *argv[]) {

  XDR *xdrs;

  long int num_xdrs;

  int num_rows;

  long int *revenue_dollars;
  long int *revenue_cents;

  long int *margin_dollars;
  long int *margin_cents;

  long int *cost_dollars;
  long int *cost_cents;

  num_rows = argc>1 ? strtol(argv[1], NULL, 10) : -1;

  if (num_rows <= 0) {
    printf("Please enter number of rows to process.\n");
    return 0;
  }

  revenue_dollars = malloc(sizeof(long int) * num_rows);
  revenue_cents = malloc(sizeof(long int) * num_rows);

  margin_dollars = malloc(sizeof(long int) * num_rows);
  margin_cents = malloc(sizeof(long int) * num_rows);

  cost_dollars = malloc(sizeof(long int) * num_rows);
  cost_cents = malloc(sizeof(long int) * num_rows);    

  char *line;
  size_t len;
  FILE *fp;

  int retval;

  long int lineno;
  
  long int rowno;
  
  line = NULL;
  len = 0;
  fp = stdin;

  retval = getline(&line,&len,fp);
  if (retval == -1) {
    printf("Trouble reading CSV header.\n");
    return -1;
  }

  lineno = 0;
  
  while ((retval = getline(&line,&len,fp)) != -1) {

    source_csv row;
    
    if (len <= 0) {
      continue;
    }

    retval = sscanf(line, "%ld, %ld, %ld, %ld, %ld, %ld, %ld", &(row.day), &(row.revenue_dollars), &(row.revenue_cents), &(row.cost_dollars), &(row.cost_cents), &(row.margin_dollars), &(row.margin_cents));

    if (retval != 7) {
      perror("sscanf");
      return -1;
    }

    rowno = lineno;

    revenue_dollars[rowno] = row.revenue_dollars;
    revenue_cents[rowno] = row.revenue_cents;
    cost_dollars[rowno] = row.cost_dollars;
    cost_cents[rowno] = row.cost_cents;
    margin_dollars[rowno] = row.margin_dollars;
    margin_cents[rowno] = row.margin_cents;
    
    lineno++;
    
  }

  {

    num_xdrs = 1;

    xdrs = malloc(sizeof(XDR) * num_xdrs);
    if (xdrs == NULL) {
      perror("malloc");
      return -1;
    }

    xdrstdio_create(xdrs, stdout, XDR_ENCODE);

    xdr_vector(xdrs, revenue_dollars, num_rows, sizeof(long int), xdr_long);
    xdr_vector(xdrs, revenue_cents, num_rows, sizeof(long int), xdr_long);

    xdr_vector(xdrs, cost_dollars, num_rows, sizeof(long int), xdr_long);
    xdr_vector(xdrs, cost_cents, num_rows, sizeof(long int), xdr_long);     

    xdr_vector(xdrs, margin_dollars, num_rows, sizeof(long int), xdr_long);
    xdr_vector(xdrs, margin_cents, num_rows, sizeof(long int), xdr_long);

    xdr_destroy(xdrs);

  }
  
  return 0;

}

