#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class MainWindow;
class OGLWidget;

class Settings : public QObject {
  Q_OBJECT
 public:
  explicit Settings(MainWindow *window, QObject *parent = nullptr);
  void loadSettings();
  void saveSettings(OGLWidget *widget);

 signals:
 private:
  QSettings *_settings;
  MainWindow *_mainWindow;
};

#endif  // SETTINGS_H
