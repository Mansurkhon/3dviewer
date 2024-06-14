#include "transformations.h"

void scale(obj_t *data, const double n) {
  if (n != 0)
    for (size_t i = 0; i < data->vertices_count * 3; i++)
      data->vertices_arr[i] *= n;
}

void move_X(obj_t *data, const double n) {
  for (size_t i = 0; i < data->vertices_count * 3; i += 3)
    data->vertices_arr[i] += n;
}

void move_Y(obj_t *data, const double n) {
  for (size_t i = 1; i < data->vertices_count * 3; i += 3)
    data->vertices_arr[i] += n;
}

void move_Z(obj_t *data, const double n) {
  for (size_t i = 2; i < data->vertices_count * 3; i += 3)
    data->vertices_arr[i] += n;
}

void rotate_X(obj_t *data, double angle) {
  angle = angle * PI / 180;
  for (size_t i = 0; i < data->vertices_count * 3; i += 3) {
    double y = data->vertices_arr[i + 1];
    double z = data->vertices_arr[i + 2];
    data->vertices_arr[i + 1] = y * cos(angle) + z * sin(angle);
    data->vertices_arr[i + 2] = -y * sin(angle) + z * cos(angle);
  }
}

void rotate_Y(obj_t *data, double angle) {
  angle = angle * PI / 180;
  for (size_t i = 0; i < data->vertices_count * 3; i += 3) {
    double x = data->vertices_arr[i];
    double z = data->vertices_arr[i + 2];
    data->vertices_arr[i] = x * cos(angle) + z * sin(angle);
    data->vertices_arr[i + 2] = -x * sin(angle) + z * cos(angle);
  }
}

void rotate_Z(obj_t *data, double angle) {
  angle = angle * PI / 180;
  for (size_t i = 0; i < data->vertices_count * 3; i += 3) {
    double x = data->vertices_arr[i];
    double y = data->vertices_arr[i + 1];
    data->vertices_arr[i] = x * cos(angle) + y * sin(angle);
    data->vertices_arr[i + 1] = -x * sin(angle) + y * cos(angle);
  }
}
