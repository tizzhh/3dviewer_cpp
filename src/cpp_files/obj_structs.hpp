#ifndef C8_3DVIEWER_V1_SRC_CPP_FILES_OBJ_STRUCTS_HPP
#define C8_3DVIEWER_V1_SRC_CPP_FILES_OBJ_STRUCTS_HPP

namespace s21 {
typedef struct Matrix {
  double** matrix = nullptr;
  size_t rows = 0;
  size_t cols = 0;
} matrix_t;

typedef struct facets {
  size_t* vertexes = nullptr;
  size_t number_of_vertexes_in_facets = 0;
} polygon_t;

typedef struct data {
  size_t count_of_vertexes = 0;
  size_t count_of_facets = 0;
  matrix_t matrix_3d;
  polygon_t* polygons = nullptr;
} data;

typedef struct point {
  double ox = 0;
  double oy = 0;
  double oz = 0;
} point;

typedef enum output { OK, ERROR } output;
typedef enum axis { OX, OY, OZ } axis;
}  // namespace s21

#endif