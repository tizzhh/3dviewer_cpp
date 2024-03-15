#include "s21_controller.hpp"

s21::output s21::Controller::PrepareData() {
    return model_.PrepareData();
}

s21::output s21::Controller::Translate(double move_x, double move_y,
                                       double move_z) {
  return model_.Translate(move_x, move_y, move_z);
}

s21::output s21::Controller::Rotate(s21::axis axis, double angle) {
  return model_.Rotate(axis, angle);
}

s21::output s21::Controller::Scale(double mult_x, double mult_y,
                                   double mult_z) {
  return model_.Scale(mult_x, mult_y, mult_z);
}

void s21::Controller::CombineFacesWithVertexes() {
  model_.CombineFacesWithVertexes();
}

void s21::Controller::SetFilePath(const std::string str) {
  model_.SetFilePath(str);
}

size_t s21::Controller::GetCountOfVertexes() { return model_.GetCountOfVertexes(); }

size_t s21::Controller::GetCountOfFacets() { return model_.GetCountOfFacets(); }

s21::data& s21::Controller::GetCubeData() { return model_.GetCubeData(); }

s21::point** s21::Controller::GetPoints() { return model_.GetPoints(); }
