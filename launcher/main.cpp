#include <QApplication>
#include "mainwindow.h"
#include "ConfigManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ConfigManager::instance().loadFromRegistry();
    MainWindow w;
    w.show();
    return app.exec();
}
