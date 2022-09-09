#include "monster-sorts.h"
#include <string.h>

void print_clocks(clock_t clocks) {
  printf("  %lfs CPU time used\n", ((double) clocks) / CLOCKS_PER_SEC);
}

/* The core swap function */
void swap_monsters(monster *list, int i, int j)
{
  monster temp;

  memcpy(&temp, list + i, sizeof(monster));
  memcpy(list + i, list + j, sizeof(monster));
  memcpy(list + j, &temp, sizeof(monster));

  Swaps++;
}

/* The core comparison function. */
int compare_monsters(monster *m1, monster *m2, int use_name, int use_weight)
{
  Comparisons++;

  // compares m1 and m2, returning 1 if m1 > m2
  if(use_weight == 1 && m1->weight > m2->weight) return 1;
  if(use_weight == 1 && m1->weight == m2->weight) return 0;
  if(use_name == 1 && strcmp(m1->name, m2->name) > 0) return 1;
  if(use_name == 1 && strcmp(m1->name, m2->name) == 0) return 0;

  return -1;
}


int repartition(monster *list, int low_index, int high_index, int use_name, int use_weight)
{
  int i = low_index, j;

  for(j = low_index; j < high_index; j++){
    if(compare_monsters(&list[j], &list[high_index], use_name, use_weight) == -1){
      swap_monsters(list, i, j); i++;
    }
  }

  swap_monsters(list, i, high_index);
  return i;
}

/* Recursive function for quick sort. */
void quick_sort_recursive(monster *list, int low_index, int high_index, int use_name, int use_weight)
{
  int pivot_index = repartition(list, low_index, high_index, use_name, use_weight);
  if(pivot_index-1 > low_index) quick_sort_recursive(list, low_index, pivot_index-1, use_name, use_weight);
  if(high_index > pivot_index) quick_sort_recursive(list, pivot_index+1, high_index, use_name, use_weight);
}

/* Shell function for quick sort. */
void quick_sort(monster *list, int n, int use_name, int use_weight)
{
  printf("Quick sort %d monsters by %s...\n", n, use_name ? "name" : "weight");

  clear_stats();
  quick_sort_recursive(list, 0, n-1, use_name, use_weight);
  print_stats();
}

/* Bubble sort implementation */
void bubble_sort(monster *list, int n, int use_name, int use_weight)
{
  int i, j;
  printf("Bubble sort %d monsters by %s...\n", n, use_name ? "name" : "weight");

  clear_stats();
  for(i = n-1; i >= 0; i--){
    for(j = 0; j < i; j++){
      if((compare_monsters(&list[j], &list[j+1], use_name, use_weight)) == 1) {
          swap_monsters(list, j, j+1);
      }
    }
  }
  print_stats();
}

/* Highest-value finder for selection sort. */
int find_highest(monster *list, int n, int use_name, int use_weight)
{
  // temp monster
  monster m;

  // temp monster will be < than any monsters in the list
  m.weight = INT_MIN;
  strcpy(m.name, "Monster #0");

  // what we'll be returning
  int highest_loc;

  // loops through whole list
  for(int i = 0; i <= n; i++){

    // compares monster at i to temp m monster
    if(compare_monsters(&list[i], &m, use_name, use_weight) == 1){
        highest_loc = i;
        m = list[i];
    }
  }

  return highest_loc;
}

/* Implement ascending selection sort. */
void selection_sort(monster *list, int n, int use_name, int use_weight)
{
  // var for loop and highest
  int i, highest;

  printf("Selection sort %d monsters by %s...\n", n, use_name ? "name" : "weight");
  clear_stats();

  // loops through the whole list
  for(i = n-1; i > 0; i--){
    highest = find_highest(list, i, use_name, use_weight); // stores highest value
    if(highest != i) swap_monsters(list, highest, i); // swap monsters
  }

  print_stats();
}

/* Find position for insertion sort. */
int insertion_sort_find_position(monster *list, int low_index, int high_index, monster *k, int use_name, int use_weight)
{
  // j holds val of high_index
  int j = high_index;

  // compare monsters until j is less than zero or list[j] > k
  while(j >= 0 && (compare_monsters(&list[j], k, use_name, use_weight) == 1)){
    list[j + 1] = list[j];
    j--; Copies++;
  }

  // returns the position
  return j + 1;
}

/* Insertion sort implementation. */
void insertion_sort_internal(monster *list, int n, int use_name, int use_weight)
{
  // var used to find position
  int low_index = 0, high_index;

  // loops through the whole list
  for(int i = 1; i < n; i++){
    monster k = list[i]; // temp key
    high_index = i - 1; // current position

    // find position and stores it
    low_index = insertion_sort_find_position(list, low_index, high_index, &k, use_name, use_weight);
    list[low_index] = k; Block_Copies++;
  }
}

/* Shell for insertion sort. */
void insertion_sort(monster *list, int n, int use_name, int use_weight)
{
  clear_stats();
  printf("Insertion sort %d monsters by %s...\n", n, use_name ? "name" : "weight");
  insertion_sort_internal(list, n, use_name, use_weight);
  print_stats();
}

/* Merge function for merge sort.  Merges sub-arrays [l1..h1] and [l2..h2]. */
void merge_sort_merge(monster *list, int l1, int h1, int l2, int h2, int use_name, int use_weight)
{
  // var for loops
  int i, l = 0, r = 0;

  // calculate the length of the left and right temp array
  int l_length = h1 - l1 + 1;
  int r_length = h2 - h1;

  // dynamically allocate space for the left and right temp array
  monster *left = malloc(sizeof(monster) * l_length);
  monster *right = malloc(sizeof(monster) * r_length);

  // sublist one values
  for(i = 0; i < l_length; i++){
    left[i] = list[l1 + i]; Copies++;
  }

  // sublist two values
  for(i = 0; i < r_length; i++){
    right[i] = list[h1 + 1 + i]; Copies++;
  }

  // compares left and right, puts the value in order
  for(i = l1; i <= h2; i++){
    Copies++;

    if((l < l_length) && (r >= r_length || (compare_monsters(&right[r], &left[l], use_name, use_weight) == 1))){
        list[i] = left[l]; l++;
    } else {
        list[i] = right[r]; r++;
    }
  }

  Mallocs++;

  // deallocate space in the heap
  free(left);
  free(right);
}

/* Recursive function for merge sort. */
void merge_sort_recursive(monster *list, int low_index, int high_index, int use_name, int use_weight)
{
  int middle; Block_Copies++;

  if(low_index < high_index){
    middle = (low_index + high_index) / 2;

    // creates sublists and merges them
    merge_sort_recursive(list, low_index, middle, use_name, use_weight);
    merge_sort_recursive(list, middle + 1, high_index, use_name, use_weight);
    merge_sort_merge(list, low_index, middle, middle + 1, high_index, use_name, use_weight);
  }
}

/* Merge sort implementation. */
void merge_sort(monster *list, int n, int use_name, int use_weight)
{
  printf("Merge sort %d monsters...\n", n);
  clear_stats(); Block_Copies = -1;
  merge_sort_recursive(list, 0, n-1, use_name, use_weight);
  print_stats();
}

/* Recursive function for merge-insertion sort. */
void merge_insertion_sort_recursive(monster *list, int low_index, int high_index, int use_name, int use_weight)
{
  int middle; Block_Copies++;

  if(low_index < high_index){ // runs until low_index > high_index
    middle = (low_index + high_index) / 2;
    merge_insertion_sort_recursive(list, low_index, middle, use_name, use_weight); // create sublist one
    merge_insertion_sort_recursive(list, middle + 1, high_index, use_name, use_weight); // create sublist two

    if(high_index - low_index <= 25){   // insertion sort if n in sub-list < 25
        insertion_sort_internal(list + low_index, high_index + 1 - low_index , use_name, use_weight);
    } else{  // merge sublist if n in sub-list is > 25
        merge_sort_merge(list, low_index, middle, middle + 1, high_index, use_name, use_weight);
    }
  }
}

/*  Merge-insertion sort implementation. */
void merge_insertion_sort(monster *list, int n, int use_name, int use_weight)
{
  printf("Merge-insertion sort %d monsters...\n", n);
  clear_stats(); Block_Copies = -1;
  merge_insertion_sort_recursive(list, 0, n-1, use_name, use_weight);
  print_stats();
}
