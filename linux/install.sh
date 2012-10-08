#!/bin/bash
# Author: Minhazul Haq Shawon <minhazul.haq@gmail.com> 2012

# Check root
if [ $(whoami) != "root" ]
  then
  echo "Superuser access required"
  exit 1
fi

# Check previous install
if [ -f /usr/bin/somewhereinblog-desktop-notifier ]
  then
  echo "Found previous version"
  echo "Removing..."
  rm /usr/bin/somewhereinblog-desktop-notifier
fi

# Installation
echo "Copying new files..."
chmod +x somewhereinblog-desktop-notifier
cp -f somewhereinblog-desktop-notifier.desktop /usr/share/applications
cp -f somewhereinblog-desktop-notifier.png /usr/share/icons
cp -f somewhereinblog-desktop-notifier /usr/bin

# Notify
if [ -f /usr/bin/zenity ]
  then
    $(zenity --info --text "Installation Completed")
elif [ -f /usr/bin/kdialog ]
  then
    $(kdialog --msgbox "Installation Completed")
fi