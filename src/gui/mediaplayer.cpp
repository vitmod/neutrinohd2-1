/*
	Neutrino-GUI  -   DBoxII-Project

	$id: mediaplayer.cpp 2015.12.22 12:07:30 mohousch $
	
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

#include <stdio.h>

#include <system/debug.h>

#include <gui/mediaplayer.h>
#include <gui/audioplayer.h>
#include <gui/movieplayer.h>
#include <gui/pictureviewer.h>
#include <gui/upnpbrowser.h>


CMediaPlayerMenu::CMediaPlayerMenu()
{
}

CMediaPlayerMenu::~CMediaPlayerMenu()
{
}

void CMediaPlayerMenu::showMenu()
{
	dprintf(DEBUG_NORMAL, "CMediaPlayerMenu::showMenu:\n");

	int shortcutMediaPlayer = 1;
	
	CMenuWidget MediaPlayer(LOCALE_MAINMENU_MEDIAPLAYER, NEUTRINO_ICON_MOVIE);
	
	//
#if defined (ENABLE_LIBEPLAYER3) || defined (ENABLE_GSTREAMER)	
	//Internet Radio
	MediaPlayer.addItem(new CMenuForwarderExtended(LOCALE_MAINMENU_INETRADIO, true, new CAudioPlayerGui(true), NULL, CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED, NEUTRINO_ICON_MENUITEM_INTERNETRADIO, LOCALE_HELPTEXT_INTERNETRADIO ));

	//AudioPlayer
	MediaPlayer.addItem(new CMenuForwarderExtended(LOCALE_MAINMENU_AUDIOPLAYER, true, new CAudioPlayerGui(), NULL, CRCInput::RC_green, NEUTRINO_ICON_BUTTON_GREEN, NEUTRINO_ICON_MENUITEM_AUDIOPLAYER, LOCALE_HELPTEXT_AUDIOPLAYER ));

	// ts player
	MediaPlayer.addItem(new CMenuForwarderExtended(LOCALE_MOVIEPLAYER_RECORDS, true, new CMoviePlayerGui(), "tsmoviebrowser", CRCInput::convertDigitToKey(shortcutMediaPlayer++), NULL, NEUTRINO_ICON_MENUITEM_MOVIEPLAYER, LOCALE_HELPTEXT_TSMOVIEBROWSER ));
	
	// movie player
	MediaPlayer.addItem(new CMenuForwarderExtended(LOCALE_MOVIEPLAYER_MOVIES, true, new CMoviePlayerGui(), "moviebrowser", CRCInput::convertDigitToKey(shortcutMediaPlayer++), NULL, NEUTRINO_ICON_MENUITEM_MOVIEPLAYER, LOCALE_HELPTEXT_TSMOVIEBROWSER ));
	
	// fileplayback
	MediaPlayer.addItem(new CMenuForwarderExtended(LOCALE_MOVIEPLAYER_FILEPLAYBACK, true, new CMoviePlayerGui(), "fileplayback", CRCInput::convertDigitToKey(shortcutMediaPlayer++), NULL, NEUTRINO_ICON_MENUITEM_MOVIEPLAYER, LOCALE_HELPTEXT_FILEPLAYBACK ));	
#endif	

	//PictureViewer
	MediaPlayer.addItem(new CMenuForwarderExtended(LOCALE_MAINMENU_PICTUREVIEWER, true, new CPictureViewerGui(), NULL, CRCInput::RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW, NEUTRINO_ICON_MENUITEM_PICTUREVIEWER, LOCALE_HELPTEXT_PICTUREVIEWER ));
	
	//UPNP Browser
 	MediaPlayer.addItem(new CMenuForwarderExtended(LOCALE_UPNPBROWSER_HEAD, true, new CUpnpBrowserGui(), NULL, CRCInput::RC_blue, NEUTRINO_ICON_BUTTON_BLUE, NEUTRINO_ICON_MENUITEM_UPNPBROWSER, LOCALE_HELPTEXT_UPNPBROWSER ));
	
	MediaPlayer.exec(NULL, "");
	MediaPlayer.hide();
}

int CMediaPlayerMenu::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CMediaplayerMenu::exec: actionKey:%s\n", actionKey.c_str());
	
	int res = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	showMenu();
	
	return res;
}


