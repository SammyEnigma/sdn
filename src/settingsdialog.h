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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QProcess>
#include <QFile>
#include <QDir>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    
private slots:
    void on_buttonSave_clicked();

signals:
    void sendUserData(QString, QString, bool, int);

private:
    Ui::SettingsDialog *ui;
    QSettings *settings;
#ifdef Q_OS_WIN32
    QSettings *win32startup;
#endif
#ifdef Q_OS_LINUX
    QFile *linuxstartup;
#endif
};

#endif // SETTINGSDIALOG_H
