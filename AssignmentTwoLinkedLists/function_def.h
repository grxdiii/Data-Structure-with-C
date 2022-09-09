/* Dynamically allocate space for a single failfish */
failfish *create_failfish(int sequence_number) {
  failfish *f = malloc(sizeof(failfish));

  f->sequence_number = sequence_number;

  f->next = NULL;
  f->prev = NULL;

  return f;
}

/* Dispose and free failfish space */
void dispose_failfish(failfish *f) { 
  free(f);
}

/* Dynamically allocate space for a list of failfishes */
failfish_queue *create_failfish_queue(char *pondname, int n, int e, int th) {
  failfish_queue *f1 = malloc(sizeof(failfish_queue));

  f1->pondname = strdup(pondname); // allocates memory for pond name
  f1->n = n;
  f1->e = e;
  f1->th = th;

  f1->head = NULL;
  f1->tail = NULL;

  return f1; // head of the linked list
}

/* Shallow destructor of failfish_queue (linked list) */
void dispose_failfish_queue(failfish_queue *f1) { 
  free(f1); 
}

/* checks if the linked list is empty */
int is_queue_empty(failfish_queue *f1) {
  if (f1->head == NULL) {
    return 0; // returns 0 if list is empty
  } else {
    return 1; // returns 1 if there are items in the list
  }
}

/* Deep destructor of failfish_queue */
void dispose_failfish_queue_all(failfish_queue *f1) {
  failfish *f = f1->head;
  failfish *fnext;

  if (is_queue_empty(f1) == 0) { // linked list is empty
    dispose_failfish_queue(f1);
    return;
  }

  do { // linked list has items
    fnext = f->next;
    dispose_failfish(f);
    f = fnext;
  } while (f != f1->head);

  dispose_failfish_queue(f1);
}

/* Adds items to the linked list */
void enqueue(failfish_queue *f1, failfish *f) {
  if (is_queue_empty(f1) == 0) { // adds item to the head of the list
    f1->head = f;
    f1->tail = f;

    f->prev = f;
    f->next = f;
  } else { // adds item to the tail of the list
    f->next = f1->head;
    f->prev = f1->tail;

    f1->head->prev = f;
    f1->tail->next = f;
    f1->tail = f;
  }
}

/* Removes items from linked list */
void dequeue(failfish_queue *f1, failfish *f) {
  if (is_queue_empty(f1) == 0) {  // list is empty
    ;  // does nothing
  } else if (f1->head == f1->tail) { // empty the list
    if (f == f1->head) {
      f1->head = NULL;
      f1->tail = NULL;
      f->prev = NULL;
      f->next = NULL;
    }
  } else if (f == f1->head) { // removes the head of the list
    // changes the head of the list
    f1->head = f1->head->next;
    f1->head->prev = f1->tail;
    f1->tail->next = f1->head;
    f->prev = NULL;
    f->next = NULL;
  } else if (f == f1->tail) { // removes the tail of the list
    // changes the tail of the list
    f1->tail = f1->tail->prev;
    f1->head->prev = f1->tail;
    f1->tail->next = f1->head;
    f->prev = NULL;
    f->next = NULL;
  } else { // removes an interior element
    f->prev->next = f->next;
    f->next->prev = f->prev;
    f->prev = NULL;
    f->next = NULL;
  }

  dispose_failfish(f); // frees the failfish
}

/* Dynamically allocates space for a pond */
pond *new_pond(char *name, int id, failfish_queue *f1) {
  pond *p = malloc(sizeof(pond));

  p->name = strdup(name); // allocates memory for pond name
  p->id = id;
  p->f1 = f1;

  return p;
}

/* Dispose and free pond space */
void dispose_pond(pond *p) {
  free(p->name);
  dispose_failfish_queue_all(p->f1); // dispose linked list in pond
  free(p);
}

/* Dynamically allocate space for a list of ponds */
pond **new_pond_list(int nponds) {
  pond **p1 = malloc(nponds * sizeof(pond *)); // double pointer
  return p1;                                   // array of pond pointers
}

/* Shallow destructor of pond */
void dispose_pond_list(pond **p1) {
  free(p1);
}

/* Deep destructor of pond */
void dispose_pond_list_all(pond **p1, int nponds) {
  int i;

  for (i = 0; i < nponds; i++) { // loops through every pond
    if (p1[i] != NULL) {
      dispose_pond(p1[i]); // dispose each ponds
    }
  }

  dispose_pond_list(p1); // free the space allocated for the pond list
}

/* Reads the input */
pond **read_input(FILE *ifp, char *buf, int maxlength, pond **p1, int *nponds) {
  int i, j;

  // var for failfish and failfish_queue
  int id;
  char pondname[30];
  int n, e, th;

  // declaration of failfish, failfish_queue, and pond
  failfish_queue *f1;
  failfish *f;
  pond *p;

  // stores numbers of ponds
  get_next_nonblank_line(ifp, buf, 255);
  sscanf(buf, "%d", &*nponds);

  // creates a pond list (array) of nponds elements
  p1 = new_pond_list(*nponds);

  // loops nponds times to store pond informations
  for (i = 0; i < *nponds; i++) {
    // stores pond id, pond name, num fish, fish eath, and fish threshold
    get_next_nonblank_line(ifp, buf, 255);
    sscanf(buf, "%d %s %d %d %d", &id, pondname, &n, &e, &th);

    // creates linked list of failfish in pond
    f1 = create_failfish_queue(pondname, n, e, th);

    // adds n failfish to the linked list in pond
    for (j = 1; j < n + 1; j++) {
      f = create_failfish(j);
      enqueue(f1, f);
    }

    p = new_pond(pondname, id, f1); // creates new pond
    p1[i] = p;                      // insert pond to the list of ponds
  }
  return p1; // returns the pond list
}

/* sort linked list */
pond **sort_pond(pond **p1, int nponds) {
  int i, j;

  // temp var for pond name, id, and failfish list
  char *temp_name;
  int temp_id;
  failfish_queue *temp_f1;

  //  bubble sort
  for (i = 0; i < nponds - 1; i++) {
    for (j = 0; j < nponds - 1; j++) {
      if (p1[j]->id > p1[j + 1]->id) {
        temp_id = p1[j]->id;
        temp_name = p1[j]->name;
        temp_f1 = p1[j]->f1;

        p1[j]->id = p1[j + 1]->id;
        p1[j + 1]->id = temp_id;

        p1[j]->name = p1[j + 1]->name;
        p1[j + 1]->name = temp_name;

        p1[j]->f1 = p1[j + 1]->f1;
        p1[j + 1]->f1 = temp_f1;
      }
    }
  }

  return p1; // returns sorted list
}

/* Prints fialifh_queue */
void print_failfish_queue(FILE *ofp, failfish_queue *q) {
  int i;
  failfish *temp;

  temp = q->head;

  for (i = 0; i < q->n; i++) { // used to print out queue status
    if (temp != NULL) {
      fprintf(ofp, " %d", temp->sequence_number);
    }

    temp = temp->next;
  }
}

/* Prints pond status */
void print_status(FILE *ofp, pond **p1, int nponds) {
  int i;

  for (i = 0; i < nponds; i++) { // prints status of each pond
    fprintf(ofp, "%d %s", p1[i]->id, p1[i]->name);
    print_failfish_queue(ofp, p1[i]->f1); // prints failfish queue

    fprintf(ofp, "\n");
  }
}

/* Prints failfish eaten status */
void print_failfish_eaten(FILE *ofp, failfish_queue *q) {
  int i;

  failfish *temp = q->head;

  while (q->n != q->th) { // runs until failfish queue reaches threshold
    for (i = 0; i < q->e; i++) {
      temp = temp->next; // loops through queue
    }

    fprintf(ofp, "Failfish %d eaten\n", temp->prev->sequence_number);
    dequeue(q, temp->prev); // removes eaten failfish from queue
    q->n--;                 // decreseas num failfish
  }
}

/* Prints fish eaten during the first course */
void print_first_course(FILE *ofp, pond **p1, int nponds) {
  int i;

  fprintf(ofp, "\nFirst Course\n\n");

  for (i = 0; i < nponds; i++) { // fish eaten during the first course per ponds
    fprintf(ofp, "Pond %d: %s\n", p1[i]->id,
            p1[i]->name);                 // prints pond id and nanme
    print_failfish_eaten(ofp, p1[i]->f1); // prints failfish eaten status
    fprintf(ofp, "\n");
  }
}

/* Print second course */
void print_second_course(FILE *ofp, pond **p1, int nponds) {
  fprintf(ofp, "\nSecond Course\n\n"); // Title

  // temporary var of pond strucct type
  pond *temp;
  pond *p;

  // for loop var
  int i, j;
  int total_fish = 0;

  // calculate num failfish in pond list
  for (i = 0; i < nponds; i++) {
    total_fish += p1[i]->f1->n;
  }

  // commence second course
  for (i = 0; i < nponds; i++) {
    while (p1[i]->f1->head != NULL) {

      p = p1[i];

      for (j = 0; j < nponds; j++) {
        if (p1[j]->f1->head != NULL) {
          temp = p1[j];

          if (temp->f1->head->sequence_number > p->f1->head->sequence_number) {
            p = temp;
          }
        }
      }

      // prints eaten failfish
      if (total_fish > 1) {
        fprintf(ofp, "Eaten: Failfish %d from pond %d\n",
                p->f1->head->sequence_number, p->id);
        total_fish--;
      } else { // prints the last, remaining fish
        fprintf(ofp, "\nFailfish %d from pond %d remains\n",
                p->f1->head->sequence_number, p->id);
      }

      // removes failfish from failfish_queue
      dequeue(p->f1, p->f1->head);
    }
  }
}