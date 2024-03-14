#include "s21_model.hpp"

namespace s21 {
Model::Model(const std::string filename) : path_(filename){};

Model::~Model() {
  FreePoints();
  RemoveMatrix();
  RemovePolygons();
};

output Model::ParserFirstReadFile() {
  output res = OK;
  std::fstream fin(path_, std::fstream::in);
  if (!fin) {
    res = ERROR;
  }
  char ch;
  if (res == OK) {
    while (fin.get(ch) && ch != EOF) {
      if (ch == 'v' && fin.get(ch) && ch == ' ') {
        ++cube_data_.count_of_vertexes;
      } else if (ch == 'f' && fin.get(ch) && ch == ' ') {
        ++cube_data_.count_of_facets;
      }
    }
  }
  return res;
}

size_t Model::ParserCountOfVertexesInStr(const std::string &str) {
  size_t count_of_vertexes = 0;
  bool ver_found = false;

  for (auto iter = str.begin(); iter != str.end(); ++iter) {
    if (*iter >= '0' && *iter <= '9' && !ver_found) {
      ++count_of_vertexes;
      ver_found = true;
    } else if (*iter == ' ') {
      ver_found = false;
    }
  }
  return count_of_vertexes;
}

void Model::ParserSecondReadFile() {
  std::fstream fin(path_, std::fstream::in);
  std::string getline_str;
  size_t count_of_vertexes = 0, count_of_polygons = 0;
  while (std::getline(fin, getline_str)) {
    char mode;
    std::istringstream iss(getline_str);
    iss >> mode;
    if (mode == 'v') {
      iss >> cube_data_.matrix_3d.matrix[count_of_vertexes][0] >>
          cube_data_.matrix_3d.matrix[count_of_vertexes][1] >>
          cube_data_.matrix_3d.matrix[count_of_vertexes][2];
      ++count_of_vertexes;
    } else if (mode == 'f') {
#define vert_in_fac \
  cube_data_.polygons[count_of_polygons].number_of_vertexes_in_facets
      vert_in_fac = ParserCountOfVertexesInStr(getline_str);
      cube_data_.polygons[count_of_polygons].vertexes =
          new size_t[vert_in_fac]();
      for (size_t i = 0; i != vert_in_fac; ++i) {
        iss >> cube_data_.polygons[count_of_polygons].vertexes[i];
      }
      ++count_of_polygons;
    }
  }
}

output Model::PrepareData() {
  output status = ParserFirstReadFile();
  if (status == OK) {
    cube_data_.matrix_3d.rows = cube_data_.count_of_vertexes;
    cube_data_.matrix_3d.cols = 3;
    cube_data_.matrix_3d.matrix = new double *[cube_data_.matrix_3d.rows]();
    for (size_t i = 0; i < cube_data_.matrix_3d.rows; ++i) {
      cube_data_.matrix_3d.matrix[i] = new double[cube_data_.matrix_3d.cols]();
    }
    cube_data_.polygons = new polygon_t[cube_data_.count_of_facets]();
    ParserSecondReadFile();
  }
  return status;
}

output Model::Translate(double move_x, double move_y, double move_z) {
  if (!cube_data_.matrix_3d.matrix) {
    return ERROR;
  }
  for (size_t i = 0; i < cube_data_.matrix_3d.rows; ++i) {
    cube_data_.matrix_3d.matrix[i][OX] =
        cube_data_.matrix_3d.matrix[i][OX] + move_x;
    cube_data_.matrix_3d.matrix[i][OY] =
        cube_data_.matrix_3d.matrix[i][OY] + move_y;
    cube_data_.matrix_3d.matrix[i][OZ] =
        cube_data_.matrix_3d.matrix[i][OZ] + move_z;
  }
  return OK;
}

output Model::Rotate(axis axis, double angle) {
  double temp_x, temp_y, temp_z;
  if (!cube_data_.matrix_3d.matrix) {
    return ERROR;
  }
  double sin_val = std::sin(angle);
  double cos_val = std::cos(angle);
  for (size_t i = 0; i < cube_data_.matrix_3d.rows; ++i) {
    if (axis == OX) {
      temp_y = cube_data_.matrix_3d.matrix[i][OY];
      temp_z = cube_data_.matrix_3d.matrix[i][OZ];
      cube_data_.matrix_3d.matrix[i][OY] = temp_y * cos_val - temp_z * sin_val;
      cube_data_.matrix_3d.matrix[i][OZ] = temp_y * sin_val + temp_z * cos_val;
    } else if (axis == OY) {
      temp_x = cube_data_.matrix_3d.matrix[i][OX];
      temp_z = cube_data_.matrix_3d.matrix[i][OZ];
      cube_data_.matrix_3d.matrix[i][OX] = temp_x * cos_val + temp_z * sin_val;
      cube_data_.matrix_3d.matrix[i][OZ] = -temp_x * sin_val + temp_z * cos_val;
    } else if (axis == OZ) {
      temp_x = cube_data_.matrix_3d.matrix[i][OX];
      temp_y = cube_data_.matrix_3d.matrix[i][OY];
      cube_data_.matrix_3d.matrix[i][OX] = temp_x * cos_val - temp_y * sin_val;
      cube_data_.matrix_3d.matrix[i][OY] = temp_x * sin_val + temp_y * cos_val;
    }
  }

  return OK;
}

output Model::Scale(double mult_x, double mult_y, double mult_z) {
  if (!cube_data_.matrix_3d.matrix) {
    return ERROR;
  }
  for (size_t i = 0; i < cube_data_.matrix_3d.rows; ++i) {
    cube_data_.matrix_3d.matrix[i][OX] =
        cube_data_.matrix_3d.matrix[i][OX] * mult_x;
    cube_data_.matrix_3d.matrix[i][OY] =
        cube_data_.matrix_3d.matrix[i][OY] * mult_y;
    cube_data_.matrix_3d.matrix[i][OZ] =
        cube_data_.matrix_3d.matrix[i][OZ] * mult_z;
  }
  return OK;
}

s21::data &Model::GetCubeData() { return cube_data_; }

s21::point **Model::GetPoints() { return points_; }

void Model::RemoveMatrix() {
  if (cube_data_.matrix_3d.matrix) {
    for (size_t i = 0; i < cube_data_.matrix_3d.rows; ++i) {
      delete[] cube_data_.matrix_3d.matrix[i];
    }
    delete[] cube_data_.matrix_3d.matrix;
    cube_data_.matrix_3d.matrix = nullptr;
    cube_data_.matrix_3d.rows = 0;
    cube_data_.matrix_3d.cols = 0;
  }
}

void Model::RemovePolygons() {
  if (cube_data_.polygons) {
    for (size_t i = 0; i < cube_data_.count_of_facets; ++i) {
      delete[] cube_data_.polygons[i].vertexes;
    }
    delete[] cube_data_.polygons;
    cube_data_.polygons = nullptr;
  }
}

void Model::CombineFacesWithVertexes() {
  points_ = new point *[cube_data_.count_of_facets];
  for (size_t i = 0; i < cube_data_.count_of_facets; ++i) {
    points_[i] = new point[cube_data_.polygons[i].number_of_vertexes_in_facets];
  }

  for (size_t i = 0; i < cube_data_.count_of_facets; ++i) {
    for (size_t j = 0; j < cube_data_.polygons[i].number_of_vertexes_in_facets;
         ++j) {
      points_[i][j].ox =
          cube_data_.matrix_3d
              .matrix[cube_data_.polygons[i].vertexes[j] - 1][OX];
      points_[i][j].oy =
          cube_data_.matrix_3d
              .matrix[cube_data_.polygons[i].vertexes[j] - 1][OY];
      points_[i][j].oz =
          cube_data_.matrix_3d
              .matrix[cube_data_.polygons[i].vertexes[j] - 1][OZ];
    }
  }
}

void Model::FreePoints() {
  if (points_) {
    for (size_t i = 0; i < cube_data_.count_of_facets; ++i) {
      delete[] points_[i];
    }
    delete[] points_;
    points_ = nullptr;
  }
}
}  // namespace s21
