#include <check.h>

#include "3d_viewer.h"

#define TOLERANCE 1e-6

START_TEST(test_parser_1) {
  char *file_name = "test_objs/cube.obj";
  data cube_data = {0};
  output first_parse = S21_PrepareData(file_name, &cube_data);
  ck_assert_int_eq(first_parse, OK);
  matrix_t parsed_data = cube_data.matrix_3d;
  float vertexes[] = {1.0, -1.0, -1.0, 1.0,  -1.0, 1.0,  -1.0, -1.0,
                      1.0, -1.0, -1.0, -1.0, 1.0,  1.0,  -1.0, 1.0,
                      1.0, 1.0,  -1.0, 1.0,  1.0,  -1.0, 1.0,  -1.0};

  for (size_t i = 0; i < (sizeof(vertexes) / sizeof(vertexes[0]) / 3); ++i) {
    ck_assert_double_eq_tol(parsed_data.matrix[i][OX], vertexes[i * 3],
                            TOLERANCE);
    ck_assert_double_eq_tol(parsed_data.matrix[i][OY], vertexes[i * 3 + 1],
                            TOLERANCE);
    ck_assert_double_eq_tol(parsed_data.matrix[i][OZ], vertexes[i * 3 + 2],
                            TOLERANCE);
  }

  int facets[] = {2, 3, 4, 8, 7, 6, 5, 6, 2, 6, 7, 3, 3, 7, 8, 1, 4, 8,
                  1, 2, 4, 5, 8, 6, 1, 5, 2, 2, 6, 3, 4, 3, 8, 5, 1, 8};

  polygon_t *facetes_data = cube_data.polygons;
  for (size_t i = 0; i < (sizeof(facets) / sizeof(facets[0]) / 3); ++i) {
    for (int j = 0; j < facetes_data[i].numbers_of_vertexes_in_facets; ++j) {
      ck_assert_int_eq(facetes_data[i].vertexes[j], facets[i * 3 + j]);
    }
  }

  point **combined_data = S21_CombineFacetsWithVertexes(&cube_data);

  double points[] = {
      1.0,  -1.0, 1.0,  -1.0, -1.0, 1.0,  -1.0, -1.0, -1.0, -1.0, 1.0,  -1.0,
      -1.0, 1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  -1.0, 1.0,  1.0,  1.0,
      1.0,  -1.0, 1.0,  1.0,  1.0,  1.0,  -1.0, 1.0,  1.0,  -1.0, -1.0, 1.0,
      -1.0, -1.0, 1.0,  -1.0, 1.0,  1.0,  -1.0, 1.0,  -1.0, 1.0,  -1.0, -1.0,
      -1.0, -1.0, -1.0, -1.0, 1.0,  -1.0, 1.0,  -1.0, -1.0, 1.0,  -1.0, 1.0,
      -1.0, -1.0, -1.0, 1.0,  1.0,  -1.0, -1.0, 1.0,  -1.0, 1.0,  1.0,  1.0,
      1.0,  -1.0, -1.0, 1.0,  1.0,  -1.0, 1.0,  -1.0, 1.0,  1.0,  -1.0, 1.0,
      1.0,  1.0,  1.0,  -1.0, -1.0, 1.0,  -1.0, -1.0, -1.0, -1.0, -1.0, 1.0,
      -1.0, 1.0,  -1.0, 1.0,  1.0,  -1.0, 1.0,  -1.0, -1.0, -1.0, 1.0,  -1.0};

  for (int i = 0; i < cube_data.count_of_facets; ++i) {
    for (int j = 0; j < cube_data.polygons[i].numbers_of_vertexes_in_facets;
         ++j) {
      ck_assert_double_eq_tol(combined_data[i][j].ox, points[i * 9 + j * 3],
                              TOLERANCE);
      ck_assert_double_eq_tol(combined_data[i][j].oy, points[i * 9 + 1 + j * 3],
                              TOLERANCE);
      ck_assert_double_eq_tol(combined_data[i][j].oz, points[i * 9 + 2 + j * 3],
                              TOLERANCE);
    }
  }

  S21_FreePoints(combined_data, &cube_data);
  S21_RemoveMatrix(&cube_data.matrix_3d);
  S21_RemovePolygons(cube_data.polygons, cube_data.count_of_facets);
}

START_TEST(test_affine_rotate) {
  char *file_name = "test_objs/cube.obj";
  data cube_data = {0};
  S21_PrepareData(file_name, &cube_data);
  int status = S21_Rotate(&cube_data.matrix_3d, OX, 45);
  ck_assert_int_eq(status, OK);
  status = S21_Rotate(&cube_data.matrix_3d, OY, 45);
  ck_assert_int_eq(status, OK);
  status = S21_Rotate(&cube_data.matrix_3d, OZ, 45);
  ck_assert_int_eq(status, OK);
  char *file_name_rotated = "test_objs/cube_rotated.obj";
  data cube_rotated_data = {0};
  S21_PrepareData(file_name_rotated, &cube_rotated_data);

  for (int i = 0; i < cube_data.matrix_3d.rows; ++i) {
    for (int j = 0; j < cube_data.matrix_3d.cols; ++j) {
      ck_assert_double_eq_tol(cube_data.matrix_3d.matrix[i][j],
                              cube_rotated_data.matrix_3d.matrix[i][j],
                              TOLERANCE);
    }
  }

  S21_RemoveMatrix(&cube_data.matrix_3d);
  S21_RemovePolygons(cube_data.polygons, cube_data.count_of_facets);
  S21_RemoveMatrix(&cube_rotated_data.matrix_3d);
  S21_RemovePolygons(cube_rotated_data.polygons,
                     cube_rotated_data.count_of_facets);
}

START_TEST(test_affine_translate) {
  char *file_name = "test_objs/cube.obj";
  data cube_data = {0};
  S21_PrepareData(file_name, &cube_data);
  int status = S21_Translate(&cube_data.matrix_3d, 5, 6, 9);
  ck_assert_int_eq(status, OK);
  char *file_name_rotated = "test_objs/cube_translated.obj";
  data cube_rotated_data = {0};
  S21_PrepareData(file_name_rotated, &cube_rotated_data);

  for (int i = 0; i < cube_data.matrix_3d.rows; ++i) {
    for (int j = 0; j < cube_data.matrix_3d.cols; ++j) {
      ck_assert_double_eq_tol(cube_data.matrix_3d.matrix[i][j],
                              cube_rotated_data.matrix_3d.matrix[i][j],
                              TOLERANCE);
    }
  }

  S21_RemoveMatrix(&cube_data.matrix_3d);
  S21_RemovePolygons(cube_data.polygons, cube_data.count_of_facets);
  S21_RemoveMatrix(&cube_rotated_data.matrix_3d);
  S21_RemovePolygons(cube_rotated_data.polygons,
                     cube_rotated_data.count_of_facets);
}

START_TEST(test_affine_scale) {
  char *file_name = "test_objs/cube.obj";
  data cube_data = {0};
  S21_PrepareData(file_name, &cube_data);
  int status = S21_Scale(&cube_data.matrix_3d, 1, 10, -0.25);
  ck_assert_int_eq(status, OK);
  char *file_name_rotated = "test_objs/cube_scaled.obj";
  data cube_rotated_data = {0};
  S21_PrepareData(file_name_rotated, &cube_rotated_data);

  for (int i = 0; i < cube_data.matrix_3d.rows; ++i) {
    for (int j = 0; j < cube_data.matrix_3d.cols; ++j) {
      ck_assert_double_eq_tol(cube_data.matrix_3d.matrix[i][j],
                              cube_rotated_data.matrix_3d.matrix[i][j],
                              TOLERANCE);
    }
  }

  S21_RemoveMatrix(&cube_data.matrix_3d);
  S21_RemovePolygons(cube_data.polygons, cube_data.count_of_facets);
  S21_RemoveMatrix(&cube_rotated_data.matrix_3d);
  S21_RemovePolygons(cube_rotated_data.polygons,
                     cube_rotated_data.count_of_facets);
}

START_TEST(wrong_data) {
  int status = S21_PrepareData("wljndjajwbgjw.obj", NULL);
  ck_assert_int_eq(status, ERROR);
}

START_TEST(wrong_file_contents) {
  data data = {0};
  int status = S21_PrepareData("sjkdfnsadg", &data);
  ck_assert_int_eq(status, ERROR);
}

START_TEST(nulls) {
  int status = S21_Translate(NULL, 1, 1, 1);
  ck_assert_int_eq(status, ERROR);
  status = S21_Rotate(NULL, OX, 0);
  ck_assert_int_eq(status, ERROR);
  status = S21_Scale(NULL, 1, 1, 1);
  ck_assert_int_eq(status, ERROR);
}

int main(void) {
  Suite *s = suite_create("Core");
  SRunner *runner = srunner_create(s);
  TCase *tc_core = tcase_create("Core");

  int no_failed = 0;
  tcase_add_test(tc_core, test_parser_1);
  tcase_add_test(tc_core, test_affine_rotate);
  tcase_add_test(tc_core, test_affine_translate);
  tcase_add_test(tc_core, test_affine_scale);
  tcase_add_test(tc_core, wrong_data);
  tcase_add_test(tc_core, wrong_file_contents);
  tcase_add_test(tc_core, nulls);

  suite_add_tcase(s, tc_core);
  srunner_run_all(runner, CK_NORMAL);
  no_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
