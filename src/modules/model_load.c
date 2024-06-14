#include "model_load.h"

// Парсинг файла .obj
int parse_obj(char *filename, obj_t *data) {
  if (filename == NULL || data == NULL) return ERROR;

  int status = OK;
  FILE *file = fopen(filename, "r");
  if (file) {
    count_vertices_and_indices(file, data);
    status = memory_alloc(data);
    if (status != ERROR) {
      fseek(file, 0, SEEK_SET);
      status = parse_vertices_and_indices(file, data);
    }
    fclose(file);
  } else {
    status = ERROR;
  }

  return status;
}

// Подсчёт вершин и рёбер
void count_vertices_and_indices(FILE *file, obj_t *data) {
  size_t len = 0;
  char *line = 0;

  while (getline(&line, &len, file) != EOF) {
    if (strncmp(line, "v ", 2) == 0) {
      data->vertices_count++;
    } else if (strncmp(line, "f ", 2) == 0) {
      char *vertex_index = strtok(line + 2, " ");
      while (vertex_index != NULL) {
        if (atoi(vertex_index)) {
          data->vertex_indices_count++;
        }
        vertex_index = strtok(NULL, " ");
      }
    }
  }

  if (line) {
    free(line);
    line = NULL;
  }
}

// Запись вершин и рёбр в объект
int parse_vertices_and_indices(FILE *file, obj_t *data) {
  int status = OK;
  char *line = NULL;
  size_t len = 0;
  size_t i = 0;
  size_t vertex_counter = 0;
  size_t vertex_indices_counter = 0;

  while (getline(&line, &len, file) != EOF) {
    if (strncmp(line, "v ", 2) == 0) {
      vertex_counter++;
      double x, y, z;
      sscanf(line, "v %lf %lf %lf", &x, &y, &z);
      data->vertices_arr[i++] = x;
      data->vertices_arr[i++] = y;
      data->vertices_arr[i++] = z;
    } else if (strncmp(line, "f ", 2) == 0) {
      int first_index = 0;
      bool is_first_index = false;
      char *vertex_index = strtok(line + 2, " ");
      while (vertex_index != NULL) {
        int index_val = atoi(vertex_index);
        if (index_val) {
          if (index_val < 0) index_val += vertex_counter + 1;
          data->vertex_indices_arr[vertex_indices_counter] = index_val - 1;
          if (!is_first_index) {
            first_index = index_val - 1;
            is_first_index = true;
          } else {
            data->vertex_indices_arr[++vertex_indices_counter] = index_val - 1;
          }
          vertex_indices_counter++;
        }
        vertex_index = strtok(NULL, " ");
      }
      data->vertex_indices_arr[vertex_indices_counter] = first_index;
      vertex_indices_counter++;
    }
  }

  if (line) {
    free(line);
    line = NULL;
  }

  if (!data->vertices_arr[data->vertices_count * 3 - 1] ||
      !data->vertex_indices_arr[data->vertex_indices_count * 2 - 1]) {
    status = ERROR;
  }

  return status;
}

// Выделение памяти для модели
int memory_alloc(obj_t *data) {
  int status = OK;

  if (data->vertices_count)
    data->vertices_arr = calloc(data->vertices_count * 3, sizeof(double));

  if (data->vertex_indices_count)
    data->vertex_indices_arr =
        calloc(data->vertex_indices_count * 2, sizeof(int));

  if (data->vertices_arr == NULL || data->vertex_indices_arr == NULL)
    status = ERROR;

  return status;
}

// Очистка памяти модели
void data_free(obj_t *data) {
  if (data != NULL) {
    if (data->vertices_arr != NULL) {
      free(data->vertices_arr);
      data->vertices_arr = NULL;
    }
    if (data->vertex_indices_arr != NULL) {
      free(data->vertex_indices_arr);
      data->vertex_indices_arr = NULL;
    }
  }
}
