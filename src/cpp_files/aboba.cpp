#include <iostream>

#include "s21_model.hpp"

int main(void) {
  s21::Model model("../test_objs/cube.obj");
  s21::Model::output status = model.PrepareData();
  std::cout << status << '\n';
}