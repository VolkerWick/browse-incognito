#include "mainwindow.h"

#include <QApplication>

// Open Urls copied to the clipboard with a configurable "incognito browser" such as brave in incognito mode
// e.g. select this URL http://example.com and hit Ctrl-C

#define NAME_ORGANIZATION   "volkerwick.github.io"
#define NAME_APPLICATION    "browse-incognito"

int main(int argc, char *argv[])
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QApplication a(argc, argv);

    a.setOrganizationName(NAME_ORGANIZATION);
    a.setApplicationName(NAME_APPLICATION);

    MainWindow w;
    w.hide();   // Mainwindow not shown, tray icon does everything
    return a.exec();
}
