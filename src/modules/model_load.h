#ifndef MODEL_LOAD_H
#define MODEL_LOAD_H

#include "s21_3dviewer.h"

// Парсинг файла .obj
int parse_obj(char *filename, obj_t *data);

// Подсчёт вершин и рёбер
void count_vertices_and_indices(FILE *file, obj_t *data);

// Запись вершин и полигонов в объект
int parse_vertices_and_indices(FILE *file, obj_t *data);

// Выделение памяти для модели
int memory_alloc(obj_t *data);

// Очистка памяти модели
void data_free(obj_t *data);

#endif  // MODEL_LOAD_H
