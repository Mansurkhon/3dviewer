#ifndef S21_3DVIEWER_H
#define S21_3DVIEWER_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum statuses { OK, ERROR };

typedef struct data {
  size_t vertices_count;
  double* vertices_arr;
  size_t vertex_indices_count;
  int* vertex_indices_arr;
} obj_t;

#endif  // S21_3DVIEWER_H
