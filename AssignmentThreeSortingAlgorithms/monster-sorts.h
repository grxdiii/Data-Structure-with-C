#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include "leak_detector_c.h"

typedef struct monster {
	int id;
	char name[64];
	char element[64];
	int population;
	double weight;
} monster;

extern int Comparisons;
extern int Copies;
extern int Block_Copies;
extern int Swaps;
extern int Mallocs;
extern clock_t Start_Cpu;

int compare_monsters(monster *m1, monster *m2, int use_name, int use_weight);
void quick_sort(monster *list, int n, int use_name, int use_weight);
void bubble_sort(monster *list, int n, int use_name, int use_weight);
void selection_sort(monster *list, int n, int use_name, int use_weight);
void insertion_sort(monster *list, int n, int use_name, int use_weight);
void merge_sort(monster *list, int n, int use_name, int use_weight);
void merge_insertion_sort(monster *list, int n, int use_name, int use_weight);

void clear_stats();
void print_stats();
