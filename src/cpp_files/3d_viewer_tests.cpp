#include <gtest/gtest.h>

#include <iostream>
#include <vector>

#include "s21_model.hpp"

#define TOLERANCE 1e-6

TEST(Parser, Test1) {
  s21::Model model("test_objs/cube.obj");
  s21::Model::output status = model.PrepareData();
  ASSERT_EQ(status, s21::Model::OK);
  std::vector<double> vertexes = {
      1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, -1.0,
      1.0, 1.0,  -1.0, 1.0, 1.0,  1.0, -1.0, 1.0,  1.0, -1.0, 1.0,  -1.0};
  auto cube_data = model.GetCubeData();
  for (size_t i = 0; i < vertexes.size() / 3; ++i) {
    ASSERT_DOUBLE_EQ(cube_data.matrix_3d.matrix[i][model.OX], vertexes[i * 3]);
    ASSERT_DOUBLE_EQ(cube_data.matrix_3d.matrix[i][model.OY],
                     vertexes[i * 3 + 1]);
    ASSERT_DOUBLE_EQ(cube_data.matrix_3d.matrix[i][model.OZ],
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

  model.CombineFacesWithVertexes();
  auto combined_data = model.GetPoints();

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
  s21::Model model("test_objs/cube.obj");
  model.PrepareData();
  s21::Model::output status = model.Rotate(model.OX, 45);
  ASSERT_EQ(status, s21::Model::OK);
  status = model.Rotate(model.OY, 45);
  ASSERT_EQ(status, s21::Model::OK);
  status = model.Rotate(model.OZ, 45);
  ASSERT_EQ(status, s21::Model::OK);
  auto cube_data = model.GetCubeData();
  s21::Model cube_rotated_model("test_objs/cube_rotated.obj");
  cube_rotated_model.PrepareData();
  auto cube_rotated = cube_rotated_model.GetCubeData();
  for (size_t i = 0; i < cube_data.matrix_3d.rows; ++i) {
    for (size_t j = 0; j < cube_data.matrix_3d.cols; ++j) {
      EXPECT_NEAR(cube_data.matrix_3d.matrix[i][j],
                  cube_rotated.matrix_3d.matrix[i][j], TOLERANCE);
    }
  }
}

TEST(Affine, Translate) {
  s21::Model model("test_objs/cube.obj");
  model.PrepareData();
  s21::Model::output status = model.Translate(5, 6, 9);
  ASSERT_EQ(status, s21::Model::OK);
  auto cube_data = model.GetCubeData();
  s21::Model cube_translated_model("test_objs/cube_translated.obj");
  cube_translated_model.PrepareData();
  auto cube_translated = cube_translated_model.GetCubeData();
  for (size_t i = 0; i < cube_data.matrix_3d.rows; ++i) {
    for (size_t j = 0; j < cube_data.matrix_3d.cols; ++j) {
      EXPECT_NEAR(cube_data.matrix_3d.matrix[i][j],
                  cube_translated.matrix_3d.matrix[i][j], TOLERANCE);
    }
  }
}

TEST(Affine, Scale) {
  s21::Model model("test_objs/cube.obj");
  model.PrepareData();
  s21::Model::output status = model.Scale(1, 10, -0.25);
  ASSERT_EQ(status, s21::Model::OK);
  auto cube_data = model.GetCubeData();
  s21::Model cube_translated_model("test_objs/cube_scaled.obj");
  cube_translated_model.PrepareData();
  auto cube_translated = cube_translated_model.GetCubeData();
  for (size_t i = 0; i < cube_data.matrix_3d.rows; ++i) {
    for (size_t j = 0; j < cube_data.matrix_3d.cols; ++j) {
      EXPECT_NEAR(cube_data.matrix_3d.matrix[i][j],
                  cube_translated.matrix_3d.matrix[i][j], TOLERANCE);
    }
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}