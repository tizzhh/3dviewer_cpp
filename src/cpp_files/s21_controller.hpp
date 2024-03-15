#ifndef C8_3DVIEWER_V1_SRC_CPP_FILES_S21_CONTROLLER_HPP
#define C8_3DVIEWER_V1_SRC_CPP_FILES_S21_CONTROLLER_HPP

#include "s21_model.hpp"

namespace s21 {
class Controller {
 public:
  Controller() = default;
  ~Controller() = default;

  output PrepareData();
  output Translate(double move_x, double move_y, double move_z);
  output Rotate(axis axis, double angle);
  output Scale(double mult_x, double mult_y, double mult_z);
  void CombineFacesWithVertexes();
  void SetFilePath(const std::string str);
  size_t GetCountOfVertexes();
  size_t GetCountOfFacets();

  s21::data &GetCubeData();
  s21::point **GetPoints();

 private:
  s21::Model model_;
};
};  // namespace s21

#endif