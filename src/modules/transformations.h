#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_3dviewer.h"

#define PI 3.14159265358979323846

void move_X(obj_t *data, const double n);
void move_Y(obj_t *data, const double n);
void move_Z(obj_t *data, const double n);

void scale(obj_t *data, const double n);

void rotate_X(obj_t *data, double angle);
void rotate_Y(obj_t *data, double angle);
void rotate_Z(obj_t *data, double angle);

#endif  // TRANSFORMATIONS_H
