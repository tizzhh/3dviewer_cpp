#include "settings.h"

#include "mainwindow.h"
#include "oglwidget.h"

Settings::Settings(MainWindow *window, QObject *parent)
    : QObject{parent}, _mainWindow(window) {
  _settings = new QSettings("settings.ini", QSettings::IniFormat, this);
}

void Settings::loadSettings() {
  _settings->beginGroup("ColorsBackground");
  QColor colorBackground = {_settings->value("BackgroundRed", 255).toInt(),
                            _settings->value("BackgroundGreen", 255).toInt(),
                            _settings->value("BackgroundBlue", 255).toInt(),
                            _settings->value("BackgroundAlpha", 255).toInt()};
  _settings->endGroup();
  _settings->beginGroup("ColorsLines");
  QColor colorLines = {_settings->value("LinesRed", 255).toInt(),
                       _settings->value("LinesGreen", 20).toInt(),
                       _settings->value("LinesBlue", 147).toInt(),
                       _settings->value("LinesAlpha", 255).toInt()};
  _settings->endGroup();
  _settings->beginGroup("ColorsVertexes");
  QColor colorVertexes = {_settings->value("VertexesRed", 255).toInt(),
                          _settings->value("VertexesGreen", 20).toInt(),
                          _settings->value("VertexesBlue", 147).toInt(),
                          _settings->value("VertexesAlpha", 255).toInt()};
  _settings->endGroup();
  _mainWindow->setColors(colorLines, colorVertexes, colorBackground);
  int lineWidth = _settings->value("lineWidth", 1).toInt();
  int vertexesWidth = _settings->value("vertexesWidth", 1).toInt();
  _mainWindow->setSize(lineWidth, vertexesWidth);
  bool ortho = _settings->value("ortho", false).toBool();
  _mainWindow->setProjection(ortho);
  bool isVertexCircle = _settings->value("isVertexCircle", false).toBool();
  bool isVertexEnable = _settings->value("isVertexEnable", false).toBool();
  _mainWindow->setTypeVertexes(isVertexCircle, isVertexEnable);
  int typeLine = _settings->value("typeLine", 0).toInt();
  _mainWindow->setTypeLine(typeLine);
}

void Settings::saveSettings(OGLWidget *widget) {
  _settings->setValue("lineWidth", widget->getLineSize());
  _settings->setValue("vertexesWidth", widget->getVertexSize());
  _settings->setValue("isVertexCircle", widget->getIsVertexCircle());
  _settings->setValue("isVertexEnable", widget->getIsVertexEnable());
  _settings->setValue("typeLine", widget->getTypeLine());
  _settings->setValue("ortho", widget->getOrtho());
  _settings->beginGroup("ColorsBackground");
  _settings->setValue("BackgroundRed", widget->getBackground().red());
  _settings->setValue("BackgroundGreen", widget->getBackground().green());
  _settings->setValue("BackgroundBlue", widget->getBackground().blue());
  _settings->setValue("BackgroundAlpha", widget->getBackground().alpha());
  _settings->endGroup();
  _settings->beginGroup("ColorsLines");
  _settings->setValue("LinesRed", widget->getLinesColor().red());
  _settings->setValue("LinesGreen", widget->getLinesColor().green());
  _settings->setValue("LinesBlue", widget->getLinesColor().blue());
  _settings->setValue("LinesAlpha", widget->getLinesColor().alpha());
  _settings->endGroup();
  _settings->beginGroup("ColorsVertexes");
  _settings->setValue("VertexesRed", widget->getVertexesColor().red());
  _settings->setValue("VertexesGreen", widget->getVertexesColor().green());
  _settings->setValue("VertexesBlue", widget->getVertexesColor().blue());
  _settings->setValue("VertexesAlpha", widget->getVertexesColor().alpha());
  _settings->endGroup();
  _settings->sync();
}
