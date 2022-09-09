// new_monster: allocates memory for a monster, its name, and its element.
monster *new_monster(int id, char *name, char *element, int population) {
  monster *m = malloc(sizeof(monster));

  m->id = id;
  m->name = strdup(name);
  m->element = strdup(element);
  m->population = population;

  return m;
}

// dispose_monster: clears and free the allocated memory of the monster struct
void dispose_monster(monster *m) {
  free(m->name);
  free(m->element);
  free(m);
}

// new_monster_list: an array of monster, whose memory is dynamically allocated
monster **new_monster_list(int nmonsters) {
  monster **m1 = malloc(nmonsters * sizeof(monster *));
  return m1;
}

// disponse_monster_list: shallow destructor of monster list
void dispose_monster_list(monster **m1) { 
  free(m1); 
}

// dispose_monster_list_all: deep destructor of monster list
void dispose_monster_list_all(monster **m1, int nmonsters) {
  int i;

  for (i = 0; i < nmonsters; i++) {
    if (m1[i] != NULL) {
      dispose_monster(m1[i]);
    }
  }
  dispose_monster_list(m1);
}

// new_region: allocates memory for a single region and its name.]
region *new_region(char *name, int nmonsters, int total_population, monster **monsters) {
  region *r = malloc(sizeof(region));

  r->name = strdup(name);
  r->nmonsters = nmonsters;
  r->total_population = total_population;
  r->monsters = monsters;

  return r;
}

// dispose_region: clears and free the allocated memory of the region struct
void dispose_region(region *r) {
  free(r->name);
  free(r->monsters);
  free(r);
}

// new_region_list: an array of regions, whose memory is dynamically allocated
region **new_region_list(int nregions) {
  region **r1 = malloc(nregions * sizeof(region *));
  return r1;
}

// dispose_region_list: shallow destructor of region list
void dispose_region_list(region **r1) { 
  free(r1); 
}

// dispose_region_list_all: deep destructor of region list
void dispose_region_list_all(region **r1, int nregions) {
  int i;

  for (i = 0; i < nregions; i++) {
    if (r1[i] != NULL) {
      dispose_region(r1[i]);
    }
  }
  dispose_region_list(r1);
}

// new_itinerary: allocates memory for each visits
itinerary *new_itinerary(int nregions, region **regions, int captures) {
  itinerary *i = malloc(sizeof(itinerary));

  i->nregions = nregions;
  i->regions = regions;
  i->captures = captures;

  return i;
}

// dispose_itinerary: clears and free the allocated memory of the itinerary struct
void dispose_itinerary(itinerary *i) {
  free(i->regions);
  free(i);
}

// new_trainer: allocates memory for a trainer and their name 
trainer *new_trainer(char *name, itinerary *visits) {
  trainer *t = malloc(sizeof(trainer));

  t->name = strdup(name);
  t->visits = visits;

  return t;
}

// dispose_trainer: clears and free the allocated memory of the trainer struct
void dispose_trainer(trainer *t) {
  free(t->name);
  dispose_itinerary(t->visits);
  free(t);
}

// new_trainer_list: an array of trainers, which dynamically allocates memory for a list of trainer
trainer **new_trainer_list(int ntrainers) {
  trainer **t1 = calloc(ntrainers, sizeof(trainer *));
  return t1;
}

// dispose_trainer_list: shallow destructor for trainer
void dispose_trainer_list(trainer **t1) { 
  free(t1); 
}

// dispose_trainer_list_all: deep destructor for trainer
void dispose_trainer_list_all(trainer **t1, int ntrainers) {
  int i;

  for (i = 0; i < ntrainers; i++) {
    if (t1[i] != NULL) {
      dispose_trainer(t1[i]);
    }
  }
  dispose_trainer_list(t1);
}

// read_monster_type: reads the types of monsters, their names, and population. 
monster **read_monster_type(FILE *ifp, char *buf, int max_length, monster **type, int *num_type) {
  int i;

  char name[30], element[30];
  int population;

  monster *m_type;

  get_next_nonblank_line(ifp, buf, max_length);
  sscanf(buf, "%d", &*num_type);

  // stores monsters data
  type = new_monster_list(*num_type);

  for (i = 0; i < *num_type; i++) {
    get_next_nonblank_line(ifp, buf, max_length);
    sscanf(buf, "%s %s %d", name, element, &population);

    m_type = new_monster(i, name, element, population);
    type[i] = m_type;
  }

  return type;
}

// check_type: checks if monster in region is present 
monster *check_type(char *m_name, monster **type, int num_type) {
  int i;
  monster *m;

  // loops through the monster types list
  for (i = 0; i < num_type; i++) {
    if (strcmp(m_name, type[i]->name) == 0) {
      m = type[i];
    }
  }

  return m;
}

// create_region: reads in region input and stores it
region *create_region(FILE *ifp, char *buf, int max_length, region *r, char *r_name, monster **type, int num_type, int nmonsters) {
  int i;

  monster *m, **m1;

  char m_name[30];

  int total_population = 0;

  // monster list for a region
  m1 = new_monster_list(nmonsters);

  // checks monster type
  for (i = 0; i < nmonsters; i++) {
    get_next_nonblank_line(ifp, buf, max_length);
    sscanf(buf, "%s", m_name);

    m = check_type(m_name, type, num_type);
    total_population += m->population;

    // stores monster in monster list
    m1[i] = m;
  }

  // stores region type
  r = new_region(r_name, nmonsters, total_population, m1);
  return r;
}

// read_regions: reads n regions and creates a region list
region **read_regions(FILE *ifp, char *buf, int max_length, region **regions, monster **type, int num_type, int *nregions) {
  int i;

  char r_name[30];
  int nmonsters;

  region *r;

  get_next_nonblank_line(ifp, buf, max_length);
  get_next_nonblank_line(ifp, buf, max_length);
  sscanf(buf, "%d", &*nregions);

  // region list with n regions
  regions = new_region_list(*nregions);

  // loops through each regions
  for (i = 0; i < *nregions; i++) {
    get_next_nonblank_line(ifp, buf, max_length);
    get_next_nonblank_line(ifp, buf, max_length);
    sscanf(buf, "%s", r_name);

    get_next_nonblank_line(ifp, buf, max_length);
    sscanf(buf, "%d", &nmonsters);

    // creates regions and store its data in region list
    r = create_region(ifp, buf, max_length, r, r_name, type, num_type, nmonsters);
    regions[i] = r;
  }

  return regions;
}

// check_region: check if region in trianer is present
region *check_region(char *r_name, region **regions, int num_region_type) {
  int i;
  region *r;

  for (i = 0; i < num_region_type; i++) {
    if (strcmp(r_name, regions[i]->name) == 0) {
      r = regions[i];
    }
  }

  return r;
}

// create_trainer: after reading trainers info, this function stores the data
trainer *create_trainer(FILE *ifp, char *buf, int max_length, trainer *t, char *t_name, int captures, region **regions, int nregions, int num_region_type) {
  int i;

  region *r, **r1;
  itinerary *visits;

  char r_name[30];

  r1 = new_region_list(nregions);

  for (i = 0; i < nregions; i++) {
    get_next_nonblank_line(ifp, buf, max_length);
    sscanf(buf, "%s", r_name);

    r = check_region(r_name, regions, num_region_type);
    r1[i] = r;
  }

  visits = new_itinerary(nregions, r1, captures);
  t = new_trainer(t_name, visits);

  return t;
}

// read_trainers: reads in trainers info and stores them
trainer **read_trainers(FILE *ifp, char *buf, int max_length, trainer **trainers, region **regions, int num_region_type, int *ntrainers) {
  int i;

  int captures;
  int nregions;

  char t_name[30];
  trainer *t;

  // reads ntrainers
  get_next_nonblank_line(ifp, buf, max_length);
  get_next_nonblank_line(ifp, buf, max_length);
  sscanf(buf, "%d", &*ntrainers);

  // creates trainer list with ntrainers
  trainers = new_trainer_list(*ntrainers);

  // reads in trainer name, captures, and region info
  for (i = 0; i < *ntrainers; i++) {
    get_next_nonblank_line(ifp, buf, max_length);
    get_next_nonblank_line(ifp, buf, max_length);
    sscanf(buf, "%s", t_name);

    get_next_nonblank_line(ifp, buf, max_length);
    sscanf(buf, "%d", &captures);

    get_next_nonblank_line(ifp, buf, max_length);
    sscanf(buf, "%d", &nregions);

    // creates trainer and stores info in trainers list
    t = create_trainer(ifp, buf, max_length, t, t_name, captures, regions, nregions, num_region_type);
    trainers[i] = t;
  }

  return trainers;
}

// print_monster: print monster captured by trainer
void print_monster(FILE *ofp, trainer *t, region *r) {
  int i;
  float m_captured;

  int nmonsters = r->nmonsters;
  int total_population = r->total_population;
  int captures = t->visits->captures;

  monster *m;

  // print monster info
  for (i = 0; i < nmonsters; i++) {
    m = r->monsters[i];
    total_population = r->total_population;
    
    // calculate m_captured
    m_captured = (float)1 / total_population;
    m_captured = m_captured * m->population;
    m_captured = m_captured * captures;

    if ((int)round(m_captured) > 0) {
      fprintf(ofp, "%d %s\n", (int)round(m_captured), m->name);
    }
  }
}

// print_region: prints regions in trainers 
void print_region(FILE *ofp, trainer *t) {
  int i;
  int nregions = t->visits->nregions;
  region *r;

  // prints region info
  for (i = 0; i < nregions; i++) {
    r = t->visits->regions[i];

    fprintf(ofp, "%s\n", r->name); // region's name
    print_monster(ofp, t, r);
  }
}

// print_output: prints in output file
void print_output(FILE *ofp, trainer **trainers, int num_trainer) {
  int i;
  trainer *t;

  // prints trainer info
  for (i = 0; i < num_trainer; i++) {
    t = trainers[i];

    fprintf(ofp, "%s\n", t->name); // trainer's name
    print_region(ofp, t); // trainer's regions
    fprintf(ofp, "\n");
  }
}