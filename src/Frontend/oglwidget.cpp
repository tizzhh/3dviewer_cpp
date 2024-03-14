#include "oglwidget.h"

#include <QKeyEvent>
#include <QtOpenGL>
#include <array>

#include "settings.h"
OGLWidget::OGLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

OGLWidget::~OGLWidget() { _settings->saveSettings(this); }

void OGLWidget::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void OGLWidget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}

void OGLWidget::paintGL() {
  glClearColor(background.redF(), background.greenF(), background.blueF(),
               background.alphaF());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  if (ortho) {
    glMatrixMode(GL_MODELVIEW);
    glOrtho(-100, 100, -100, 100, -100, 5000);
  } else {
    glMatrixMode(GL_PROJECTION);
    glFrustum(-10, 10, -10, 10, 10, 100);
  }

  glTranslated(0, 0, -50);
  glScalef(scale, scale, scale);
  glRotatef(_angleZ, 0, 0, 1);
  glRotatef(_angleY, 0, 1, 0);
  glRotatef(_angleX, 1, 0, 0);
  glTranslatef(coordX, coordY, coordZ);

  drawObject();
  if (isVertexEnable) {
    drawVertexes();
  }
}

void OGLWidget::createDrawbleData() {
  vecLines.clear();
  vecVertexes.clear();
  countVertexes = 0;
  point **combinedData;
  combinedData = S21_CombineFacetsWithVertexes(&dataObject);
  for (int i = 0; i < dataObject.count_of_facets; ++i) {
    countVertexes += dataObject.polygons[i].numbers_of_vertexes_in_facets;
    for (int j = 0; j < dataObject.polygons[i].numbers_of_vertexes_in_facets;
         ++j) {
      vecLines.push_back(combinedData[i][j].ox);
      vecLines.push_back(combinedData[i][j].oy);
      vecLines.push_back(combinedData[i][j].oz);
    }
  }
  for (int i = 0; i < dataObject.matrix_3d.rows; ++i) {
    for (int j = 0; j < dataObject.matrix_3d.cols; ++j) {
      vecVertexes.push_back(dataObject.matrix_3d.matrix[i][j]);
    }
  }
}

bool OGLWidget::getIsVertexCircle() const { return isVertexCircle; }

bool OGLWidget::getIsVertexEnable() const { return isVertexEnable; }

double OGLWidget::getVertexSize() const { return vertexSize; }

bool OGLWidget::getOrtho() const { return ortho; }

int OGLWidget::getLineSize() const { return lineSize; }

void OGLWidget::setOrtho(bool newOrtho) {
  ortho = newOrtho;
  update();
}

void OGLWidget::setVertexSize(int newVertexSize) {
  vertexSize = newVertexSize;
  update();
}

void OGLWidget::setLineSize(int newLineSize) {
  lineSize = newLineSize;
  update();
}

void OGLWidget::setIsVertexCircle(bool newIsVertexCircle) {
  isVertexCircle = newIsVertexCircle;
  update();
}

void OGLWidget::setIsVertexEnable(bool newIsVertexEnable) {
  isVertexEnable = newIsVertexEnable;
  update();
}

void OGLWidget::drawObject() {
  glLineWidth(lineSize);
  float lineWidth[2];
  glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidth);
  glLineStipple(1, 0x00FF);
  if (typeLine == 1) {
    //Пунктирная
    glEnable(GL_LINE_STIPPLE);
  } else
    glDisable(GL_LINE_STIPPLE);
  if (dataObject.count_of_vertexes != 0) {
    glColor3f(linesColor.redF(), linesColor.greenF(), linesColor.blueF());
    double *data = vecLines.data();
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, data);
    if (typeLine == 2)
      glDrawArrays(GL_TRIANGLES, 0, countVertexes);
    else
      glDrawArrays(GL_LINE_STRIP, 0, countVertexes);
    glDisableClientState(GL_VERTEX_ARRAY);
  }
  glGetError();
}

void OGLWidget::drawVertexes() {
  if (isVertexCircle)
    glEnable(GL_POINT_SMOOTH);
  else
    glDisable(GL_POINT_SMOOTH);
  glColor3f(vertexesColor.redF(), vertexesColor.greenF(),
            vertexesColor.blueF());
  double *data = vecVertexes.data();
  glPointSize(vertexSize);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_DOUBLE, 0, data);
  glDrawArrays(GL_POINTS, 0, vecVertexes.size() / 3);
  glDisableClientState(GL_VERTEX_ARRAY);
}

int OGLWidget::getTypeLine() const { return typeLine; }

void OGLWidget::setTypeLine(int newTypeLine) {
  typeLine = newTypeLine;
  update();
}

void OGLWidget::setSettings(Settings *settings) { _settings = settings; }

const QColor &OGLWidget::getVertexesColor() const { return vertexesColor; }

void OGLWidget::setVertexesColor(const QColor &newVertexesColor) {
  vertexesColor = newVertexesColor;
  update();
}

const QColor &OGLWidget::getLinesColor() const { return linesColor; }

void OGLWidget::setLinesColor(const QColor &newLinesColor) {
  linesColor = newLinesColor;
  update();
}

const QColor &OGLWidget::getBackground() const { return background; }

void OGLWidget::setBackground(const QColor &newBackground) {
  background = newBackground;
  update();
}

int OGLWidget::getCoordZ() const { return coordZ; }

int OGLWidget::getCoordY() const { return coordY; }

int OGLWidget::getCoordX() const { return coordX; }

double OGLWidget::getScale() const { return scale; }

void OGLWidget::setScale(double newScale) {
  scale = newScale;
  update();
}

void OGLWidget::setCoordZ(int newCoordZ) {
  coordZ = newCoordZ;
  update();
}

void OGLWidget::setCoordY(int newCoordY) {
  coordY = newCoordY;
  update();
}

void OGLWidget::setCoordX(int newCoordX) {
  coordX = newCoordX;
  update();
}

void OGLWidget::setAngleY(int newAngleY) {
  _angleY = newAngleY;
  update();
}

void OGLWidget::setAngleX(int newAngleX) {
  _angleX = newAngleX;
  update();
}

void OGLWidget::setAngleZ(int newAngleZ) {
  _angleZ = newAngleZ;
  update();
}

void OGLWidget::setDataObject(data_t newDataObject) {
  dataObject = newDataObject;
  createDrawbleData();
}

void OGLWidget::moveCamera(QString str) {
  glPushMatrix();
  glLoadIdentity();
  if (str == "S") {
    glTranslatef(0, 0, -30);
  } else if (str == "W") {
    _angleZ += 25;
    _angleZ += 25;
    update();
    //        glTranslatef(0,0,-3);
    //        drawObject();
  } else if (str == "A")
    glTranslatef(-30, 0, 0);
  else if (str == "D")
    glTranslatef(30, 0, 0);

  glPopMatrix();
  update();
}
