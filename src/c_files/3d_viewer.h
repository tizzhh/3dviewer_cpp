#ifndef C8_3DVIEWER_V1_SRC_3D_VIEWER_H
#define C8_3DVIEWER_V1_SRC_3D_VIEWER_H

#define _GNU_SOURCE
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Представление двумерной матрицы.
 *
 * Двумерная матрица со значениями типа double, создающаяся динамически.
 */
typedef struct Matrix {
  double** matrix; /**< Двумерный массив для хранения элементов матрицы. */
  int rows; /**< Количество строчек в матрице. */
  int cols; /**< Количество столбцов в матрице. */
} matrix_t;

/**
 * @brief Представление полигона, определенного поверхностями.
 *
 * Полигон определен массивом вершин и их количеством.
 */
typedef struct facets {
  int* vertexes; /**< Массив вершин поверхностей. */
  int numbers_of_vertexes_in_facets; /**< Количество вершин у полигона. */
} polygon_t;

/**
 * @brief Представление данных .obj файла.
 *
 * Предаствление .obj файла, включающего в себя:
 * количество вершин и поверхностей, матрицу вершин, массив полигонов.
 */
typedef struct data {
  int count_of_vertexes; /**< Общее количество вершин в .obj файле. */
  int count_of_facets; /**< Общее количество поверхностей в .obj файле. */
  matrix_t matrix_3d; /**< Представление вершин в виде матрицы. */
  polygon_t* polygons; /**< Массив всех полигонов .obj файла. */
} data;

/**
 * @brief Возможные исходы операции.
 *
 * Показывает, успешна ли прошла какая-либо операция.
 * 'OK' означает успех, 'ERROR' - ошибку.
 */
typedef enum output { OK, ERROR } output;

/**
 * @brief Структура точки с координатами.
 *
 */
typedef struct point {
  double ox;
  double oy;
  double oz;
} point;

/**
 * @brief Три оси техмерного пространства.
 *
 * Используется для определения рассматриваемой оси.
 */
typedef enum axis {
  OX, /**<  Ось x. */
  OY, /**<  Ось y. */
  OZ  /**<  Ось z. */
} axis;
// PARSER
/**
 * @brief Функция прочтения файла для получения данных о количестве поверхостей
 * и вершин.
 *
 * Функция подсчитывает общее количество вершин и поверхностей в .obj файле,
 * который указан в 'path'.
 *
 * @param path Путь к .obj файлу.
 * @param count_of_vertexes Указатель на переменную, храняющуюю количество
 * вершин.
 * @param count_of_facets Указатель на переменную, храняющую количство
 * поверхностей
 * @return 'output', означающий успех или ошибку чтения.
 *          - 'OK', если чтение успешно.
 *          - 'ERROR', если произошла ошибка.
 */
output S21_ParserFirstReadFile(char* path, int* count_of_vertexes,
                               int* count_of_facets);
/**
 * @brief Функция прочтения файла для заполнения структуры 'data'.
 *
 * Функция считывает указанный в 'path' .obj файл, заполняя при этом 'data'
 * вершинами и поверхностями.
 *
 * @param path Путь к .obj файлу.
 * @param data Указатель на переменную, храняющую информацию о .obj файле.
 */
void S21_ParserSecondReadFile(char* path, data* data);
/**
 * @brief Функция для подсчета количества цифр в строке.
 *
 * @param str Указатель на строку, в которой необходим подсчет.
 * @return size_t Количество подсчитанных цифр.
 */
size_t S21_ParserCountOfChars(char* str);
/**
 * @brief Функция подсчета количества вершин в строке.
 *
 * @param str Указатель на строку, в которой необходим подсчет.
 * @return size_t Количество подсчитанных вершин.
 */
size_t S21_ParserCountOFVertexesInStr(char* str);
/**
 * @brief Функция для выделения необходимой памяти.
 *
 * Функция выделяет необходимую память для матрицы вершин и полигонов.
 * Затем происходит вызов S21_ParserSecondReadFile, который и
 * заполняет данными выделлную память.
 *
 * @param path Путь к .obj файлу.
 * @param data Указатель на переменную, храняющую информацию о .obj файле.
 * @return 'output', означающий успех или ошибку чтения.
 *          - 'OK', если чтение успешно.
 *          - 'ERROR', если произошла ошибка.
 */
output S21_PrepareData(char* path, data* data);

// AFFINE
/**
 * @brief Функция для перемещения объекта в пространстве.
 *
 * @param vertices Укзаатель на 'matrix_t', хранящуюю матрицу вершин.
 * @param move_x На сколько передвигается объект по оси x.
 * @param move_y На сколько передвигается объект по оси y.
 * @param move_z На сколько передвигается объект по оси z.
 * @return 'output', означающий успех или ошибку чтения.
 *          - 'OK', если чтение успешно.
 *          - 'ERROR', если произошла ошибка.
 */
output S21_Translate(matrix_t* vertices, double move_x, double move_y,
                     double move_z);
/**
 * @brief Функция для повора объекта в пространстве.
 *
 * @param vertices Укзаатель на 'matrix_t', хранящуюю матрицу вершин.
 * @param axis По какой оси поварачивается объект.
 * @param angle На какой угол поворачивается объект.
 * @return 'output', означающий успех или ошибку чтения.
 *          - 'OK', если чтение успешно.
 *          - 'ERROR', если произошла ошибка.
 */
output S21_Rotate(matrix_t* vertices, axis axis, double angle);
/**
 * @brief Функция для масштабирования объекта в пространстве.
 *
 * @param vertices Укзаатель на 'matrix_t', хранящуюю матрицу вершин.
 * @param mult_x На сколько умножается координата x.
 * @param mult_y На сколько умножается координата y.
 * @param mult_z На сколько умножается координата z.
 * @return 'output', означающий успех или ошибку чтения.
 *          - 'OK', если чтение успешно.
 *          - 'ERROR', если произошла ошибка.
 */
output S21_Scale(matrix_t* vertices, double mult_x, double mult_y,
                 double mult_z);
/**
 * @brief Функция для освобождения выделенной памяти для 'matrix_t'.
 *
 * @param matrix Указатель на 'matrix_t', который необходимо освободить.
 */
void S21_RemoveMatrix(matrix_t* matrix);
/**
 * @brief Функция для освобождения выделенной памяти для 'polygom_t'.
 *
 * @param polygons Указатель на 'polygon_t', который необходимо освободить.
 * @param count_of_polygons Количество полигонов у объекта.
 */
void S21_RemovePolygons(polygon_t* polygons, int count_of_polygons);

/**
 * @brief Функция для создания матрицы всех точек полигонов.
 *
 * @param obj_data прочитанные из файла .obj данные.
 * @return point**  указатель на полученную матрицу.
 */
point** S21_CombineFacetsWithVertexes(const data* obj_data);

/**
 * @brief функция для удаления матрицы точек из памяти.
 *
 * @param points указатель на матрицу точек.
 * @param obj_data прочитанные из файла .obj данные.
 */
void S21_FreePoints(point** points, const data* obj_data);

#endif  // C8_3DVIEWER_V1_SRC_3D_VIEWER_H
