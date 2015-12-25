/*
  neutrinoHD2 project
  
  https://code.google.com/p/neutrinohd2/
  
  $Id: mediaportal.cpp 2015/13/22 mohousch Exp $

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

#include <plugin.h>	// plugin.h

extern "C" void plugin_exec(void);
extern "C" void plugin_init(void);
extern "C" void plugin_del(void);

//plugin API
void plugin_init(void)
{
}

void plugin_del(void)
{
}

void plugin_exec(void)
{
	int i = 0;
	int j = 0;
REPAINT:  
	CBox Box;
	
	Box.iX = g_settings.screen_StartX + 20;
	Box.iY = g_settings.screen_StartY + 20;
	Box.iWidth = g_settings.screen_EndX - g_settings.screen_StartX - 40;
	Box.iHeight = (g_settings.screen_EndY - g_settings.screen_StartY - 40);
	
	// paintBox (background)
	CFrameBuffer::getInstance()->paintBoxRel(Box.iX, Box.iY, Box.iWidth, Box.iHeight, /*COL_MENUCONTENT_PLUS_0*/COL_BACKGROUND);
	
	// paint horizontal line top
	CFrameBuffer::getInstance()->paintHLineRel(Box.iX + BORDER_LEFT, Box.iWidth - (BORDER_LEFT + BORDER_RIGHT), Box.iY + 35, COL_MENUCONTENT_PLUS_5);
	
	// paint horizontal line bottom
	CFrameBuffer::getInstance()->paintHLineRel(Box.iX + BORDER_LEFT, Box.iWidth - (BORDER_LEFT + BORDER_RIGHT), Box.iY + Box.iHeight - 35, COL_MENUCONTENT_PLUS_5);
	
	// paint title
	g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->RenderString(Box.iX + BORDER_LEFT + ICON_OFFSET, Box.iY + (35 - g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight(), Box.iWidth - (BORDER_LEFT + BORDER_RIGHT + ICON_OFFSET), "Media Portal", COL_MENUHEAD);
	
	// paint foot:FIXME: use arrays
	if(i == 0 && j == 0)
	{
		g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->RenderString(Box.iX + BORDER_LEFT + ICON_OFFSET, Box.iY + Box.iHeight - 35 + (35 - g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight(), Box.iWidth - (BORDER_LEFT + BORDER_RIGHT + ICON_OFFSET), g_Locale->getText(LOCALE_YOUTUBE), COL_MENUHEAD);
	}
	else if(i == 1 && j == 0)
	{
		g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->RenderString(Box.iX + BORDER_LEFT + ICON_OFFSET, Box.iY + Box.iHeight - 35 + (35 - g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight(), Box.iWidth - (BORDER_LEFT + BORDER_RIGHT + ICON_OFFSET), g_Locale->getText(LOCALE_NETZKINO), COL_MENUHEAD);
	}
	else if(i == 2 && j == 0)
	{
		g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->RenderString(Box.iX + BORDER_LEFT + ICON_OFFSET, Box.iY + Box.iHeight - 35 + (35 - g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight(), Box.iWidth - (BORDER_LEFT + BORDER_RIGHT + ICON_OFFSET), "Music deluxe", COL_MENUHEAD);
	}
	else if(i == 3 && j == 0)
	{
		g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->RenderString(Box.iX + BORDER_LEFT + ICON_OFFSET, Box.iY + Box.iHeight - 35 + (35 - g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight(), Box.iWidth - (BORDER_LEFT + BORDER_RIGHT + ICON_OFFSET), g_Locale->getText(LOCALE_MAINMENU_INETRADIO), COL_MENUHEAD);
	}
	
	// paint buttons
	int iw, ih;
	CFrameBuffer::getInstance()->getIconSize(NEUTRINO_ICON_BUTTON_TOP, &iw, &ih);
	CFrameBuffer::getInstance()->paintIcon(NEUTRINO_ICON_BUTTON_TOP, Box.iX + Box.iWidth - BORDER_RIGHT - iw, Box.iY + Box.iHeight - 35 + (35 - ih)/2);
	
	CFrameBuffer::getInstance()->getIconSize(NEUTRINO_ICON_BUTTON_DOWN, &iw, &ih);
	CFrameBuffer::getInstance()->paintIcon(NEUTRINO_ICON_BUTTON_DOWN, Box.iX + Box.iWidth - BORDER_RIGHT - 2*iw - 2, Box.iY + Box.iHeight - 35 + (35 - ih)/2);
	
	CFrameBuffer::getInstance()->getIconSize(NEUTRINO_ICON_BUTTON_RIGHT, &iw, &ih);
	CFrameBuffer::getInstance()->paintIcon(NEUTRINO_ICON_BUTTON_RIGHT, Box.iX + Box.iWidth - BORDER_RIGHT - 3*iw - 2*2, Box.iY + Box.iHeight - 35 + (35 - ih)/2);
	
	CFrameBuffer::getInstance()->getIconSize(NEUTRINO_ICON_BUTTON_LEFT, &iw, &ih);
	CFrameBuffer::getInstance()->paintIcon(NEUTRINO_ICON_BUTTON_LEFT, Box.iX + Box.iWidth - BORDER_RIGHT - 4*iw - 3*2, Box.iY + Box.iHeight - 35 + (35 - ih)/2);
	
	// calculte frameBoxes
	CBox frameBox;
	
	frameBox.iX = Box.iX + BORDER_LEFT;
	frameBox.iY = Box.iY + 35 + 5;
	frameBox.iWidth = (Box.iWidth - (BORDER_LEFT + BORDER_RIGHT))/6;
	frameBox.iHeight = (Box.iHeight - 80)/3;
	
	// framBox
	CFrameBuffer::getInstance()->paintBoxRel(frameBox.iX + frameBox.iWidth*i, frameBox.iY + frameBox.iHeight*j, frameBox.iWidth, frameBox.iHeight, COL_MENUCONTENT_PLUS_6, RADIUS_SMALL, CORNER_BOTH);
	
	//FIXME: use arrays
	// paint youtube logo in first boxframe
	std::string yt_logo = PLUGINDIR "/youtube/youtube_small.png";
	CFrameBuffer::getInstance()->DisplayImage(yt_logo, frameBox.iX + 5, frameBox.iY + 5, frameBox.iWidth - 10, frameBox.iHeight - 10);
	
	// paint netzkino logo in second boxframe
	std::string nk_logo = PLUGINDIR "/netzkino/netzkino_small.png";
	CFrameBuffer::getInstance()->DisplayImage(nk_logo, frameBox.iX + frameBox.iWidth + 5, frameBox.iY + 5, frameBox.iWidth - 10, frameBox.iHeight - 10);
	
	// paint music deluxe logo in third boxframe
	std::string musicdeluxe_logo = PLUGINDIR "/mediaportal/musicdeluxe.png";
	CFrameBuffer::getInstance()->DisplayImage(musicdeluxe_logo, frameBox.iX + 2*frameBox.iWidth + 5, frameBox.iY + 5, frameBox.iWidth - 10, frameBox.iHeight - 10);
	
	// paint internet radio logo in third boxframe
	std::string internetradio_logo = DATADIR "/neutrino/icons/audioplayersettings.png";
	CFrameBuffer::getInstance()->DisplayImage(internetradio_logo, frameBox.iX + 3*frameBox.iWidth + 5, frameBox.iY + 5, frameBox.iWidth - 10, frameBox.iHeight - 10);
	
	// blit all
	CFrameBuffer::getInstance()->blit();
	
	// loop
	neutrino_msg_t      msg;
	neutrino_msg_data_t data;
	
	unsigned long long timeoutEnd = CRCInput::calcTimeoutEnd(g_settings.timing[SNeutrinoSettings::TIMING_MENU] == 0 ? 0xFFFF : g_settings.timing[SNeutrinoSettings::TIMING_MENU]);

	bool loop = true;
	while (loop) 
	{
		g_RCInput->getMsgAbsoluteTimeout(&msg, &data, &timeoutEnd);

		if ((msg == CRCInput::RC_timeout ) || (msg == CRCInput::RC_home))
		{
			loop = false;
		}
		else if(msg == CRCInput::RC_ok)
		{
			if(i == 0 && j == 0)
			{
				CFrameBuffer::getInstance()->paintBackground();
				CFrameBuffer::getInstance()->blit();
					
				g_PluginList->startPlugin("youtube");
			}
			else if(i == 1 && j == 0)
			{
				CFrameBuffer::getInstance()->paintBackground();
				CFrameBuffer::getInstance()->blit();
					
				g_PluginList->startPlugin("netzkino");
			}
			else if(i == 2 && j == 0)
			{
				CFrameBuffer::getInstance()->paintBackground();
				CFrameBuffer::getInstance()->blit();
					
				CFile file;
		
				file.Title = "Music deluxe";
				file.Info1 = "stream";
				file.Info2 = "Musik Sender";
				file.Thumbnail = PLUGINDIR "/test/musicdeluxe.png";
				file.Name = "Music Deluxe";
				file.Url = "rtmp://flash.cdn.deluxemusic.tv/deluxemusic.tv-live/web_850.stream";
				
				CMoviePlayerGui tmpMoviePlayerGui;
					
				tmpMoviePlayerGui.addToPlaylist(file);
				tmpMoviePlayerGui.exec(NULL, "urlplayback");
				
			}
			else if(i == 3 && j == 0)
			{
				CFrameBuffer::getInstance()->paintBackground();
				CFrameBuffer::getInstance()->blit();
					
				CAudioPlayerGui internetRadio(true);
				internetRadio.exec(NULL, "");
			}
			
			goto REPAINT;
		}
		else if (msg == CRCInput::RC_right)
		{
			i++;
			if (i >= 6)
			{
				i = 0;
				j++;
				
				if(j >= 3)
					j = 0;
			}
			
			goto REPAINT;
		}
		else if (msg == CRCInput::RC_left)
		{
			i--;
			if(i < 0 && j > 0)
			{
				i = 5;
				j--;
				
				if(j < 0)
					j = 0;
			}
			
			// stay at first framBox
			if (i < 0)
				i = 0;
			
			goto REPAINT;
		}
		else if (msg == CRCInput::RC_down)
		{
			j++;
			if (j > 2)
				j = 2;
			
			goto REPAINT;
		}
		else if (msg == CRCInput::RC_up)
		{
			j--;
			if (j < 0)
				j = 0;
			
			goto REPAINT;
		}
		else
		{
			CNeutrinoApp::getInstance()->handleMsg( msg, data );
			// kein canceling...
		}
	}
	
	// hide and exit
	CFrameBuffer::getInstance()->paintBackground();
	CFrameBuffer::getInstance()->blit();
}


