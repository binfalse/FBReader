/*
 * FBReader -- electronic book reader
 * Copyright (C) 2004-2006 Nikolay Pultsin <geometer@mawhrin.net>
 * Copyright (C) 2005 Mikhail Sobolev <mss@mawhrin.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <locale.h>
#include <libintl.h>
#include <gtk/gtk.h>

#include <abstract/ZLEncodingConverter.h>
#include <unix/ZLUnixFSManager.h>
#include <unix/ZLUnixTime.h>
#include <desktop/XMLOptions.h>
#include <maemo/GtkDialogManager.h>
#include <gtk/GtkImageManager.h>
#include <maemo/GtkDeviceInfo.h>

#include "GtkFBReader.h"
#include "Paths.h"

class ConfigSaverRunnable : public ZLRunnable {

public:
	void run() {
		XMLOptions::deleteInstance();
	}
};

int main(int argc, char **argv) {
	gtk_disable_setlocale();

	gtk_init(&argc, &argv);

	ZLUnixFSManager::createInstance();
	ZLUnixTimeManager::createInstance();
	ZLEncodingConverter::setEncodingDescriptionPath(EncodingDescriptionPath);
	XMLOptions::createInstance("FBReader");
	GtkDialogManager::createInstance();
	GtkImageManager::createInstance();
	((GtkDialogManager&)GtkDialogManager::instance()).setPixmapPath(GtkFBReader::ImageDirectory);
	GtkDeviceInfo::createInstance();

	GtkFBReader *reader = new GtkFBReader(argc == 1 ? std::string() : argv[1]);

	((GtkDialogManager&)GtkDialogManager::instance()).setMainWindow(GTK_WINDOW(reader->getMainWindow()));

	gtk_main();

	((GtkDialogManager&)GtkDialogManager::instance()).setMainWindow(0);

	GtkDeviceInfo::deleteInstance();
	GtkImageManager::deleteInstance();
	ConfigSaverRunnable configSaver;
	GtkDialogManager::instance().wait(configSaver, "Saving config...");
	GtkDialogManager::deleteInstance();
	ZLUnixFSManager::deleteInstance();
	ZLUnixTimeManager::deleteInstance();

	return 0;
}
