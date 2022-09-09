#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "class_library.h"
#include "cop3502_as1.h"
#include "function_def.h"

#include "leak_detector_c.h"

int main() {

  // reports memory leaks
  atexit(report_mem_leak);

  // opens input and output files
  FILE *ifp;
  FILE *ofp;

  ifp = fopen("as1-sample-input.txt", "r");
  ofp = fopen("cop3502-as1-output-tshielekeja mbuyi-gradi.txt", "w");

  // buffer for file reading
  char buf[255];

  // var for monster types
  monster **type; int num_type;

  // var for region types
  region **regions; int num_region_type;

  // var for each trainers
  trainer **trainers; int num_trainer;

  // reads and stores monster types
  type = read_monster_type(ifp, buf, 255, type, &num_type);

  // reads and stores region types
  regions = read_regions(ifp, buf, 255, regions, type, num_type, &num_region_type);

  // reads and stores trainer information
  trainers = read_trainers(ifp, buf, 255, trainers, regions, num_region_type, &num_trainer);

  // prints results 
  print_output(ofp, trainers, num_trainer);

  // frees the dynomically allocated memories
  dispose_monster_list_all(type, num_type);
  dispose_region_list_all(regions, num_region_type);
  dispose_trainer_list_all(trainers, num_trainer);

  // closes input and output file
  fclose(ifp);
  fclose(ofp);

  return 0;
}