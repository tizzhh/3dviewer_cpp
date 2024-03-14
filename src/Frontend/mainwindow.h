#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define WIDTH 640
#define HEIGHT 480
#define FPS 10
#define DELAY_BETWEEN_FRAMES 100
#define GIF_DURATION 5

#include <QMainWindow>
#include <QTimer>

class Settings;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void setColors(QColor lineColor, QColor VertexColor, QColor backgroundColor);
  void setSize(int vertexesWidth, int lineWidth);
  void setTypeLine(int type);
  void setTypeVertexes(bool isVertexCircle, bool isVertexEnable);
  void setProjection(bool ortho);

 public slots:
  void keyPressEvent(QKeyEvent *event);
  void wheelEvent(QWheelEvent *event);

 private slots:
  void on_screenButton_clicked();

  void on_loadButton_clicked();

  void on_SliderZ_sliderMoved(int position);

  void on_SliderY_sliderMoved(int position);

  void on_sliderX_sliderMoved(int position);

  void on_radioButton_2_clicked();

  void on_radioButton_clicked();

  void on_sliderMoveX_sliderMoved(int position);

  void on_sliderMoveY_sliderMoved(int position);

  void on_sliderMoveZ_sliderMoved(int position);

  void on_verticalSlider_sliderMoved(int position);

  void on_gifButton_clicked();

  void CaptureFrames();

  void SaveGif();

  void on_radioButtonCircle_clicked();

  void on_radioButtonNone_clicked();

  void on_radioButtonRect_clicked();

  void on_buttonColorVertex_clicked();

  void on_sliderVertexSize_sliderMoved(int position);

  void on_sliderLineSize_sliderMoved(int position);

  void on_bunnonColorLines_clicked();

  void on_bunnotColorBackground_clicked();

  void on_comboBox_currentIndexChanged(int index);

  void on_radioButtonSolid_clicked();

  void on_radioButtonDashed_clicked();

  void on_radioButtonTriangles_clicked();

 private:
  Ui::MainWindow *ui;
  int screenshots;
  int gifs;
  QVector<QImage> gif_list;
  QTimer timer;
  Settings *_settings;
};
#endif  // MAINWINDOW_H
