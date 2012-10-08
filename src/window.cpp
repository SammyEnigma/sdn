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

#include "window.h"
#include "ui_window.h"

Window::Window(QWidget *parent) : QDialog(parent), ui(new Ui::Window)
{
    ui->setupUi(this);

    // Bangla Numbers
    bn0 = QString::fromUtf8("০");
    bn1 = QString::fromUtf8("১");
    bn2 = QString::fromUtf8("২");
    bn3 = QString::fromUtf8("৩");
    bn4 = QString::fromUtf8("৪");
    bn5 = QString::fromUtf8("৫");
    bn6 = QString::fromUtf8("৬");
    bn7 = QString::fromUtf8("৭");
    bn8 = QString::fromUtf8("৮");
    bn9 = QString::fromUtf8("৯");

    // Actions
    actionAppStatus = new QAction(tr("En&abled"),this);
    actionCheckNow = new QAction(tr("&Check Now"),this);
    actionNotifications = new QAction(tr("No New &Notifications"), this);
    actionSettings = new QAction(tr("&Settings"), this);
    actionManageSubscription = new QAction(tr("Manage S&ubscriptions"), this);
    actionOwnUnseenComments = new QAction(tr("Manage &Own Posts"),this);
    actionAbout = new QAction(tr("A&bout"), this);
    actionQuit = new QAction(tr("&Quit"), this);

    // Tray Icons
    trayNormal = QIcon((QPixmap(":/trayNormal")));
    trayAlert = QIcon((QPixmap(":/trayAlert")));
    trayOnline = QIcon((QPixmap(":/trayOnline")));
    trayOffline = QIcon((QPixmap(":/trayOffline")));
    trayNew = QIcon((QPixmap(":/trayNew")));

    // Icons Context Menu
    iconAbout = QIcon(QPixmap(":/actionAbout"));
    iconCheck = QIcon(QPixmap(":/actionCheck"));
    iconDisabled = QIcon(QPixmap(":/actionDisabled"));
    iconEnabled = QIcon(QPixmap(":/actionEnabled"));
    iconUnseenComments = QIcon(QPixmap(":/actionUnseenComments"));
    iconSubscriptions = QIcon(QPixmap(":/actionSubscriptions"));
    iconNotifications = QIcon(QPixmap(":/actionNotifications"));
    iconSettings = QIcon(QPixmap(":/settings"));
    iconQuit = QIcon(QPixmap(":/quit"));

    // Othres
    trayIconMenu = new QMenu(this);
    trayIcon = new QSystemTrayIcon(this);
    settings = new QSettings("mdminhazulhaque", "somewhereinblog.net", this);
    timer = new QTimer;
    settingsDialog = new SettingsDialog(this);
    aboutDialog = new AboutDialog(this);

    // Set Icons
    actionAppStatus->setIcon(iconEnabled);
    actionCheckNow->setIcon(iconCheck);
    actionNotifications->setIcon(iconNotifications);
    actionManageSubscription->setIcon(iconSubscriptions);
    actionOwnUnseenComments->setIcon(iconUnseenComments);
    actionSettings->setIcon(iconSettings);
    actionAbout->setIcon(iconAbout);
    actionQuit->setIcon(iconQuit);

    // Values
    pageCount = 1;
    nNewComments = 0;
    nUpdatedPost = 0;
    nComments = 0;

    // Macros
    bullet = QString::fromUtf8("▢ ");
    nextPageIdentifier = QString::fromUtf8("পরের পাতা");
    zeroNotification = QString::fromUtf8("(০)");
    errorLogin = QString::fromUtf8("একটি ভুল পাওয়া গেছে");
    noUnseenComment = QString::fromUtf8("কোন অদেখা মন্তব্য নেই");
    noSubscribedPost = QString::fromUtf8("আপনার কোন পর্যবেক্ষণকৃত পোস্ট নেই");

    // Network
    managerSubscribed = new QNetworkAccessManager;
    managerOwnUnseen = new QNetworkAccessManager;
    req.setUrl(QUrl("http://www.somewhereinblog.net/loginmanager/login"));
    req.setHeader(QNetworkRequest::ContentTypeHeader,
                  "application/x-www-form-urlencoded");

    // Create tray icon
    trayIcon->setIcon(trayNormal);
    trayIconMenu->addAction(actionNotifications);
    trayIconMenu->addAction(actionCheckNow);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(actionOwnUnseenComments);
    trayIconMenu->addAction(actionManageSubscription);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(actionAppStatus);
    trayIconMenu->addAction(actionSettings);
    trayIconMenu->addAction(actionAbout);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(actionQuit);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();

    // Actions
    actionAppStatus->setCheckable(true);
    actionAppStatus->setChecked(true);
    connect(actionAppStatus, SIGNAL(triggered(bool)), this, SLOT(manageAppStatus(bool)));
    connect(actionCheckNow, SIGNAL(triggered()), this, SLOT(manageCheckNow()));
    connect(actionNotifications, SIGNAL(triggered()), this, SLOT(manageAction()));
    connect(actionManageSubscription, SIGNAL(triggered()), this, SLOT(manageSubscription()));
    connect(actionOwnUnseenComments, SIGNAL(triggered()), this, SLOT(manageOwnUnseenComments()));
    connect(actionSettings, SIGNAL(triggered()), settingsDialog, SLOT(show()));
    connect(actionAbout, SIGNAL(triggered()), aboutDialog, SLOT(show()));
    connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(show()));

    //Buttons
    connect(ui->buttonReload, SIGNAL(clicked()), this, SLOT(manageCheckNow()));
    connect(ui->buttonClear, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(ui->buttonFont, SIGNAL(clicked()), this, SLOT(manageFont()));
    connect(ui->buttonHide, SIGNAL(clicked()), this, SLOT(hideApplication()));
    connect(ui->buttonQuit, SIGNAL(clicked()), qApp, SLOT(quit()));

    //Settings
    connect(settingsDialog, SIGNAL(sendUserData(QString,QString,bool,int)), this, SLOT(receiveUserData(QString,QString,bool,int)));
    // Tray icon
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    // Timer
    connect(timer, SIGNAL(timeout()), this, SLOT(manageCheckNow()));
    // Network
    connect(managerSubscribed, SIGNAL(finished(QNetworkReply*)), this, SLOT(readSubscribedPosts(QNetworkReply*)));
    connect(managerOwnUnseen, SIGNAL(finished(QNetworkReply*)), this, SLOT(readOwnUnseenPosts(QNetworkReply*)));

    // Throw a notification if settings are missing/first run
    if(settings->value("username").toString().isEmpty() or settings->value("password").toString().isEmpty())
    {
        trayIcon->showMessage("No username or password provided!", "Application has found no saved user information", QSystemTrayIcon::Critical, 3000);
        trayIcon->setIcon(trayAlert);
        settingsDialog->show();
    }
    else
    {
        // Userdata
        username = settings->value("username").toString();
        password = settings->value("password").toString();
        // Font
        ui->browserSubscribed->setFont(QFont(settings->value("fontfamily").toString(),settings->value("fontsize").toInt()));
        ui->browserOwnUnseen->setFont(ui->browserSubscribed->font());
        // Timer
        timer->setInterval(settings->value("time").toInt()*60000);
        trayIcon->setIcon(trayOffline);
        doLogin();
        timer->start();
    }
}

Window::~Window()
{
    delete ui;
    delete settings;
    delete managerSubscribed;
    delete managerOwnUnseen;
    delete timer;
    delete trayIcon;
    delete trayIconMenu;
    delete settingsDialog;
    delete aboutDialog;
    delete actionAppStatus;
    delete actionCheckNow;
    delete actionNotifications;
    delete actionManageSubscription;
    delete actionOwnUnseenComments;
    delete actionSettings;
    delete actionAbout;
    delete actionQuit;
}

void Window::toEnglish(QString &BN)
{
    QString *BanglaText = new QString(BN);
    BanglaText->remove(")");
    BanglaText->remove("(");
    BanglaText->replace(bn0,"0");
    BanglaText->replace(bn1,"1");
    BanglaText->replace(bn2,"2");
    BanglaText->replace(bn3,"3");
    BanglaText->replace(bn4,"4");
    BanglaText->replace(bn5,"5");
    BanglaText->replace(bn6,"6");
    BanglaText->replace(bn7,"7");
    BanglaText->replace(bn8,"8");
    BanglaText->replace(bn9,"9");
    nComments = BanglaText->toInt();
    delete BanglaText;
    return;
}

void Window::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        if(isActiveWindow())
            close();
        else
            showNormal();
        if(readPosts)
        {
            trayIcon->setIcon(trayOnline);
            actionNotifications->setText("No New &Notifications");
        }
        break;
    case QSystemTrayIcon::MiddleClick:
        settingsDialog->show();
        break;
    default:
        ;
    }
}

void Window::receiveUserData(QString username, QString password, bool rememberme, int timer)
{
    this->username = username;
    this->password = password;
    this->timer->setInterval(timer*60000);
    if(rememberme)
    {
        settings->setValue("username",username);
        settings->setValue("password",password);
        settings->setValue("time", timer);
    }
    else
        settings->clear();
    doLogin();
    this->timer->stop();
    this->timer->start();
    trayIcon->setIcon(trayNormal);
}

void Window::doLogin()
{
    clearAll();
    postData.clear();
    postData.append("username="+username);
    postData.append("&password="+password);
    managerSubscribed->post(req, postData);
}

void Window::manageCheckNow()
{
    if(loginError)
        doLogin();
    else
    clearAll();
    managerSubscribed->get(QNetworkRequest(QUrl(MONITORED_POSTS_URL+username)));
}

void Window::readSubscribedPosts(QNetworkReply *reply)
{
    // If no error
    if(reply->error() == QNetworkReply::NoError)
    {
        statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        // If readable
        if (statusCode>=200 and statusCode<300)
        {
            content.clear();
            content = QString::fromUtf8(reply->readAll());
            content.remove(0,334);

            // If login error
            if(content.indexOf(errorLogin) != -1)
            {
                loginError = true;
                trayIcon->setIcon(trayAlert);
                trayIcon->showMessage("Login error!", "There was an error logging in", QSystemTrayIcon::Information, 3000);
                return;
            }
            else
            {
                trayIcon->setIcon(trayOnline);
                loginError = false;
                readPosts = true;
            }

            if(!content.contains(noSubscribedPost))
            {
                // Parse the reply and extract data
                for(i=0,j=0,k=0;k<10;j=i+1,k++)
                {
                    xmlDoc.clear();
                    // Find link and post name
                    i = content.indexOf("<a target",j);
                    if(i==-1)
                        break;
                    postID.clear();
                    // Cut off text
                    postID = content.midRef(i, 1000);
                    xmlDoc.setContent(postID.toString());
                    postName.clear();
                    postName = (bullet + "<a href=\"" +xmlDoc.firstChild().toElement().attribute("href") + "\">" + xmlDoc.firstChild().toElement().text() + "</a>");
                    postName.remove("[sb]");
                    postName.remove("[/sb]");
                    postName.remove("[si]");
                    postName.remove("[/si]");
                    postName.remove("[su]");
                    postName.remove("[/su]");

                    // Find comments
                    j = postID.toString().indexOf("<td class");
                    xmlDoc.setContent(postID.toString().midRef(j, 100).toString());
                    postComments.clear();
                    postComments = xmlDoc.firstChild().toElement().text();

                    if(postComments != zeroNotification)
                    {
                        nUpdatedPost++;
                        toEnglish(postComments);
                        nNewComments += nComments;
                    }

                    // Add to text browser
                    ui->browserSubscribed->append("<tr><td width=\"90\%\">" + postName + "</td><td width=\"8\%\">" + postComments + "</td></tr>");
                }

                // If notificatiosn have multiple pages
                if(content.contains(nextPageIdentifier))
                {
                    managerSubscribed->get(QNetworkRequest(QUrl(MONITORED_POSTS_URL + username + "/" + QString::number(pageCount*10))));
                    pageCount++;
                    return;
                }
            }

            // Done loading
            pageCount = 1;
            managerOwnUnseen->get(QNetworkRequest(QUrl(UNSEEN_COMMENTS_URL+username)));
            return;
        }


        // If redirection
        else if (statusCode>=300 and statusCode<400)
        {
            managerOwnUnseen->setCookieJar(managerSubscribed->cookieJar());
            managerSubscribed->get(QNetworkRequest(QUrl(MONITORED_POSTS_URL)));
            return;
        }

    } // End of readable

    // If error strikes
    else
    {
        trayIcon->setIcon(trayAlert);
        return;
    }
}

void Window::readOwnUnseenPosts(QNetworkReply *reply)
{
    content.clear();
    content = QString::fromUtf8(reply->readAll());
    content.remove(0,334);

    if(!content.contains(noUnseenComment))
    {
        // Parse the reply and extract data
        for(i=0,j=0,k=0;k<10;j=i+1,k++)
        {
            xmlDoc.clear();
            // Find link and post name
            i = content.indexOf("<a target",j);
            if(i==-1)
                break;
            postID.clear();
            // Cut off text
            postID = content.midRef(i, 1000);
            xmlDoc.setContent(postID.toString());
            postName.clear();
            postName = (bullet + "<a href=\"" +xmlDoc.firstChild().toElement().attribute("href") + "\">" + xmlDoc.firstChild().toElement().text() + "</a>");
            postName.remove("[sb]");
            postName.remove("[/sb]");
            postName.remove("[si]");
            postName.remove("[/si]");
            postName.remove("[su]");
            postName.remove("[/su]");

            // Find comments
            j = postID.toString().indexOf("<td class");
            xmlDoc.setContent(postID.toString().midRef(j, 100).toString());
            postComments.clear();
            postComments = xmlDoc.firstChild().toElement().text();

            if(postComments != zeroNotification)
            {
                nUpdatedPost++;
                toEnglish(postComments);
                nNewComments += nComments;
            }

            // Add to text browser
            ui->browserOwnUnseen->append("<tr><td width=\"90\%\">" + postName + "</td><td width=\"8\%\">" + postComments + "</td></tr>");
        } // End of parsing

        // If notificatiosn have multiple pages
        if(content.contains(nextPageIdentifier))
        {
            managerOwnUnseen->get(QNetworkRequest(QUrl(UNSEEN_COMMENTS_URL + username + "/" + QString::number(pageCount*10))));
            pageCount++;
            return;
        }
    } // Done loading

    // Updated posts/Comments are non-zero
    if(nUpdatedPost or nNewComments)
    {
        notifyMsg.clear();
        notifyMsg = QString::number(nUpdatedPost) + " updated posts with " + QString::number(nNewComments) + " comments";
        actionNotifications->setText(notifyMsg);
        setWindowTitle(notifyMsg);
        trayIcon->showMessage("Update!", notifyMsg, QSystemTrayIcon::Information, 3000);
        trayIcon->setIcon(trayNew);
    }
    // Both updates post and comment count is zero
    else
    {
        actionNotifications->setText("No New &Notifications");
        setWindowTitle("No New Notifications");
    }
    // Reset all counters
    pageCount = 1;
    nUpdatedPost = 0;
    nNewComments = 0;
    return;
}

void Window::manageAppStatus(bool enabled)
{
    if(enabled)
    {
        actionAppStatus->setText("En&abled");
        actionAppStatus->setIcon(iconEnabled);
        trayIcon->setIcon(trayOnline);
        manageCheckNow();
        timer->start();
    }
    else
    {
        actionAppStatus->setText("Dis&abled");
        actionAppStatus->setIcon(iconDisabled);
        trayIcon->setIcon(trayOffline);
        readPosts = false;
        timer->stop();
    }
    return;
}

void Window::manageSubscription()
{
    QDesktopServices::openUrl(QUrl(MONITORED_POSTS_URL));
}

void Window::manageOwnUnseenComments()
{
    QDesktopServices::openUrl(QUrl(UNSEEN_COMMENTS_URL));
}

void Window::manageFont()
{
    ui->browserSubscribed->setFont(QFontDialog::getFont(0, ui->browserSubscribed->font()));
    ui->browserOwnUnseen->setFont(ui->browserSubscribed->font());
    settings->setValue("fontfamily", ui->browserSubscribed->font().family());
    settings->setValue("fontsize", ui->browserSubscribed->font().pointSize());
}

void Window::manageAction()
{
    if(readPosts)
    {
        trayIcon->setIcon(trayOnline);
        actionNotifications->setText("No New &Notifications");
    }
    showNormal();
}

void Window::clearAll()
{
    ui->browserSubscribed->clear();
    ui->browserOwnUnseen->clear();
}

void Window::hideApplication()
{
    if(readPosts)
    {
        trayIcon->setIcon(trayOnline);
        actionNotifications->setText("No New &Notifications");
    }
    hide();
}

void Window::quitApplication()
{
    qApp->quit();
}
