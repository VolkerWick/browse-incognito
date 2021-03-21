#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QIcon>

class QMenu;
class QSystemTrayIcon;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onClipboardDataChanged();

private:
    QSettings settings;
    const QString browser;
    const QString browserParams;
    bool enabled;

    QMenu* contextMenu;
    QSystemTrayIcon* systemTrayIcon;
    QIcon enabledIcon;
    QIcon disabledIcon;

    void pause();
    void pause30();
    void resume();
    void openConfigFile();
    void exit();

    void enable(bool b);
};
#endif // MAINWINDOW_H
