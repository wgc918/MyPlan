#include "qt_test.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // Create and show main window
  qt_test mainWindow;
  mainWindow.show();

  return app.exec();
}