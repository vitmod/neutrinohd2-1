/*
	Neutrino-GUI  -   DBoxII-Project

	$id: main_setup.cpp 2015.12.22 21:31:30 mohousch $
	
	Copyright (C) 2001 Steffen Hehn 'McClean'
	and some other guys
	Homepage: http://dbox.cyberphoria.org/

	License: GPL

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/ 

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <global.h>
#include <neutrino.h>

#include <stdio.h>

#include <gui/main_setup.h>

#include <gui/audio_setup.h>
#include <gui/video_setup.h>
#include <gui/parentallock_setup.h>
#include <gui/network_setup.h>
#include <gui/movieplayer_setup.h>
#include <gui/osd_setup.h>
#include <gui/audioplayer_setup.h>
#include <gui/pictureviewer_setup.h>
#include <gui/lcd_setup.h>
#include <gui/rc_setup.h>
#include <gui/recording_setup.h>
#include <gui/misc_setup.h>
#include <gui/hdd_menu.h>

#include <system/debug.h>
#include <system/setting_helpers.h>
#include <system/helpers.h>


// main settings
CMainSetup::CMainSetup()
{
}

CMainSetup::~CMainSetup()
{
}

int CMainSetup::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CMainSetup::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	showMenu();
	
	return ret;
}

void CMainSetup::showMenu()
{
	dprintf(DEBUG_NORMAL, "CMainSetup::showMenu:\n");
	
	CMenuWidget mainSettings(LOCALE_MAINSETTINGS_HEAD, NEUTRINO_ICON_SETTINGS);
	
	int shortcutMainSettings = 1;

	// video settings
	mainSettings.addItem(new CMenuForwarderExtended(LOCALE_MAINSETTINGS_VIDEO, true, new CVideoSettings(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL, NEUTRINO_ICON_MENUITEM_TV, LOCALE_HELPTEXT_VIDEOSETTINGS ));

	// audio settings
	mainSettings.addItem(new CMenuForwarderExtended(LOCALE_MAINSETTINGS_AUDIO, true, new CAudioSettings(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL, NEUTRINO_ICON_MENUITEM_AUDIOSETTINGS, LOCALE_HELPTEXT_AUDIOSETTINGS ));

	// parentallock
	if(g_settings.parentallock_prompt)
		mainSettings.addItem(new CLockedMenuForwarderExtended(LOCALE_PARENTALLOCK_PARENTALLOCK, g_settings.parentallock_pincode, true, true, new CParentalLockSettings(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL, "parentallock", LOCALE_HELPTEXT_PARENTALLOCK ));
	else
		mainSettings.addItem(new CMenuForwarderExtended(LOCALE_PARENTALLOCK_PARENTALLOCK, true, new CParentalLockSettings(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL, NEUTRINO_ICON_MENUITEM_PARENTALLOCK, LOCALE_HELPTEXT_PARENTALLOCK ));

	// network settings
	mainSettings.addItem(new CMenuForwarderExtended(LOCALE_MAINSETTINGS_NETWORK, true, CNetworkSettings::getInstance(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL, NEUTRINO_ICON_MENUITEM_UPNPBROWSER, LOCALE_HELPTEXT_NETWORKSETTINGS ));

	// recording settings
	mainSettings.addItem(new CMenuForwarderExtended(LOCALE_MAINSETTINGS_RECORDING, true, new CRecordingSettings(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL, NEUTRINO_ICON_MENUITEM_RECORDINGSETTINGS, LOCALE_HELPTEXT_RECORDINGSETTINGS ));

	// movieplayer settings
	mainSettings.addItem(new CMenuForwarderExtended(LOCALE_MAINSETTINGS_STREAMING, true, new CMoviePlayerSettings(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL, NEUTRINO_ICON_MENUITEM_MOVIEPLAYER, LOCALE_HELPTEXT_MOVIEPLAYERSETTINGS ));

	//OSD settings
	mainSettings.addItem(new CMenuForwarderExtended(LOCALE_MAINSETTINGS_OSD, true, new COSDSettings(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL, NEUTRINO_ICON_MENUITEM_OSDSETTINGS, LOCALE_HELPTEXT_OSDSETTINGS ));

	// vfd/lcd settings
	//if(CVFD::getInstance()->has_lcd)
		mainSettings.addItem(new CMenuForwarderExtended(LOCALE_MAINSETTINGS_LCD, true, new CLCDSettings(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL, NEUTRINO_ICON_MENUITEM_MAINSETTINGS, LOCALE_HELPTEXT_VFDSETTINGS ));	

	// remote control settings
	mainSettings.addItem(new CMenuForwarderExtended(LOCALE_MAINSETTINGS_KEYBINDING, true, new CRemoteControlSettings(), NULL, CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED, NEUTRINO_ICON_MENUITEM_KEYSSETTINGS, LOCALE_HELPTEXT_KEYSSETTINGS ));

	// audioplayer settings
	mainSettings.addItem(new CMenuForwarderExtended(LOCALE_AUDIOPLAYERSETTINGS_GENERAL, true, new CAudioPlayerSettings(), NULL, CRCInput::RC_green, NEUTRINO_ICON_BUTTON_GREEN, NEUTRINO_ICON_MENUITEM_AUDIOPLAYERSETTINGS, LOCALE_HELPTEXT_AUDIOPLAYERSETTINGS ));
	
	// pictureviewer settings
	mainSettings.addItem(new CMenuForwarderExtended(LOCALE_PICTUREVIEWERSETTINGS_GENERAL, true, new CPictureViewerSettings(), NULL, CRCInput::RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW, NEUTRINO_ICON_MENUITEM_PICTUREVIEWER, LOCALE_HELPTEXT_PICTUREVIEWERSETTINGS ));

	// misc settings
	mainSettings.addItem(new CMenuForwarderExtended(LOCALE_MAINSETTINGS_MISC, true, new CMiscSettings(), NULL, CRCInput::RC_blue, NEUTRINO_ICON_BUTTON_BLUE, NEUTRINO_ICON_MENUITEM_MAINSETTINGS, LOCALE_HELPTEXT_MISCSETTINGS ));

	//HDD settings
	mainSettings.addItem(new CMenuForwarderExtended(LOCALE_HDD_SETTINGS, true, new CHDDMenuHandler(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL, NEUTRINO_ICON_MENUITEM_HDDSETTINGS, LOCALE_HELPTEXT_HDDSETTINGS ));
	
	mainSettings.exec(NULL, "");
	mainSettings.hide();
}

