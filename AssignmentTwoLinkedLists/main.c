#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "class_library.h"
#include "struct_def.h"
#include "leak_detector_c.h"

#include "function_def.h"

int main() {
  atexit(report_mem_leak);

  // input and output files
  FILE *ifp;
  FILE *ofp;

  ifp = fopen("cop3502-as2-input.txt", "r");
  ofp = fopen("cop3502-as2-output-tshielekeja mbuyi-gradi.txt", "w");

  // buf: used to read each lines of input file
  char buf[255];

  // nponds: stores the number of pond
  int nponds;

  // p1: double pointer of pond
  pond **p1;

  // read and store pond list information
  p1 = read_input(ifp, buf, 255, p1, &nponds);
  p1 = sort_pond(p1, nponds);

  // prints initial pond status
  fprintf(ofp, "Initial Pond Status\n");
  print_status(ofp, p1, nponds);

  // prints first course
  print_first_course(ofp, p1, nponds);

  // prints end of course status
  fprintf(ofp, "End of Course Pond Status\n");
  print_status(ofp, p1, nponds);

  // prints second course
  print_second_course(ofp, p1, nponds);

  // dispose pond list
  dispose_pond_list_all(p1, nponds);

  // close input and output file
  fclose(ifp);
  fclose(ofp);

  return 0;
}