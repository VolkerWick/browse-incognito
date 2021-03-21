#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QClipboard>
#include <QDebug>
#include <QDesktopServices>
#include <QFile>
#include <QMenu>
#include <QMimeData>
#include <QStandardPaths>
#include <QSystemTrayIcon>
#include <QProcess>
#include <QTimer>
#include <QUrl>

#define SETTING_BROWSER "browser"
#define SETTING_BROWSERPARAMS "browserparams"
#define SETTING_ENABLED "enabled"

#define DEFAULT_BROWSER "brave"
#define DEFAULT_BROWSERPARAMS "--incognito"

#define MENU_PAUSE "Pause"
#define MENU_PAUSE_30s "Pause (30s)"
#define MENU_RESUME "Resume"
#define MENU_OPEN_CONFIGFILE "Open Config File"
#define MENU_EXIT "Exit"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , browser(settings.value(SETTING_BROWSER, DEFAULT_BROWSER).toString())
    , browserParams(settings.value(SETTING_BROWSERPARAMS, DEFAULT_BROWSERPARAMS).toString())
    , enabled(settings.value(SETTING_ENABLED, true).toBool())
    , contextMenu(new QMenu(this))
    , systemTrayIcon(new QSystemTrayIcon(this))
    , enabledIcon(QIcon("://enabled.ico"))
    , disabledIcon(QIcon("://disabled.ico"))
{
    settings.setValue(SETTING_BROWSER, browser);
    qDebug() << "settings file: " << settings.fileName() << settings.value(SETTING_BROWSER, "NO BROWSER SET");

    connect(qApp->clipboard(), &QClipboard::dataChanged, this, &MainWindow::onClipboardDataChanged);

    QAction* pauseAction = new QAction(MENU_PAUSE, this);
    connect(pauseAction, &QAction::triggered, this, &MainWindow::pause);
    contextMenu->addAction(pauseAction);

    QAction* pause30sAction = new QAction(MENU_PAUSE_30s, this);
    connect(pause30sAction, &QAction::triggered, this, &MainWindow::pause30);
    contextMenu->addAction(pause30sAction);

    QAction* resumeAction = new QAction(MENU_RESUME);
    connect(resumeAction , &QAction::triggered, this, &MainWindow::resume);
    contextMenu->addAction(resumeAction);

    QAction* openConfigFileAction = new QAction(MENU_OPEN_CONFIGFILE);
    connect(openConfigFileAction , &QAction::triggered, this, &MainWindow::openConfigFile);
    contextMenu->addAction(openConfigFileAction);

    contextMenu->addSeparator();

    QAction* exitAction = new QAction(MENU_EXIT);
    connect(exitAction, &QAction::triggered, this, &MainWindow::exit);
    contextMenu->addAction(exitAction);

    systemTrayIcon->setContextMenu(contextMenu);
    systemTrayIcon->setIcon(enabled ? enabledIcon : disabledIcon);
    systemTrayIcon->show();
}

void MainWindow::onClipboardDataChanged()
{
    if (!enabled) {
        return;
    }

    const QMimeData* mimeData = qApp->clipboard()->mimeData();
    if (mimeData == nullptr || !mimeData->hasText()) {
        return;
    }

    QUrl url(mimeData->text());
    if (url.scheme().isEmpty()) {
        return;
    }

    QProcess::startDetached(browser, QStringList{ browserParams, url.toString() });
}

void MainWindow::pause()
{
    enable(false);
}

void MainWindow::pause30()
{
    enable(false);
    QTimer::singleShot(30000, [this](){ enable(true); });
}

void MainWindow::resume()
{
    enable(true);
}

void MainWindow::openConfigFile()
{
    QUrl url = QUrl::fromLocalFile(settings.fileName());
    QDesktopServices::openUrl(url);
}

void MainWindow::exit()
{
    qApp->exit();
}

void MainWindow::enable(bool b)
{
    qDebug() << Q_FUNC_INFO << b;
    enabled = b;

    systemTrayIcon->setIcon(enabled ? enabledIcon : disabledIcon);
    settings.setValue(SETTING_ENABLED, enabled);
}


