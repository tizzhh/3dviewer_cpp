#ifndef OGLWINDOW_H
#define OGLWINDOW_H

#include <QObject>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include "mainwindow.h"
class Settings;

class OGLWidget : public QOpenGLWidget {
  Q_OBJECT
 public:
  OGLWidget(QWidget *parent = nullptr);
  virtual ~OGLWidget();

//  void setDataObject(data_t newDataObject);
  void moveCamera(QString str);
  void setAngleZ(int newAngleZ);
  void setAngleX(int newAngleX);
  void setAngleY(int newAngleY);
  void setCoordX(int newCoordX);
  void setCoordY(int newCoordY);
  void setCoordZ(int newCoordZ);
  void setScale(double newScale);
  double getScale() const;
  int getCoordX() const;
  int getCoordY() const;
  int getCoordZ() const;
  void setIsVertexEnable(bool newIsVertexEnable);
  void setIsVertexCircle(bool newIsVertexCircle);
  void setLineSize(int newLineSize);
  void setVertexSize(int newVertexSize);
  const QColor &getBackground() const;
  void setBackground(const QColor &newBackground);
  const QColor &getLinesColor() const;
  void setLinesColor(const QColor &newLinesColor);
  const QColor &getVertexesColor() const;
  void setVertexesColor(const QColor &newVertexesColor);
  void setOrtho(bool newOrtho);
  void setTypeLine(int newTypeLine);
  void setSettings(Settings *settings);

  int getLineSize() const;

  bool getOrtho() const;

  double getVertexSize() const;

  bool getIsVertexEnable() const;

  bool getIsVertexCircle() const;

  int getTypeLine() const;
  void createDrawbleData();

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

 private:

  bool ortho = true;
  double vertexSize = 1;
  double lineSize = 1;
  bool isVertexEnable = false;
  bool isVertexCircle = true;
  void drawObject();
  void drawVertexes();
//  data_t dataObject = {0};
  int _angleZ = 0;
  int _angleX = 0;
  int _angleY = 0;
  int coordX{0};
  int coordY{0};
  int coordZ{0};
  double scale{0.5};
  QColor background = {255, 255, 255};
  QColor linesColor = {255, 20, 147};
  QColor vertexesColor = {255, 20, 147};
  //тип линии: 0 - сплошная, 1 - пунктирная, 2 - треугольники
  int typeLine = 0;
  QVector<double> vecLines;
  QVector<double> vecVertexes;
  int countVertexes{0};
  Settings *_settings;
};

#endif  // OGLWINDOW_H
