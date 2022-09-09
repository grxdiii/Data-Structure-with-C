/* Defining failfish struct */
typedef struct failfish_struct{
    int sequence_number;

    struct failfish_struct *next;
    struct failfish_struct *prev;
} failfish;

/* Defining failfish_queue struct */
typedef struct failfish_list_struct{
    char *pondname;
    int n;
    int e;
    int th;

    // keeps track of head and tail of the list
    failfish *head;
    failfish *tail;
} failfish_queue;

/* Defining pond struct */
typedef struct pond_struct{
    char *name;
    int id;

    failfish_queue *f1;
} pond;