#ifndef C8_3DVIEWER_V1_SRC_CPP_FILES_S21_MODEL_HPP
#define C8_3DVIEWER_V1_SRC_CPP_FILES_S21_MODEL_HPP

#include <cmath>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

#include "obj_structs.hpp"

namespace s21 {
class Model {
 public:
  Model(const std::string filename);
  ~Model();

  output ParserFirstReadFile();
  void ParserSecondReadFile();
  size_t ParserCountOfVertexesInStr(const std::string &str);
  output PrepareData();
  output Translate(double move_x, double move_y, double move_z);
  output Rotate(axis axis, double angle);
  output Scale(double mult_x, double mult_y, double mult_z);
  void CombineFacesWithVertexes();

  s21::data &GetCubeData();
  s21::point **GetPoints();
  void RemoveMatrix();
  void RemovePolygons();
  void FreePoints();

 private:
  s21::point **points_ = nullptr;
  s21::data cube_data_;
  const std::string path_;
};
};  // namespace s21

#endif