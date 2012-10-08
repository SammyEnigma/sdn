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

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    settings = new QSettings("mdminhazulhaque", "somewhereinblog.net", this);
#ifdef Q_OS_WIN32
    win32startup = new QSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
#endif
#ifdef Q_OS_LINUX
    linuxstartup = new QFile(QDir::homePath()+"/.config/autostart/somewhereinblog-desktop-notifier.desktop");
#endif
    if( !settings->value("username").toString().isEmpty() or !settings->value("password").toString().isEmpty())
    {
        ui->username->setText(settings->value("username").toString());
        ui->password->setText(settings->value("password").toString());
        ui->time->setValue(settings->value("time").toInt());
    }
#ifdef Q_OS_WIN32
    if(win32startup->value("somewhereinblog-desktop-notifier").isValid())
            ui->autostart->setChecked(true);
    else
        ui->autostart->setChecked(false);
#endif
#ifdef Q_OS_LINUX
    if(linuxstartup->exists())
        ui->autostart->setChecked(true);
    else
        ui->autostart->setChecked(false);
#endif
    connect(ui->buttonClose,SIGNAL(clicked()),this,SLOT(close()));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
#ifdef Q_OS_WIN32
    delete win32startup;
#endif
#ifdef Q_OS_LINUX
    delete linuxstartup;
#endif
}

void SettingsDialog::on_buttonSave_clicked()
{
    if(ui->autostart->isChecked())
    {
#ifdef Q_OS_WIN32
        win32startup->setValue("somewhereinblog-desktop-notifier", QCoreApplication::applicationFilePath().replace('/','\\'));
#endif
#ifdef Q_OS_LINUX
        linuxstartup->setFileName("/usr/share/applications/somewhereinblog-desktop-notifier.desktop");
        linuxstartup->copy(QDir::homePath() + "/.config/autostart/somewhereinblog-desktop-notifier.desktop");
#endif
    }
    else
    {
#ifdef Q_OS_WIN32
        win32startup->remove("somewhereinblog-desktop-notifier");
#endif
#ifdef Q_OS_LINUX
        linuxstartup->remove();
#endif
    }
    emit sendUserData(ui->username->text(),
                      ui->password->text(),
                      ui->rememberme->isEnabled(),
                      ui->time->value());
    close();
}
