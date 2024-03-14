#include "mainwindow.h"

#include "./ui_mainwindow.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "../c_files/3d_viewer.h"
#ifdef __cplusplus
}
#endif

#include <QColorDialog>
#include <QDir>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QTimer>
#include <QWheelEvent>

#include "../giflib/dgif_lib.c"
#include "../giflib/egif_lib.c"
#include "../giflib/gif_err.c"
#include "../giflib/gif_hash.c"
#include "../giflib/gifalloc.c"
#include "../giflib/qgifimage.h"
#include "oglwidget.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->screenshots = 0;
  this->gifs = 0;
  _settings = new Settings(this);
  ui->oglwidget->setSettings(_settings);
  _settings->loadSettings();
  gif_list.clear();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setColors(QColor lineColor, QColor vertexColor,
                           QColor backgroundColor) {
  ui->oglwidget->setBackground(backgroundColor);
  ui->oglwidget->setVertexesColor(vertexColor);
  ui->oglwidget->setLinesColor(lineColor);
}

void MainWindow::setSize(int vertexesWidth, int lineWidth) {
  ui->oglwidget->setLineSize(lineWidth);
  ui->sliderLineSize->setValue(lineWidth);
  ui->oglwidget->setVertexSize(vertexesWidth);
  ui->sliderVertexSize->setValue(vertexesWidth);
}

void MainWindow::setTypeLine(int type) {
  ui->oglwidget->setTypeLine(type);
  if (type == 0)
    ui->radioButtonSolid->setChecked(true);
  else if (type == 1)
    ui->radioButtonDashed->setChecked(true);
  else if (type == 2)
    ui->radioButtonTriangles->setChecked(true);
}

void MainWindow::setTypeVertexes(bool isVertexCircle, bool isVertexEnable) {
  ui->oglwidget->setIsVertexEnable(isVertexEnable);
  ui->oglwidget->setIsVertexCircle(isVertexCircle);
  if (isVertexEnable) {
    if (isVertexCircle) {
      ui->radioButtonCircle->setChecked(true);
    } else
      ui->radioButtonRect->setChecked(true);
  } else
    ui->radioButtonNone->setChecked(true);
}

void MainWindow::setProjection(bool ortho) {
  ui->oglwidget->setOrtho(ortho);
  if (ortho)
    ui->comboBox->setCurrentIndex(0);
  else
    ui->comboBox->setCurrentIndex(1);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_W) {
    int y = ui->oglwidget->getCoordY();
    y -= 2;
    ui->sliderMoveY->setValue(y);
    ui->oglwidget->setCoordY(y);
  } else if (event->key() == Qt::Key_A) {
    int x = ui->oglwidget->getCoordX();
    x += 2;
    ui->sliderMoveX->setValue(x);
    ui->oglwidget->setCoordX(x);
  } else if (event->key() == Qt::Key_S) {
    int y = ui->oglwidget->getCoordY();
    y += 2;
    ui->sliderMoveY->setValue(y);
    ui->oglwidget->setCoordY(y);
  } else if (event->key() == Qt::Key_D) {
    int x = ui->oglwidget->getCoordX();
    x -= 2;
    ui->sliderMoveX->setValue(x);
    ui->oglwidget->setCoordX(x);
  } else if (event->key() == Qt::Key_Q) {
  } else if (event->key() == Qt::Key_E) {
  }
}

void MainWindow::wheelEvent(QWheelEvent *event) {
  if (event->angleDelta().y() < 0) {
    double scale = ui->oglwidget->getScale();
    scale -= 0.1;
    ui->verticalSlider->setValue(scale * 10);
    ui->oglwidget->setScale(scale);
  } else if (event->angleDelta().y() > 0) {
    double scale = ui->oglwidget->getScale();
    scale += 0.1;
    ui->verticalSlider->setValue(scale * 10);
    ui->oglwidget->setScale(scale);
  }
}

void MainWindow::on_screenButton_clicked() {
  QDir parentDir(QCoreApplication::applicationDirPath());
  parentDir.cdUp();

  QDir screenshotsDir(parentDir.absolutePath() + "/screenshots");
  if (!screenshotsDir.exists()) {
    screenshotsDir.mkpath(".");
  }
  auto geom = ui->oglwidget->grab();
  auto image = geom.toImage();
  image = image.scaled(WIDTH, HEIGHT, Qt::KeepAspectRatio,
                       Qt::SmoothTransformation);

  QStringList screens;

  int highest_num = 0;
  if (this->screenshots == 0) {
    screens = screenshotsDir.entryList(QStringList() << "*.bmp"
                                                     << "*.jpeg",
                                       QDir::Files);
    for (auto &screen : screens) {
      screen.replace("screenshot_", "");
      int name_num = screen.at(0).digitValue();
      if (name_num > highest_num) {
        highest_num = name_num;
      }
    }
    this->screenshots = highest_num + 1;
  }

  QString file_name;
  if (ui->radioButton_bmp->isChecked()) {
    file_name = screenshotsDir.filePath("screenshot_") +
                QString::number(this->screenshots++) + ".bmp";
  } else {
    file_name = screenshotsDir.filePath("screenshot_") +
                QString::number(this->screenshots++) + ".jpeg";
  }

  image.save(file_name);
  QMessageBox::information(this, "Screenshot Captured",
                           "Screenshot saved as:\n" + file_name);
}

void MainWindow::on_loadButton_clicked() {
  QString file =
      QFileDialog::getOpenFileName(this, "Open", "../test_objs", "*.obj");
  if (!file.isEmpty()) {
    struct data dataObject = {0};
    QByteArray ba = file.toLocal8Bit();
    char *path = ba.data();
    output res = S21_PrepareData(path, &dataObject);
    if (res == ERROR) {
      QMessageBox::critical(this, "Error", "Failed parse file");
    } else {
      ui->oglwidget->setDataObject(dataObject);
      ui->oglwidget->update();
      QStringList list = file.split("/");
      QString text = list.last();
      ui->labelName->setText("obj:" + text);
      text.clear();
      text = QString::number(dataObject.count_of_facets);
      ui->labelPoligons->setText("Vertexes:" + text);
      text.clear();
      text = QString::number(dataObject.count_of_vertexes);
      ui->labelVertexes->setText("Poligons:" + text);
    }
  }
}

void MainWindow::on_SliderZ_sliderMoved(int position) {
  ui->oglwidget->setAngleZ(position);
}

void MainWindow::on_SliderY_sliderMoved(int position) {
  ui->oglwidget->setAngleY(position);
}

void MainWindow::on_sliderX_sliderMoved(int position) {
  ui->oglwidget->setAngleX(position);
}

void MainWindow::on_radioButton_2_clicked() {
  ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_radioButton_clicked() {
  ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_sliderMoveX_sliderMoved(int position) {
  ui->oglwidget->setCoordX(position);
}

void MainWindow::on_sliderMoveY_sliderMoved(int position) {
  ui->oglwidget->setCoordY(position);
}

void MainWindow::on_sliderMoveZ_sliderMoved(int position) {
  ui->oglwidget->setCoordZ(position);
}

void MainWindow::on_verticalSlider_sliderMoved(int position) {
  ui->oglwidget->setScale(position / 10.0);
}

void MainWindow::on_gifButton_clicked() {
  connect(&timer, SIGNAL(timeout()), this, SLOT(CaptureFrames()));
  timer.start(DELAY_BETWEEN_FRAMES);
}

void MainWindow::CaptureFrames() {
  auto geom = ui->oglwidget->grab();
  auto image = geom.toImage();
  gif_list.append(image.scaled(WIDTH, HEIGHT, Qt::KeepAspectRatio,
                               Qt::SmoothTransformation));
  if (gif_list.size() >= GIF_DURATION * FPS) {
    timer.stop();
    disconnect(&timer, SIGNAL(timeout()), this, SLOT(CaptureFrames()));
    SaveGif();
  }
}

void MainWindow::SaveGif() {
  QDir parentDir(QCoreApplication::applicationDirPath());
  parentDir.cdUp();

  QDir gifDir(parentDir.absolutePath() + "/gifs");
  if (!gifDir.exists()) {
    gifDir.mkpath(".");
  }

  QStringList gifs;

  int highest_num = 0;
  if (this->gifs == 0) {
    gifs = gifDir.entryList(QStringList() << "*.gif", QDir::Files);
    for (auto &screen : gifs) {
      screen.replace("gif_", "");
      int name_num = screen.at(0).digitValue();
      if (name_num > highest_num) {
        highest_num = name_num;
      }
    }
    this->gifs = highest_num + 1;
  }

  QGifImage gif_recorder;
  gif_recorder.setDefaultDelay(DELAY_BETWEEN_FRAMES);
  for (const auto frame : gif_list) {
    gif_recorder.addFrame(frame);
  }

  QString file_name;
  file_name = gifDir.filePath("gif_") + QString::number(this->gifs++) + ".gif";

  gif_recorder.save(file_name);
  gif_list.clear();

  QMessageBox::information(this, "Gif Captured", "Gif saved as:\n" + file_name);
}

void MainWindow::on_radioButtonCircle_clicked() {
  ui->oglwidget->setIsVertexCircle(true);
  ui->oglwidget->setIsVertexEnable(true);
}

void MainWindow::on_radioButtonNone_clicked() {
  ui->oglwidget->setIsVertexEnable(false);
}

void MainWindow::on_radioButtonRect_clicked() {
  ui->oglwidget->setIsVertexCircle(false);
  ui->oglwidget->setIsVertexEnable(true);
}

void MainWindow::on_buttonColorVertex_clicked() {
  QColor initialColor = ui->oglwidget->getVertexesColor();
  QColor myColor = QColorDialog::getColor(initialColor, this, "Choose Color");
  if (myColor.isValid()) {
    ui->oglwidget->setVertexesColor(myColor);
  }
}

void MainWindow::on_sliderVertexSize_sliderMoved(int position) {
  ui->oglwidget->setVertexSize(position);
}

void MainWindow::on_sliderLineSize_sliderMoved(int position) {
  ui->oglwidget->setLineSize(position);
}

void MainWindow::on_bunnonColorLines_clicked() {
  QColor initialColor = ui->oglwidget->getLinesColor();
  QColor myColor = QColorDialog::getColor(initialColor, this, "Choose Color");
  if (myColor.isValid()) {
    ui->oglwidget->setLinesColor(myColor);
  }
}

void MainWindow::on_bunnotColorBackground_clicked() {
  QColor initialColor = ui->oglwidget->getBackground();
  QColor myColor = QColorDialog::getColor(initialColor, this, "Choose Color");
  if (myColor.isValid()) {
    ui->oglwidget->setBackground(myColor);
  }
}

void MainWindow::on_comboBox_currentIndexChanged(int index) {
  if (index == 0) ui->oglwidget->setOrtho(true);
  if (index == 1) ui->oglwidget->setOrtho(false);
}

void MainWindow::on_radioButtonSolid_clicked() {
  ui->oglwidget->setTypeLine(0);
}

void MainWindow::on_radioButtonDashed_clicked() {
  ui->oglwidget->setTypeLine(1);
}

void MainWindow::on_radioButtonTriangles_clicked() {
  ui->oglwidget->setTypeLine(2);
}
