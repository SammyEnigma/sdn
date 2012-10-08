/*****************************************************************************
somewhereinblog-desktop-app, get your notifications on your desktop.
Copyright (C) 2012 MD: Minhazul Haque;
mdminhazulhaque@gmail.com.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef WINDOW_H
#define WINDOW_H

#include <QDialog>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QUrl>
#include <QTimer>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>
#include <QDesktopServices>
#include <QFontDialog>

#include "settingsdialog.h"
#include "aboutdialog.h"
#include "defines.h"

namespace Ui {
class Window;
}

class Window : public QDialog
{
    Q_OBJECT
    
public:
    explicit Window(QWidget *parent = 0);
    ~Window();
    void toEnglish(QString &BN);

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void receiveUserData(
            QString username,
            QString password,
            bool rememberme,
            int timer);
    void doLogin();
    void manageCheckNow();
    void readSubscribedPosts(QNetworkReply *reply);
    void readOwnUnseenPosts(QNetworkReply *reply);
    void manageAppStatus(bool enabled);
    void manageSubscription();
    void manageOwnUnseenComments();
    void manageFont();
    void manageAction();
    void clearAll();
    void hideApplication();
    void quitApplication();

private:
    Ui::Window *ui;

    //Actions
    QAction *actionAppStatus;
    QAction *actionCheckNow;
    QAction *actionNotifications;
    QAction *actionManageSubscription;
    QAction *actionOwnUnseenComments;
    QAction *actionSettings;
    QAction *actionAbout;
    QAction *actionQuit;

    // Misc
    QSettings *settings;
    SettingsDialog *settingsDialog;
    AboutDialog *aboutDialog;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    bool readPosts;
    bool loginError;

    // User control
    QString username;
    QString password;
    QTimer *timer;

    // Tray Icons
    QIcon trayNormal;
    QIcon trayAlert;
    QIcon trayOnline;
    QIcon trayOffline;
    QIcon trayNew;

    // Icons Context Menu
    QIcon iconAbout;
    QIcon iconCheck;
    QIcon iconDisabled;
    QIcon iconEnabled;
    QIcon iconSubscriptions;
    QIcon iconUnseenComments;
    QIcon iconNotifications;
    QIcon iconSettings;
    QIcon iconQuit;

    // Network
    QNetworkAccessManager *managerSubscribed;
    QNetworkAccessManager *managerOwnUnseen;
    QNetworkRequest req;
    QByteArray postData;
    QString content;
    QStringRef postID;
    QString postName;
    QString postComments;
    QDomDocument xmlDoc;

    // Bangla
    QString bn0;
    QString bn1;
    QString bn2;
    QString bn3;
    QString bn4;
    QString bn5;
    QString bn6;
    QString bn7;
    QString bn8;
    QString bn9;

    // Macros
    QString bullet;
    QString nextPageIdentifier;
    QString notifyMsg;
    QString zeroNotification;
    QString errorLogin;
    QString noUnseenComment;
    QString noSubscribedPost;

    // Counters
    int pageCount;
    int nUpdatedPost;
    int nNewComments;
    int nComments;
    int statusCode;
    int i;
    int j;
    int k;
};

#endif // WINDOW_H
