#include <gtest/gtest.h>

#include <iostream>
#include <vector>

#include "s21_controller.hpp"

#define TOLERANCE 1e-6

TEST(Parser, Test1) {
  s21::Controller controller;
  controller.SetFilePath("test_objs/cube.obj");
  s21::output status = controller.PrepareData();
  ASSERT_EQ(status, s21::OK);
  std::vector<double> vertexes = {
      1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, -1.0,
      1.0, 1.0,  -1.0, 1.0, 1.0,  1.0, -1.0, 1.0,  1.0, -1.0, 1.0,  -1.0};
  auto cube_data = controller.GetCubeData();
  for (size_t i = 0; i < vertexes.size() / 3; ++i) {
    ASSERT_DOUBLE_EQ(cube_data.matrix_3d.matrix[i][s21::OX], vertexes[i * 3]);
    ASSERT_DOUBLE_EQ(cube_data.matrix_3d.matrix[i][s21::OY],
                     vertexes[i * 3 + 1]);
    ASSERT_DOUBLE_EQ(cube_data.matrix_3d.matrix[i][s21::OZ],
                     vertexes[i * 3 + 2]);
  }
  std::vector<int> facets = {2, 3, 4, 8, 7, 6, 5, 6, 2, 6, 7, 3,
                             3, 7, 8, 1, 4, 8, 1, 2, 4, 5, 8, 6,
                             1, 5, 2, 2, 6, 3, 4, 3, 8, 5, 1, 8};
  for (size_t i = 0; i < facets.size() / 3; ++i) {
    for (size_t j = 0; j < cube_data.polygons[i].number_of_vertexes_in_facets;
         ++j) {
      ASSERT_DOUBLE_EQ(cube_data.polygons[i].vertexes[j], facets[i * 3 + j]);
    }
  }

  controller.CombineFacesWithVertexes();
  auto combined_data = controller.GetPoints();

  std::vector<double> points = {
      1.0,  -1.0, 1.0,  -1.0, -1.0, 1.0,  -1.0, -1.0, -1.0, -1.0, 1.0,  -1.0,
      -1.0, 1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  -1.0, 1.0,  1.0,  1.0,
      1.0,  -1.0, 1.0,  1.0,  1.0,  1.0,  -1.0, 1.0,  1.0,  -1.0, -1.0, 1.0,
      -1.0, -1.0, 1.0,  -1.0, 1.0,  1.0,  -1.0, 1.0,  -1.0, 1.0,  -1.0, -1.0,
      -1.0, -1.0, -1.0, -1.0, 1.0,  -1.0, 1.0,  -1.0, -1.0, 1.0,  -1.0, 1.0,
      -1.0, -1.0, -1.0, 1.0,  1.0,  -1.0, -1.0, 1.0,  -1.0, 1.0,  1.0,  1.0,
      1.0,  -1.0, -1.0, 1.0,  1.0,  -1.0, 1.0,  -1.0, 1.0,  1.0,  -1.0, 1.0,
      1.0,  1.0,  1.0,  -1.0, -1.0, 1.0,  -1.0, -1.0, -1.0, -1.0, -1.0, 1.0,
      -1.0, 1.0,  -1.0, 1.0,  1.0,  -1.0, 1.0,  -1.0, -1.0, -1.0, 1.0,  -1.0};

  for (size_t i = 0; i < cube_data.count_of_facets; ++i) {
    for (size_t j = 0; j < cube_data.polygons[i].number_of_vertexes_in_facets;
         ++j) {
      ASSERT_DOUBLE_EQ(combined_data[i][j].ox, points[i * 9 + j * 3]);
      ASSERT_DOUBLE_EQ(combined_data[i][j].oy, points[i * 9 + 1 + j * 3]);
      ASSERT_DOUBLE_EQ(combined_data[i][j].oz, points[i * 9 + 2 + j * 3]);
    }
  }
}

TEST(Affine, Rotate) {
  s21::Controller controller;
  controller.SetFilePath("test_objs/cube.obj");
  controller.PrepareData();
  s21::output status = controller.Rotate(s21::OX, 45);
  ASSERT_EQ(status, s21::OK);
  status = controller.Rotate(s21::OY, 45);
  ASSERT_EQ(status, s21::OK);
  status = controller.Rotate(s21::OZ, 45);
  ASSERT_EQ(status, s21::OK);
  auto cube_data = controller.GetCubeData();
  s21::Controller cube_rotated_controller;
  cube_rotated_controller.SetFilePath("test_objs/cube_rotated.obj");
  cube_rotated_controller.PrepareData();
  auto cube_rotated = cube_rotated_controller.GetCubeData();
  for (size_t i = 0; i < cube_data.matrix_3d.rows; ++i) {
    for (size_t j = 0; j < cube_data.matrix_3d.cols; ++j) {
      EXPECT_NEAR(cube_data.matrix_3d.matrix[i][j],
                  cube_rotated.matrix_3d.matrix[i][j], TOLERANCE);
    }
  }
}

TEST(Affine, Translate) {
  s21::Controller controller;
  controller.SetFilePath("test_objs/cube.obj");
  controller.PrepareData();
  s21::output status = controller.Translate(5, 6, 9);
  ASSERT_EQ(status, s21::OK);
  auto cube_data = controller.GetCubeData();
  s21::Controller cube_translated_controller;
  cube_translated_controller.SetFilePath("test_objs/cube_translated.obj");
  cube_translated_controller.PrepareData();
  auto cube_translated = cube_translated_controller.GetCubeData();
  for (size_t i = 0; i < cube_data.matrix_3d.rows; ++i) {
    for (size_t j = 0; j < cube_data.matrix_3d.cols; ++j) {
      EXPECT_NEAR(cube_data.matrix_3d.matrix[i][j],
                  cube_translated.matrix_3d.matrix[i][j], TOLERANCE);
    }
  }
}

TEST(Affine, Scale) {
  s21::Controller controller;
  controller.SetFilePath("test_objs/cube.obj");
  controller.PrepareData();
  s21::output status = controller.Scale(1, 10, -0.25);
  ASSERT_EQ(status, s21::OK);
  auto cube_data = controller.GetCubeData();
  s21::Controller cube_translated_controller;
  cube_translated_controller.SetFilePath("test_objs/cube_scaled.obj");
  cube_translated_controller.PrepareData();
  auto cube_translated = cube_translated_controller.GetCubeData();
  for (size_t i = 0; i < cube_data.matrix_3d.rows; ++i) {
    for (size_t j = 0; j < cube_data.matrix_3d.cols; ++j) {
      EXPECT_NEAR(cube_data.matrix_3d.matrix[i][j],
                  cube_translated.matrix_3d.matrix[i][j], TOLERANCE);
    }
  }
}

TEST(Parser, FileDoesNotExist) {
  s21::Controller controller;
  controller.SetFilePath("wljndjajwbgjw.obj");
  s21::output status = controller.PrepareData();
  ASSERT_EQ(status, s21::ERROR);
}

TEST(Parser, FileLarge) {
  s21::Controller controller;
  controller.SetFilePath("test_objs/sword.obj");
  s21::output status = controller.PrepareData();
  ASSERT_EQ(status, s21::OK);
}

TEST(Affine, Nulls) {
  s21::Controller controller;
  controller.SetFilePath("test_objs/cube.obj");
  s21::output status = controller.Rotate(s21::OX, 45);
  ASSERT_EQ(status, s21::ERROR);
  status = controller.Translate(5, 6, 9);
  ASSERT_EQ(status, s21::ERROR);
  controller.Scale(1, 10, -0.25);
  ASSERT_EQ(status, s21::ERROR);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}