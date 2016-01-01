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
#include <sys/stat.h>
#include <dirent.h>

#include <gui/widget/icons.h>
#include <gui/widget/hintbox.h>
#include <gui/widget/vfdcontroler.h>
#include <gui/widget/stringinput.h>
#include <gui/widget/stringinput_ext.h>
#include <gui/widget/colorchooser.h>
#include <gui/widget/keychooser.h>

#include <gui/main_setup.h>
#include <gui/hdd_menu.h>
#include <gui/audioplayer.h>
#include <gui/proxyserver_setup.h>
#include <gui/nfs.h>
#include <gui/themes.h>
#include <gui/screensetup.h>
#include <gui/alphasetup.h>
#include <gui/zapit_setup.h>
#include <gui/psisetup.h>

#include <system/debug.h>
#include <system/setting_helpers.h>
#include <system/helpers.h>

#include <video_cs.h>
#include <audio_cs.h>


// video settings
extern CVideoSetupNotifier	* videoSetupNotifier;

//Video Settings
//hdmi color space
#if !defined (PLATFORM_COOLSTREAM)
#ifdef __sh__
#define VIDEOMENU_HDMI_COLOR_SPACE_OPTION_COUNT 3
const CMenuOptionChooser::keyval VIDEOMENU_HDMI_COLOR_SPACE_OPTIONS[VIDEOMENU_HDMI_COLOR_SPACE_OPTION_COUNT] =
{
	 { HDMI_RGB, NONEXISTANT_LOCALE, "HDMI-RGB" },
	 { HDMI_YUV, NONEXISTANT_LOCALE, "HDMI-YUV" } ,
	 { HDMI_422, NONEXISTANT_LOCALE, "HDMI-422" }
};
#else
// giga
/*
Edid(Auto) 
Hdmi_Rgb 
Itu_R_BT_709 
Unknown
*/
#define VIDEOMENU_HDMI_COLOR_SPACE_OPTION_COUNT 4
const CMenuOptionChooser::keyval VIDEOMENU_HDMI_COLOR_SPACE_OPTIONS[VIDEOMENU_HDMI_COLOR_SPACE_OPTION_COUNT] =
{
	 { HDMI_AUTO, NONEXISTANT_LOCALE, "Edid(Auto)" },
	 { HDMI_RGB, NONEXISTANT_LOCALE, "Hdmi_Rgb" } ,
	 { HDMI_ITU_R_BT_709, NONEXISTANT_LOCALE, "Itu_R_BT_709" },
	 { HDMI_UNKNOW, NONEXISTANT_LOCALE, "Unknow" }
};
#endif
#endif /* !coolstream*/

//Analog Output 
#ifdef __sh__
//rgb/cvbs/svideo/yuv
#define VIDEOMENU_ANALOGUE_MODE_OPTION_COUNT 4
const CMenuOptionChooser::keyval VIDEOMENU_ANALOGUE_MODE_OPTIONS[VIDEOMENU_ANALOGUE_MODE_OPTION_COUNT] =
{
	 { ANALOG_RGB, NONEXISTANT_LOCALE, "RGB" },
	 { ANALOG_CVBS, NONEXISTANT_LOCALE, "CVBS" },
	 { ANALOG_SVIDEO, NONEXISTANT_LOCALE, "SVIDEO" }, //not used
	 { ANALOG_YUV, NONEXISTANT_LOCALE, "YUV" }
};
#elif defined (PLATFORM_COOLSTREAM)
#define VIDEOMENU_ANALOGUE_MODE_OPTION_COUNT 4
const CMenuOptionChooser::keyval VIDEOMENU_ANALOGUE_MODE_OPTIONS[VIDEOMENU_ANALOGUE_MODE_OPTION_COUNT] =
{
	{ ANALOG_SD_RGB_SCART, NONEXISTANT_LOCALE, "RGB"   }, /* composite + RGB */
	{ ANALOG_SD_YPRPB_SCART, NONEXISTANT_LOCALE, "YPbPr" }, /* YPbPr SCART */
	{ ANALOG_HD_RGB_SCART, NONEXISTANT_LOCALE, "RGB Scart"   },
	{ ANALOG_HD_YPRPB_SCART, NONEXISTANT_LOCALE, "YPbPr Scart" },
};
#else
//rgb/cvbs/yuv
#define VIDEOMENU_ANALOGUE_MODE_OPTION_COUNT 3
const CMenuOptionChooser::keyval VIDEOMENU_ANALOGUE_MODE_OPTIONS[VIDEOMENU_ANALOGUE_MODE_OPTION_COUNT] =
{
	 { ANALOG_RGB, NONEXISTANT_LOCALE, "RGB" },
	 { ANALOG_CVBS, NONEXISTANT_LOCALE, "CVBS" },
	 { ANALOG_YUV, NONEXISTANT_LOCALE, "YUV" }
};
#endif

// aspect ratio
#ifdef __sh__
#define VIDEOMENU_VIDEORATIO_OPTION_COUNT 2
const CMenuOptionChooser::keyval VIDEOMENU_VIDEORATIO_OPTIONS[VIDEOMENU_VIDEORATIO_OPTION_COUNT] =
{
	{ ASPECTRATIO_43, LOCALE_VIDEOMENU_VIDEORATIO_43, NULL },
	{ ASPECTRATIO_169, LOCALE_VIDEOMENU_VIDEORATIO_169, NULL }
};
#elif defined (PLATFORM_COOLSTREAM)
#define VIDEOMENU_VIDEORATIO_OPTION_COUNT 2
const CMenuOptionChooser::keyval VIDEOMENU_VIDEORATIO_OPTIONS[VIDEOMENU_VIDEORATIO_OPTION_COUNT] =
{
	{ DISPLAY_AR_4_3, LOCALE_VIDEOMENU_VIDEORATIO_43, NULL },
	{ DISPLAY_AR_16_9, LOCALE_VIDEOMENU_VIDEORATIO_169, NULL },
};
#else
#define VIDEOMENU_VIDEORATIO_OPTION_COUNT 3
const CMenuOptionChooser::keyval VIDEOMENU_VIDEORATIO_OPTIONS[VIDEOMENU_VIDEORATIO_OPTION_COUNT] =
{
	{ ASPECTRATIO_43, LOCALE_VIDEOMENU_VIDEORATIO_43, NULL },
	{ ASPECTRATIO_169, LOCALE_VIDEOMENU_VIDEORATIO_169, NULL },
	{ ASPECTRATIO_AUTO, NONEXISTANT_LOCALE, "Auto" }
};
#endif

// policy
#ifdef __sh__
/*
letterbox 
panscan 
non 
bestfit
*/
#define VIDEOMENU_VIDEOFORMAT_OPTION_COUNT 4
const CMenuOptionChooser::keyval VIDEOMENU_VIDEOFORMAT_OPTIONS[VIDEOMENU_VIDEOFORMAT_OPTION_COUNT] = 
{
	{ VIDEOFORMAT_LETTERBOX, LOCALE_VIDEOMENU_LETTERBOX, NULL },
	{ VIDEOFORMAT_PANSCAN, LOCALE_VIDEOMENU_PANSCAN, NULL },
	{ VIDEOFORMAT_FULLSCREEN, LOCALE_VIDEOMENU_FULLSCREEN, NULL },
	{ VIDEOFORMAT_PANSCAN2, LOCALE_VIDEOMENU_PANSCAN2, NULL }
};
#elif defined (PLATFORM_COOLSTREAM)
#define VIDEOMENU_VIDEOFORMAT_OPTION_COUNT 4
const CMenuOptionChooser::keyval VIDEOMENU_VIDEOFORMAT_OPTIONS[VIDEOMENU_VIDEOFORMAT_OPTION_COUNT] = 
{
	{ DISPLAY_AR_MODE_PANSCAN, LOCALE_VIDEOMENU_PANSCAN, NULL },
	{ DISPLAY_AR_MODE_PANSCAN2, LOCALE_VIDEOMENU_PANSCAN2, NULL },
	{ DISPLAY_AR_MODE_LETTERBOX, LOCALE_VIDEOMENU_LETTERBOX, NULL },
	{ DISPLAY_AR_MODE_NONE, LOCALE_VIDEOMENU_FULLSCREEN, NULL }
};
#else
// giga
/*
letterbox 
panscan 
bestfit 
nonlinear
*/
#define VIDEOMENU_VIDEOFORMAT_OPTION_COUNT 4
const CMenuOptionChooser::keyval VIDEOMENU_VIDEOFORMAT_OPTIONS[VIDEOMENU_VIDEOFORMAT_OPTION_COUNT] = 
{
	{ VIDEOFORMAT_LETTERBOX, LOCALE_VIDEOMENU_LETTERBOX, NULL },
	{ VIDEOFORMAT_PANSCAN, LOCALE_VIDEOMENU_PANSCAN, NULL },
	{ VIDEOFORMAT_PANSCAN2, LOCALE_VIDEOMENU_PANSCAN2, NULL },
	{ VIDEOFORMAT_FULLSCREEN, LOCALE_VIDEOMENU_FULLSCREEN, NULL }
};
#endif

//video mode
#ifdef __sh__
// cuberevo
/*
pal 
1080i50 
720p50 
576p50 
576i50 
1080i60 
720p60 
1080p24 
1080p25 
1080p30 
1080p50
PC
*/
#define VIDEOMENU_VIDEOMODE_OPTION_COUNT 12
const CMenuOptionChooser::keyval VIDEOMENU_VIDEOMODE_OPTIONS[VIDEOMENU_VIDEOMODE_OPTION_COUNT] =
{
	{ VIDEO_STD_PAL, NONEXISTANT_LOCALE, "PAL"		},
	{ VIDEO_STD_1080I50, NONEXISTANT_LOCALE, "1080i 50Hz"	},
	{ VIDEO_STD_720P50, NONEXISTANT_LOCALE, "720p 50Hz"	},
	{ VIDEO_STD_576P50, NONEXISTANT_LOCALE, "576p 50Hz"	},
	{ VIDEO_STD_576I50, NONEXISTANT_LOCALE, "576i 50Hz"	},
	{ VIDEO_STD_1080I60, NONEXISTANT_LOCALE, "1080i 60Hz"	},
	{ VIDEO_STD_720P60, NONEXISTANT_LOCALE, "720p 60Hz"	},
	{ VIDEO_STD_1080P24, NONEXISTANT_LOCALE, "1080p 24Hz"	},
	{ VIDEO_STD_1080P25, NONEXISTANT_LOCALE, "1080p 25Hz"	},
	{ VIDEO_STD_1080P30, NONEXISTANT_LOCALE, "1080p 30Hz"	},
	{ VIDEO_STD_1080P50, NONEXISTANT_LOCALE, "1080p 50Hz" 	},
	{ VIDEO_STD_PC, NONEXISTANT_LOCALE, "PC"		}
};
#elif defined (PLATFORM_COOLSTREAM)
#define VIDEOMENU_VIDEOMODE_OPTION_COUNT 12
const CMenuOptionChooser::keyval VIDEOMENU_VIDEOMODE_OPTIONS[VIDEOMENU_VIDEOMODE_OPTION_COUNT] =
{
	{ VIDEO_STD_SECAM,   NONEXISTANT_LOCALE, "SECAM"	},
	{ VIDEO_STD_PAL,     NONEXISTANT_LOCALE, "PAL"		},
	{ VIDEO_STD_576P,    NONEXISTANT_LOCALE, "576p"		},
	{ VIDEO_STD_720P50,  NONEXISTANT_LOCALE, "720p 50Hz"	},
	{ VIDEO_STD_1080I50, NONEXISTANT_LOCALE, "1080i 50Hz"	},
	{ VIDEO_STD_1080P24, NONEXISTANT_LOCALE, "1080p 24Hz"	},
	{ VIDEO_STD_1080P25, NONEXISTANT_LOCALE, "1080p 25Hz"	},
	{ VIDEO_STD_NTSC,    NONEXISTANT_LOCALE, "NTSC"		},
	{ VIDEO_STD_480P,    NONEXISTANT_LOCALE, "480p"		},
	{ VIDEO_STD_720P60,  NONEXISTANT_LOCALE, "720p 60Hz"	},
	{ VIDEO_STD_1080I60, NONEXISTANT_LOCALE, "1080i 60Hz"	},
	{ VIDEO_STD_AUTO,    NONEXISTANT_LOCALE, "Auto"         }
};
#else
// giga
/*
pal 
ntsc 
480i 
576i 
480p 
576p 
720p50 
720p 
1080i50 
1080i
*/
#define VIDEOMENU_VIDEOMODE_OPTION_COUNT 10
const CMenuOptionChooser::keyval VIDEOMENU_VIDEOMODE_OPTIONS[VIDEOMENU_VIDEOMODE_OPTION_COUNT] =
{
	{ VIDEO_STD_PAL, NONEXISTANT_LOCALE, "PAL"		},
	{ VIDEO_STD_NTSC, NONEXISTANT_LOCALE, "NTSC"		},
	{ VIDEO_STD_480I60, NONEXISTANT_LOCALE, "480i 60Hz"	},
	{ VIDEO_STD_576I50, NONEXISTANT_LOCALE, "576i 50Hz"	},
	{ VIDEO_STD_480P60, NONEXISTANT_LOCALE, "480p 60Hz"	},
	{ VIDEO_STD_576P50, NONEXISTANT_LOCALE, "576p 50Hz"	},
	{ VIDEO_STD_720P50, NONEXISTANT_LOCALE, "720p 50Hz"	},
	{ VIDEO_STD_720P60, NONEXISTANT_LOCALE, "720p 60Hz"	},
	{ VIDEO_STD_1080I50, NONEXISTANT_LOCALE, "1080i 50Hz"	},
	{ VIDEO_STD_1080I60, NONEXISTANT_LOCALE, "1080i 60Hz"	}
};
#endif

// wss
/*
off 
auto 
auto(4:3_off) 
4:3_full_format 
16:9_full_format 
14:9_letterbox_center 
14:9_letterbox_top 
16:9_letterbox_center 
16:9_letterbox_top 
>16:9_letterbox_center 
14:9_full_format
*/
#if !defined (PLATFORM_COOLSTREAM)
#ifdef __sh__
#define VIDEOMENU_WSS_OPTION_COUNT 3
const CMenuOptionChooser::keyval VIDEOMENU_WSS_OPTIONS[VIDEOMENU_WSS_OPTION_COUNT] =
{
	{ WSS_OFF, NONEXISTANT_LOCALE, "Off" },
	{ WSS_AUTO, NONEXISTANT_LOCALE, "Auto" },
	{ WSS_43_OFF, NONEXISTANT_LOCALE, "Auto(4:3_off)" },
};
#else
// giga
/*
off 
auto 
auto(4:3_off) 
4:3_full_format 
16:9_full_format 
14:9_letterbox_center 
14:9_letterbox_top 
16:9_letterbox_center 
16:9_letterbox_top 
>16:9_letterbox_center 
14:9_full_format
*/
#define VIDEOMENU_WSS_OPTION_COUNT 11
const CMenuOptionChooser::keyval VIDEOMENU_WSS_OPTIONS[VIDEOMENU_WSS_OPTION_COUNT] =
{
	{ WSS_OFF, NONEXISTANT_LOCALE, "Off" },
	{ WSS_AUTO, NONEXISTANT_LOCALE, "Auto" },
	{ WSS_43_OFF, NONEXISTANT_LOCALE, "Auto(4:3_off)" },
	{ WSS_43_FULL, NONEXISTANT_LOCALE, "4:3(full_format)" },
	{ WSS_169_FULL, NONEXISTANT_LOCALE, "16:9(full_format)" },
	{ WSS_149_LETTERBOX_CENTER, NONEXISTANT_LOCALE, "14:9(letterbox-center)" },
	{ WSS_149_LETTERBOX_TOP, NONEXISTANT_LOCALE, "14:9(letterbox-top)" },
	{ WSS_169_LETTERBOX_CENTER, NONEXISTANT_LOCALE, "16:9(letterbox-center)" },
	{ WSS_169_LETTERBOX_TOP, NONEXISTANT_LOCALE, "16:9(letterbox-top)" },
	{ WSS_169_LETTERBOX_CENTER_RIGHT, NONEXISTANT_LOCALE, ">16:9(letterbox-center)" },
	{ WSS_149_FULL, NONEXISTANT_LOCALE, "14:9(full)"}
	
};
#endif
#endif // !coolstream

CVideoSettings::CVideoSettings()
{
}

CVideoSettings::~CVideoSettings()
{
}

int CVideoSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CVideoSettings::exec: actionKey:%s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void CVideoSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CVideoSettings::showMenu\n");
	
	int shortcutVideo = 1;
	
	CMenuWidget videoSettings(LOCALE_VIDEOMENU_HEAD, NEUTRINO_ICON_VIDEO);
	
	// intros
	videoSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	videoSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	// save settings
	videoSettings.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));
	videoSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );

	// video aspect ratio 4:3/16:9
	videoSettings.addItem(new CMenuOptionChooser(LOCALE_VIDEOMENU_VIDEORATIO, &g_settings.video_Ratio, VIDEOMENU_VIDEORATIO_OPTIONS, VIDEOMENU_VIDEORATIO_OPTION_COUNT, true, videoSetupNotifier, CRCInput::convertDigitToKey(shortcutVideo++), "", true ));
	
	// video format bestfit/letterbox/panscan/non
	videoSettings.addItem(new CMenuOptionChooser(LOCALE_VIDEOMENU_VIDEOFORMAT, &g_settings.video_Format, VIDEOMENU_VIDEOFORMAT_OPTIONS, VIDEOMENU_VIDEOFORMAT_OPTION_COUNT, true, videoSetupNotifier, CRCInput::convertDigitToKey(shortcutVideo++), "", true ));
	
	// video analogue mode
	videoSettings.addItem(new CMenuOptionChooser(LOCALE_VIDEOMENU_ANALOG_MODE, &g_settings.analog_mode, VIDEOMENU_ANALOGUE_MODE_OPTIONS, VIDEOMENU_ANALOGUE_MODE_OPTION_COUNT, true, videoSetupNotifier, CRCInput::convertDigitToKey(shortcutVideo++), "", true ));

#if !defined (PLATFORM_COOLSTREAM)
	// video hdmi space colour	
	videoSettings.addItem(new CMenuOptionChooser(LOCALE_VIDEOMENU_HDMI_COLOR_SPACE, &g_settings.hdmi_color_space, VIDEOMENU_HDMI_COLOR_SPACE_OPTIONS, VIDEOMENU_HDMI_COLOR_SPACE_OPTION_COUNT, true, videoSetupNotifier, CRCInput::convertDigitToKey(shortcutVideo++), "", true ));	
	
	// wss
	videoSettings.addItem(new CMenuOptionChooser(LOCALE_VIDEOMENU_WSS, &g_settings.wss_mode, VIDEOMENU_WSS_OPTIONS, VIDEOMENU_WSS_OPTION_COUNT, true, videoSetupNotifier, CRCInput::convertDigitToKey(shortcutVideo++), "", true ));
#endif	

	// video mode
	videoSettings.addItem(new CMenuOptionChooser(LOCALE_VIDEOMENU_VIDEOMODE, &g_settings.video_Mode, VIDEOMENU_VIDEOMODE_OPTIONS, VIDEOMENU_VIDEOMODE_OPTION_COUNT, true, videoSetupNotifier, CRCInput::convertDigitToKey(shortcutVideo++), "", true));
	
	videoSettings.exec(NULL, "");
	videoSettings.hide();
}

// audio settings
extern CAudioSetupNotifier	* audioSetupNotifier;

#define OPTIONS_OFF0_ON1_OPTION_COUNT 2
const CMenuOptionChooser::keyval OPTIONS_OFF0_ON1_OPTIONS[OPTIONS_OFF0_ON1_OPTION_COUNT] =
{
        { 0, LOCALE_OPTIONS_OFF, NULL },
        { 1, LOCALE_OPTIONS_ON, NULL }
};

#define AUDIOMENU_ANALOGOUT_OPTION_COUNT 3
const CMenuOptionChooser::keyval AUDIOMENU_ANALOGOUT_OPTIONS[AUDIOMENU_ANALOGOUT_OPTION_COUNT] =
{
	{ 0, LOCALE_AUDIOMENU_STEREO, NULL },
	{ 1, LOCALE_AUDIOMENU_MONOLEFT, NULL },
	{ 2, LOCALE_AUDIOMENU_MONORIGHT, NULL }
};

#if defined (PLATFORM_COOLSTREAM)
#define AUDIOMENU_AVSYNC_OPTION_COUNT 3
const CMenuOptionChooser::keyval AUDIOMENU_AVSYNC_OPTIONS[AUDIOMENU_AVSYNC_OPTION_COUNT] =
{
        { 0, LOCALE_OPTIONS_OFF, NULL },
        { 1, LOCALE_OPTIONS_ON, NULL },
        { 2, LOCALE_AUDIOMENU_AVSYNC_AM, NULL }
};
#else
#define AUDIOMENU_AVSYNC_OPTION_COUNT 3
const CMenuOptionChooser::keyval AUDIOMENU_AVSYNC_OPTIONS[AUDIOMENU_AVSYNC_OPTION_COUNT] =
{
        { AVSYNC_OFF, LOCALE_OPTIONS_OFF, NULL },
        { AVSYNC_ON, LOCALE_OPTIONS_ON, NULL },
        { AVSYNC_AM, LOCALE_AUDIOMENU_AVSYNC_AM, NULL }
};
#endif

// ac3
#if !defined (PLATFORM_COOLSTREAM)
#define AC3_OPTION_COUNT 2
const CMenuOptionChooser::keyval AC3_OPTIONS[AC3_OPTION_COUNT] =
{
	{ AC3_DOWNMIX, NONEXISTANT_LOCALE, "downmix" },
	{ AC3_PASSTHROUGH, NONEXISTANT_LOCALE, "passthrough" }
};

#define AUDIODELAY_OPTION_COUNT 9
const CMenuOptionChooser::keyval AUDIODELAY_OPTIONS[AUDIODELAY_OPTION_COUNT] =
{
	{ -1000, NONEXISTANT_LOCALE, "-1000" },
	{ -750, NONEXISTANT_LOCALE, "-750" },
	{ -500, NONEXISTANT_LOCALE, "-500" },
	{ -250, NONEXISTANT_LOCALE, "-250" },
	{ 0, NONEXISTANT_LOCALE, "0" },
	{ 250, NONEXISTANT_LOCALE, "250" },
	{ 500, NONEXISTANT_LOCALE, "500" },
	{ 750, NONEXISTANT_LOCALE, "750" },
	{ 1000, NONEXISTANT_LOCALE, "1000" }
};
#endif

CAudioSettings::CAudioSettings()
{
}

CAudioSettings::~CAudioSettings()
{
}

int CAudioSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CAudioSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void CAudioSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CMainSetup::showMenu:\n");
	
	CMenuWidget audioSettings(LOCALE_AUDIOMENU_HEAD, NEUTRINO_ICON_AUDIO);
	
	int shortcutAudio = 1;
	
	// intros
	audioSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	audioSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	// save settings
	audioSettings.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));
	audioSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );

	// analog output
	audioSettings.addItem(new CMenuOptionChooser(LOCALE_AUDIOMENU_ANALOGOUT, &g_settings.audio_AnalogMode, AUDIOMENU_ANALOGOUT_OPTIONS, AUDIOMENU_ANALOGOUT_OPTION_COUNT, true, audioSetupNotifier, CRCInput::convertDigitToKey(shortcutAudio++), "", true ));

#if !defined (PLATFORM_COOLSTREAM)	
	// hdmi-dd
	audioSettings.addItem(new CMenuOptionChooser(LOCALE_AUDIOMENU_HDMI_DD, &g_settings.hdmi_dd, AC3_OPTIONS, AC3_OPTION_COUNT, true, audioSetupNotifier, CRCInput::convertDigitToKey(shortcutAudio++) ));	
#endif	

	// A/V sync
	audioSettings.addItem(new CMenuOptionChooser(LOCALE_AUDIOMENU_AVSYNC, &g_settings.avsync, AUDIOMENU_AVSYNC_OPTIONS, AUDIOMENU_AVSYNC_OPTION_COUNT, true, audioSetupNotifier, CRCInput::convertDigitToKey(shortcutAudio++), "", true ));
	
#if !defined (PLATFORM_COOLSTREAM)	
	// ac3 delay
	audioSettings.addItem(new CMenuOptionChooser(LOCALE_AUDIOMENU_AC3_DELAY, &g_settings.ac3_delay, AUDIODELAY_OPTIONS, AUDIODELAY_OPTION_COUNT, true, audioSetupNotifier, CRCInput::convertDigitToKey(shortcutAudio++) ));
	
	// pcm delay
	audioSettings.addItem(new CMenuOptionChooser(LOCALE_AUDIOMENU_PCM_DELAY, &g_settings.pcm_delay, AUDIODELAY_OPTIONS, AUDIODELAY_OPTION_COUNT, true, audioSetupNotifier, CRCInput::convertDigitToKey(shortcutAudio++) ));
#endif	
	
	// pref sub/lang
	audioSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_AUDIOMENU_PREF_LANG_HEAD));
	
	// auto ac3 
	CMenuOptionChooser * a1 = new CMenuOptionChooser(LOCALE_AUDIOMENU_DOLBYDIGITAL, &g_settings.audio_DolbyDigital, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, g_settings.auto_lang, audioSetupNotifier );
	
	// audiolang
	CMenuOptionStringChooser * audiolangSelect[3];
	
	for(int i = 0; i < 3; i++) 
	{
		audiolangSelect[i] = new CMenuOptionStringChooser(LOCALE_AUDIOMENU_PREF_LANG, g_settings.pref_lang[i], g_settings.auto_lang, NULL, CRCInput::RC_nokey, "", true);
		
		audiolangSelect[i]->addOption("");
		std::map<std::string, std::string>::const_iterator it;
		for(it = iso639rev.begin(); it != iso639rev.end(); it++) 
			audiolangSelect[i]->addOption(it->first.c_str());
	}
	
	CAutoAudioNotifier * autoAudioNotifier = new CAutoAudioNotifier(a1, audiolangSelect[0], audiolangSelect[1], audiolangSelect[2]);
	
	// auto lang
	audioSettings.addItem(new CMenuOptionChooser(LOCALE_AUDIOMENU_AUTO_LANG, &g_settings.auto_lang, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true, autoAudioNotifier));
	
	// ac3
	audioSettings.addItem(a1);
	
	// lang
	for(int i = 0; i < 3; i++) 
		audioSettings.addItem(audiolangSelect[i]);
	
	// sublang
	audioSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_AUDIOMENU_PREF_SUBS_HEAD));
	
	CMenuOptionStringChooser * sublangSelect[3];
	for(int i = 0; i < 3; i++) 
	{
		sublangSelect[i] = new CMenuOptionStringChooser(LOCALE_AUDIOMENU_PREF_SUBS, g_settings.pref_subs[i], g_settings.auto_subs, NULL, CRCInput::RC_nokey, "", true);
		std::map<std::string, std::string>::const_iterator it;
		
		sublangSelect[i]->addOption("");
		for(it = iso639rev.begin(); it != iso639rev.end(); it++) 
			sublangSelect[i]->addOption(it->first.c_str());
	}
	
	CSubLangSelectNotifier * subLangSelectNotifier = new CSubLangSelectNotifier(sublangSelect[0], sublangSelect[1], sublangSelect[2]);
	
	// auto sublang
	audioSettings.addItem(new CMenuOptionChooser(LOCALE_AUDIOMENU_AUTO_SUBS, &g_settings.auto_subs, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true, subLangSelectNotifier));
	
	// sublang
	for(int i = 0; i < 3; i++) 
		audioSettings.addItem(sublangSelect[i]);
	
	audioSettings.exec(NULL, "");
	audioSettings.hide();
}

// parentallock settings
extern bool parentallocked;			// defined neutrino.cpp

#define PARENTALLOCK_PROMPT_OPTION_COUNT 3
const CMenuOptionChooser::keyval PARENTALLOCK_PROMPT_OPTIONS[PARENTALLOCK_PROMPT_OPTION_COUNT] =
{
	{ PARENTALLOCK_PROMPT_NEVER         , LOCALE_PARENTALLOCK_NEVER, NULL         },
	{ PARENTALLOCK_PROMPT_CHANGETOLOCKED, LOCALE_PARENTALLOCK_CHANGETOLOCKED, NULL },
	{ PARENTALLOCK_PROMPT_ONSIGNAL      , LOCALE_PARENTALLOCK_ONSIGNAL, NULL       }
};

#define PARENTALLOCK_LOCKAGE_OPTION_COUNT 3
const CMenuOptionChooser::keyval PARENTALLOCK_LOCKAGE_OPTIONS[PARENTALLOCK_LOCKAGE_OPTION_COUNT] =
{
	{ 12, LOCALE_PARENTALLOCK_LOCKAGE12, NULL },
	{ 16, LOCALE_PARENTALLOCK_LOCKAGE16, NULL },
	{ 18, LOCALE_PARENTALLOCK_LOCKAGE18, NULL }
};

CParentalLockSettings::CParentalLockSettings()
{
}

CParentalLockSettings::~CParentalLockSettings()
{
}

int CParentalLockSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CParentalLockSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void CParentalLockSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CParentalLockSettings::showMenu:\n");
	
	int shortcutLock = 1;
	
	CMenuWidget parentallockSettings(LOCALE_PARENTALLOCK_PARENTALLOCK, NEUTRINO_ICON_LOCK);
	
	// intro
	parentallockSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	parentallockSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	// save settings
	parentallockSettings.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));
	parentallockSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );

	// prompt
	parentallockSettings.addItem(new CMenuOptionChooser(LOCALE_PARENTALLOCK_PROMPT, &g_settings.parentallock_prompt, PARENTALLOCK_PROMPT_OPTIONS, PARENTALLOCK_PROMPT_OPTION_COUNT, !parentallocked, NULL, CRCInput::convertDigitToKey(shortcutLock++), "", true ));

	// lockage
	parentallockSettings.addItem(new CMenuOptionChooser(LOCALE_PARENTALLOCK_LOCKAGE, &g_settings.parentallock_lockage, PARENTALLOCK_LOCKAGE_OPTIONS, PARENTALLOCK_LOCKAGE_OPTION_COUNT, !parentallocked, NULL, CRCInput::convertDigitToKey(shortcutLock++), "", true ));

	// Pin
	CPINChangeWidget * pinChangeWidget = new CPINChangeWidget(LOCALE_PARENTALLOCK_CHANGEPIN, g_settings.parentallock_pincode, 4, LOCALE_PARENTALLOCK_CHANGEPIN_HINT1);
	parentallockSettings.addItem( new CMenuForwarder(LOCALE_PARENTALLOCK_CHANGEPIN, true, g_settings.parentallock_pincode, pinChangeWidget, NULL, CRCInput::convertDigitToKey(shortcutLock++) ));
	
	parentallockSettings.exec(NULL, "");
	parentallockSettings.hide();
}

// network settings
#define OPTIONS_NTPENABLE_OPTION_COUNT 2
const CMenuOptionChooser::keyval OPTIONS_NTPENABLE_OPTIONS[OPTIONS_NTPENABLE_OPTION_COUNT] =
{
	{ 0, NONEXISTANT_LOCALE, "DVB" },
	{ 1, NONEXISTANT_LOCALE, "NTP" }
};

#define OPTIONS_WLAN_SECURITY_OPTION_COUNT 2
const CMenuOptionChooser::keyval OPTIONS_WLAN_SECURITY_OPTIONS[OPTIONS_WLAN_SECURITY_OPTION_COUNT] =
{
        { 0, NONEXISTANT_LOCALE, "WPA" },
        { 1, NONEXISTANT_LOCALE, "WPA2"  }
};

static int my_filter(const struct dirent * dent)
{
	if(dent->d_name[0] == 'l' && dent->d_name[1] == 'o')
		return 0;
	if(dent->d_name[0] == '.')
		return 0;
	
	return 1;
}

CNetworkSettings::CNetworkSettings()
{
	networkConfig = CNetworkConfig::getInstance();

	// init IP changer
	MyIPChanger = new CIPChangeNotifier;
}

CNetworkSettings *CNetworkSettings::getInstance()
{
	static CNetworkSettings *networkSettings = NULL;

	if(!networkSettings)
	{
		networkSettings = new CNetworkSettings();
		dprintf(DEBUG_NORMAL, "CNetworkSettings::getInstance: Instance created\n");
	}
	
	return networkSettings;
}

CNetworkSettings::~CNetworkSettings()
{
	delete networkConfig;
}

int CNetworkSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CNetworkSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		networkConfig->automatic_start = (network_automatic_start == 1);
		networkConfig->commitConfig();

		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	else if(actionKey == "network") 
	{
		CHintBox * hintBox = new CHintBox(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_NETWORKMENU_SETUPNOW)); // UTF-8
		hintBox->paint();
		
		networkConfig->automatic_start = (network_automatic_start == 1);
		networkConfig->stopNetwork();
		networkConfig->commitConfig();
		networkConfig->startNetwork();
		
		hintBox->hide();
		delete hintBox;
		hintBox = NULL;
		
		return ret;
	}
	else if(actionKey == "networktest") 
	{
		dprintf(DEBUG_INFO, "CNeutrinoApp::exec: doing network test...\n");

		testNetworkSettings(networkConfig->address.c_str(), networkConfig->netmask.c_str(), networkConfig->broadcast.c_str(), networkConfig->gateway.c_str(), networkConfig->nameserver.c_str(), networkConfig->inet_static);
		
		return ret;
	}
	else if(actionKey == "networkshow") 
	{
		dprintf(DEBUG_INFO, "CNeutrinoApp::exec: showing current network settings...\n");
		showCurrentNetworkSettings();
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void CNetworkSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CNetworkSettings::showMenu:\n");
	
	CMenuWidget networkSettings(LOCALE_NETWORKMENU_HEAD, NEUTRINO_ICON_NETWORK);
	
	struct dirent **namelist;

	//if select
	int ifcount = scandir("/sys/class/net", &namelist, my_filter, alphasort);

	CMenuOptionStringChooser * ifSelect = new CMenuOptionStringChooser(LOCALE_NETWORKMENU_SELECT_IF, g_settings.ifname, ifcount > 1, this, CRCInput::RC_nokey, "", true);

	bool found = false;

	for(int i = 0; i < ifcount; i++) 
	{
		ifSelect->addOption(namelist[i]->d_name);
		
		if(strcmp(g_settings.ifname, namelist[i]->d_name) == 0)
			found = true;
		free(namelist[i]);
	}

	if (ifcount >= 0)
		free(namelist);

	if(!found)
		strcpy(g_settings.ifname, "eth0");
	
	networkConfig->readConfig(g_settings.ifname);

	network_hostname = networkConfig->hostname;
	mac_addr = networkConfig->mac_addr;
	network_ssid = networkConfig->ssid;
	network_key = networkConfig->key;
	network_encryption = (networkConfig->encryption == "WPA") ? 0 : 1;
	
	//eth id
	CMenuForwarder * mac = new CMenuForwarder("MAC", false, mac_addr);
	
	CIPInput * networkSettings_NetworkIP  = new CIPInput(LOCALE_NETWORKMENU_IPADDRESS, networkConfig->address, LOCALE_IPSETUP_HINT_1, LOCALE_IPSETUP_HINT_2, MyIPChanger);

	CIPInput * networkSettings_NetMask    = new CIPInput(LOCALE_NETWORKMENU_NETMASK, networkConfig->netmask, LOCALE_IPSETUP_HINT_1, LOCALE_IPSETUP_HINT_2);

	CIPInput * networkSettings_Broadcast  = new CIPInput(LOCALE_NETWORKMENU_BROADCAST, networkConfig->broadcast, LOCALE_IPSETUP_HINT_1, LOCALE_IPSETUP_HINT_2);

	CIPInput * networkSettings_Gateway    = new CIPInput(LOCALE_NETWORKMENU_GATEWAY, networkConfig->gateway, LOCALE_IPSETUP_HINT_1, LOCALE_IPSETUP_HINT_2);

	CIPInput * networkSettings_NameServer = new CIPInput(LOCALE_NETWORKMENU_NAMESERVER, networkConfig->nameserver, LOCALE_IPSETUP_HINT_1, LOCALE_IPSETUP_HINT_2);
	
	//hostname
	CStringInputSMS * networkSettings_Hostname = new CStringInputSMS(LOCALE_NETWORKMENU_HOSTNAME, &network_hostname);

        CSectionsdConfigNotifier * sectionsdConfigNotifier = new CSectionsdConfigNotifier;
	// ntp server
        CStringInputSMS * networkSettings_NtpServer = new CStringInputSMS(LOCALE_NETWORKMENU_NTPSERVER, &g_settings.network_ntpserver, MAX_INPUT_CHARS, LOCALE_NETWORKMENU_NTPSERVER_HINT1, LOCALE_NETWORKMENU_NTPSERVER_HINT2, "abcdefghijklmnopqrstuvwxyz0123456789-. ", sectionsdConfigNotifier);
        CStringInput * networkSettings_NtpRefresh = new CStringInput(LOCALE_NETWORKMENU_NTPREFRESH, &g_settings.network_ntprefresh, 3, LOCALE_NETWORKMENU_NTPREFRESH_HINT1, LOCALE_NETWORKMENU_NTPREFRESH_HINT2 , "0123456789 ", sectionsdConfigNotifier);

	CMenuForwarder * m0 = new CMenuForwarder(LOCALE_NETWORKMENU_SETUPNOW, true, NULL, this, "network", CRCInput::RC_green, NEUTRINO_ICON_BUTTON_GREEN);
	CMenuForwarder * m1 = new CMenuForwarder(LOCALE_NETWORKMENU_IPADDRESS, networkConfig->inet_static, networkConfig->address, networkSettings_NetworkIP );
	CMenuForwarder * m2 = new CMenuForwarder(LOCALE_NETWORKMENU_NETMASK, networkConfig->inet_static, networkConfig->netmask, networkSettings_NetMask   );
	CMenuForwarder * m3 = new CMenuForwarder(LOCALE_NETWORKMENU_BROADCAST, networkConfig->inet_static, networkConfig->broadcast, networkSettings_Broadcast );
	CMenuForwarder * m4 = new CMenuForwarder(LOCALE_NETWORKMENU_GATEWAY, networkConfig->inet_static, networkConfig->gateway, networkSettings_Gateway   );
	CMenuForwarder * m5 = new CMenuForwarder(LOCALE_NETWORKMENU_NAMESERVER, networkConfig->inet_static, networkConfig->nameserver, networkSettings_NameServer);
        CMenuForwarder * m6 = new CMenuForwarder( LOCALE_NETWORKMENU_NTPSERVER, true, g_settings.network_ntpserver, networkSettings_NtpServer );
        CMenuForwarder * m7 = new CMenuForwarder( LOCALE_NETWORKMENU_NTPREFRESH, true, g_settings.network_ntprefresh, networkSettings_NtpRefresh );
	
	CMenuForwarder * m8 = new CMenuForwarder(LOCALE_NETWORKMENU_HOSTNAME, true, network_hostname, networkSettings_Hostname);

	CDHCPNotifier * dhcpNotifier = new CDHCPNotifier(m1, m2, m3, m4, m5);

	// setup network on startup
	network_automatic_start = networkConfig->automatic_start ? 1 : 0;
	CMenuOptionChooser * oj = new CMenuOptionChooser(LOCALE_NETWORKMENU_SETUPONSTARTUP, &network_automatic_start, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true);

	// intros
	networkSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	networkSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	// save settings
	networkSettings.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));
	networkSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	// setup network on start
	networkSettings.addItem( oj );

	// test network now
	networkSettings.addItem(new CMenuForwarder(LOCALE_NETWORKMENU_TEST, true, NULL, this, "networktest"));

	// show active network settings
	networkSettings.addItem(new CMenuForwarder(LOCALE_NETWORKMENU_SHOW, true, NULL, this, "networkshow", CRCInput::RC_info, NEUTRINO_ICON_BUTTON_HELP_SMALL));
	
	// setup network now
	networkSettings.addItem( m0 );
	
	// mac id
	networkSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE));
	networkSettings.addItem(mac);	//eth id
	
	// if select
	if(ifcount)
		networkSettings.addItem(ifSelect);	//if select
	else
		delete ifSelect;

	networkSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE));

	// dhcp on/off
	network_dhcp = networkConfig->inet_static ? 0 : 1;
	oj = new CMenuOptionChooser(LOCALE_NETWORKMENU_DHCP, &network_dhcp, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true, dhcpNotifier);
	networkSettings.addItem(oj);

	// hostname
	networkSettings.addItem( m8);

	// ip
	networkSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE));
	networkSettings.addItem( m1);

	// netmask
	networkSettings.addItem( m2);

	// broadcast
	networkSettings.addItem( m3);

	// default gateway
	networkSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE));
	networkSettings.addItem( m4);

	// nameserver
	networkSettings.addItem( m5);
	
	//
	if(ifcount > 1) // if there is only one, its probably wired
	{
		//ssid
		CStringInputSMS * networkSettings_ssid = new CStringInputSMS(LOCALE_NETWORKMENU_SSID, &network_ssid);
		CMenuForwarder * m9 = new CMenuForwarder(LOCALE_NETWORKMENU_SSID, networkConfig->wireless, network_ssid , networkSettings_ssid );
		//key
		CStringInputSMS *networkSettings_key = new CStringInputSMS(LOCALE_NETWORKMENU_PASSWORD, &network_key);
		CMenuForwarder *m10 = new CMenuForwarder(LOCALE_NETWORKMENU_PASSWORD, networkConfig->wireless, network_key , networkSettings_key );

		wlanEnable[0] = m9;
		wlanEnable[1] = m10;
		
		// ssid
		networkSettings.addItem( m9);

		// key
		networkSettings.addItem( m10);

		//encryption
		CMenuOptionChooser * m11 = new CMenuOptionChooser(LOCALE_NETWORKMENU_WLAN_SECURITY, &network_encryption, OPTIONS_WLAN_SECURITY_OPTIONS, OPTIONS_WLAN_SECURITY_OPTION_COUNT, true);
		wlanEnable[2] = m11;
		networkSettings.addItem( m11); //encryption
		networkSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE));
	}
	
	// ntp
	networkSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_NETWORKMENU_NTPTITLE));
	networkSettings.addItem(new CMenuOptionChooser(LOCALE_NETWORKMENU_NTPENABLE, &g_settings.network_ntpenable, OPTIONS_NTPENABLE_OPTIONS, OPTIONS_NTPENABLE_OPTION_COUNT, true, sectionsdConfigNotifier));
        networkSettings.addItem( m6);
        networkSettings.addItem( m7);
	
	//proxyserver submenu
	networkSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE));
	networkSettings.addItem(new CMenuForwarder(LOCALE_FLASHUPDATE_PROXYSERVER_SEP, true, NULL, new CProxySetup(LOCALE_FLASHUPDATE_PROXYSERVER_SEP), NULL, CRCInput::RC_nokey, NULL));

	// mount manager
	networkSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_NETWORKMENU_MOUNT));
	networkSettings.addItem(new CMenuForwarder(LOCALE_NFS_MOUNT , true, NULL, new CNFSMountGui(), NULL, CRCInput::RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW));
	networkSettings.addItem(new CMenuForwarder(LOCALE_NFS_UMOUNT, true, NULL, new CNFSUmountGui(), NULL, CRCInput::RC_blue, NEUTRINO_ICON_BUTTON_BLUE));
	
	networkSettings.exec(NULL, "");
	networkSettings.hide();
}

// movieplayer settings
#define MESSAGEBOX_NO_YES_OPTION_COUNT 2
const CMenuOptionChooser::keyval MESSAGEBOX_NO_YES_OPTIONS[MESSAGEBOX_NO_YES_OPTION_COUNT] =
{
	{ 0, LOCALE_MESSAGEBOX_NO, NULL },
	{ 1, LOCALE_MESSAGEBOX_YES, NULL }
};

CMoviePlayerSettings::CMoviePlayerSettings()
{
}

CMoviePlayerSettings::~CMoviePlayerSettings()
{
}

int CMoviePlayerSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CMoviePlayerSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	else if(actionKey == "moviedir")
	{
		if(parent)
			parent->hide();
		
		CFileBrowser b;
		b.Dir_Mode = true;

		if (b.exec(g_settings.network_nfs_moviedir))
			strncpy(g_settings.network_nfs_moviedir, b.getSelectedFile()->Name.c_str(), sizeof(g_settings.network_nfs_moviedir)-1);
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void CMoviePlayerSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CMoviePlayerSettings::showMenu:\n");
	
	CMenuWidget moviePlayerSettings(LOCALE_STREAMINGMENU_HEAD, NEUTRINO_ICON_STREAMING );
	
	// intros
	moviePlayerSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	moviePlayerSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	// save settings
	moviePlayerSettings.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));
	moviePlayerSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	// multi select
	moviePlayerSettings.addItem(new CMenuOptionChooser(LOCALE_STREAMINGMENU_FILEBROWSER_ALLOW_MULTISELECT, &g_settings.movieplayer_allow_multiselect, MESSAGEBOX_NO_YES_OPTIONS, MESSAGEBOX_NO_YES_OPTION_COUNT, true));

	// multiformat Dir
	moviePlayerSettings.addItem(new CMenuForwarder(LOCALE_MOVIEPLAYER_DEFDIR, true, g_settings.network_nfs_moviedir, this, "moviedir") ); 
	
	moviePlayerSettings.exec(NULL, "");
	moviePlayerSettings.hide();
}

// osd settings
COSDSettings::COSDSettings()
{
}

COSDSettings::~COSDSettings()
{
}

int COSDSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "COSDSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	else if(actionKey == "select_font")
	{
		CFileBrowser fileBrowser;
		CFileFilter fileFilter;
		fileFilter.addFilter("ttf");
		fileBrowser.Filter = &fileFilter;
		
		if (fileBrowser.exec(FONTDIR) == true)
		{
			strcpy(g_settings.font_file, fileBrowser.getSelectedFile()->Name.c_str());
			dprintf(DEBUG_NORMAL, "COSDSettings::exec: new font file %s\n", fileBrowser.getSelectedFile()->Name.c_str());
			
			CNeutrinoApp::getInstance()->SetupFonts();
		}
		
		return ret;
	}
	else if(actionKey == "font_scaling")
	{
		if(parent)
			parent->hide();
		
		CMenuWidget fontscale(LOCALE_FONTMENU_HEAD, NEUTRINO_ICON_COLORS);
		
		fontscale.enableSaveScreen(true);

		fontscale.addItem(new CMenuOptionNumberChooser(LOCALE_FONTMENU_SCALING_X, &g_settings.screen_xres, true, 50, 200, NULL) );
		fontscale.addItem(new CMenuOptionNumberChooser(LOCALE_FONTMENU_SCALING_Y, &g_settings.screen_yres, true, 50, 200, NULL) );
		
		fontscale.exec(NULL, "");
		
		CNeutrinoApp::getInstance()->SetupFonts();
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void COSDSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "COSDSettings::showMenu:\n");
	
	CMenuWidget osdSettings(LOCALE_MAINSETTINGS_OSD, NEUTRINO_ICON_COLORS );
	
	int shortcutOSD = 1;
	
	// intros
	osdSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	osdSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	// save settings
	osdSettings.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));
	osdSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );

	// Themes
	CThemes * osdSettings_Themes = new CThemes();
	
	osdSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_THEMESELECT, true, NULL, osdSettings_Themes, NULL, CRCInput::RC_green, NEUTRINO_ICON_BUTTON_GREEN) );

	// menu colors
	osdSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_MENUCOLORS, true, NULL, new COSDMenuColorSettings(), NULL, CRCInput::RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW) );

	// infobar
	//CMenuWidget * osdSettings_statusbarColors = new CMenuWidget(LOCALE_COLORMENU_STATUSBAR, NEUTRINO_ICON_SETTINGS);
	//InitColorSettingsStatusBarColors(*colorSettings_statusbarColors);

	osdSettings.addItem( new CMenuForwarder(LOCALE_COLORSTATUSBAR_HEAD, true, NULL, new COSDInfoBarColorSettings(), NULL, CRCInput::RC_blue, NEUTRINO_ICON_BUTTON_BLUE) );

	// language
	osdSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	osdSettings.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_LANGUAGE, true, NULL, new CLanguageSettings(), NULL, CRCInput::convertDigitToKey(shortcutOSD++) ));

	// font/timing
	osdSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_VIDEOMENU_OSD));
	
	// select font
	osdSettings.addItem( new CMenuForwarder(LOCALE_EPGPLUS_SELECT_FONT_NAME, true, NULL, this, "select_font", CRCInput::convertDigitToKey(shortcutOSD++) ));
	
	//font scaling
	osdSettings.addItem(new CMenuForwarder(LOCALE_FONTMENU_SCALING, true, NULL, this, "font_scaling", CRCInput::convertDigitToKey(shortcutOSD++) ));

	// osd timing
	osdSettings.addItem(new CMenuForwarder(LOCALE_TIMING_HEAD, true, NULL, new COSDTimingSettings(), NULL, CRCInput::convertDigitToKey(shortcutOSD++) ));

	// sceensetup
	osdSettings.addItem(new CMenuForwarder(LOCALE_VIDEOMENU_SCREENSETUP, true, NULL, new CScreenSetup(), NULL, CRCInput::convertDigitToKey(shortcutOSD++) ));
	
	// alpha setup
	osdSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE));

	CAlphaSetup * chAlphaSetup = new CAlphaSetup(LOCALE_COLORMENU_GTX_ALPHA, &g_settings.gtx_alpha);
	osdSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_GTX_ALPHA, true, NULL, chAlphaSetup, NULL, CRCInput::convertDigitToKey(shortcutOSD++)));
	
	osdSettings.exec(NULL, "");
	osdSettings.hide();
}

// osd menucolor settings
COSDMenuColorSettings::COSDMenuColorSettings()
{
}

COSDMenuColorSettings::~COSDMenuColorSettings()
{
}

int COSDMenuColorSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "COSDMenuColorSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void COSDMenuColorSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "COSDMenuColorSettings::showMenu:\n");
	
	CMenuWidget OSDmenuColorsSettings(LOCALE_COLORMENUSETUP_HEAD, NEUTRINO_ICON_SETTINGS );
	
	// intros
	OSDmenuColorsSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));

	// head
	CColorChooser* chHeadcolor = new CColorChooser(LOCALE_COLORMENU_BACKGROUND, &g_settings.menu_Head_red, &g_settings.menu_Head_green, &g_settings.menu_Head_blue, &g_settings.menu_Head_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);
	CColorChooser* chHeadTextcolor = new CColorChooser(LOCALE_COLORMENU_TEXTCOLOR, &g_settings.menu_Head_Text_red, &g_settings.menu_Head_Text_green, &g_settings.menu_Head_Text_blue,NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);

	// content
	CColorChooser* chContentcolor = new CColorChooser(LOCALE_COLORMENU_BACKGROUND, &g_settings.menu_Content_red, &g_settings.menu_Content_green, &g_settings.menu_Content_blue,&g_settings.menu_Content_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);
	CColorChooser* chContentTextcolor = new CColorChooser(LOCALE_COLORMENU_TEXTCOLOR, &g_settings.menu_Content_Text_red, &g_settings.menu_Content_Text_green, &g_settings.menu_Content_Text_blue,NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);

	// inactive
	CColorChooser* chContentInactivecolor = new CColorChooser(LOCALE_COLORMENU_BACKGROUND, &g_settings.menu_Content_inactive_red, &g_settings.menu_Content_inactive_green, &g_settings.menu_Content_inactive_blue,&g_settings.menu_Content_inactive_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);
	CColorChooser* chContentInactiveTextcolor = new CColorChooser(LOCALE_COLORMENU_TEXTCOLOR, &g_settings.menu_Content_inactive_Text_red, &g_settings.menu_Content_inactive_Text_green, &g_settings.menu_Content_inactive_Text_blue, NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);
	
	// selected
	CColorChooser* chContentSelectedcolor = new CColorChooser(LOCALE_COLORMENU_BACKGROUND, &g_settings.menu_Content_Selected_red, &g_settings.menu_Content_Selected_green, &g_settings.menu_Content_Selected_blue,&g_settings.menu_Content_Selected_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);
	CColorChooser* chContentSelectedTextcolor = new CColorChooser(LOCALE_COLORMENU_TEXTCOLOR, &g_settings.menu_Content_Selected_Text_red, &g_settings.menu_Content_Selected_Text_green, &g_settings.menu_Content_Selected_Text_blue,NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);
	
	// foot
	CColorChooser* chFootcolor = new CColorChooser(LOCALE_COLORMENU_BACKGROUND, &g_settings.menu_Foot_red, &g_settings.menu_Foot_green, &g_settings.menu_Foot_blue, &g_settings.menu_Foot_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);
	CColorChooser * chFootTextcolor = new CColorChooser(LOCALE_COLORMENU_TEXTCOLOR, &g_settings.menu_Foot_Text_red, &g_settings.menu_Foot_Text_green, &g_settings.menu_Foot_Text_blue, NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);

	// head
	OSDmenuColorsSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_COLORMENUSETUP_MENUHEAD));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_BACKGROUND, true, NULL, chHeadcolor ));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_TEXTCOLOR, true, NULL, chHeadTextcolor ));

	// menu content
	OSDmenuColorsSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_COLORMENUSETUP_MENUCONTENT));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_BACKGROUND, true, NULL, chContentcolor ));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_TEXTCOLOR, true, NULL, chContentTextcolor ));

	// inactiv
	OSDmenuColorsSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_COLORMENUSETUP_MENUCONTENT_INACTIVE));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_BACKGROUND, true, NULL, chContentInactivecolor ));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_TEXTCOLOR, true, NULL, chContentInactiveTextcolor));

	// selected
	OSDmenuColorsSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_COLORMENUSETUP_MENUCONTENT_SELECTED));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_BACKGROUND, true, NULL, chContentSelectedcolor ));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_TEXTCOLOR, true, NULL, chContentSelectedTextcolor ));
	
	// foot
	OSDmenuColorsSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_COLORMENU_HELPBAR));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_BACKGROUND, true, NULL, chFootcolor ));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_TEXTCOLOR, true, NULL, chFootTextcolor ));
	
	OSDmenuColorsSettings.exec(NULL, "");
	OSDmenuColorsSettings.hide();
}

// osd infobarcolor settings
COSDInfoBarColorSettings::COSDInfoBarColorSettings()
{
}

COSDInfoBarColorSettings::~COSDInfoBarColorSettings()
{
}

int COSDInfoBarColorSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "COSDInfoBarColorSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void COSDInfoBarColorSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "COSDInfoBarColorSettings::showMenu:\n");
	
	CMenuWidget OSDinfobarColorSettings(LOCALE_COLORMENU_STATUSBAR, NEUTRINO_ICON_SETTINGS);
	
	// intros
	OSDinfobarColorSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));

	// bg
	CColorChooser * chInfobarcolor = new CColorChooser(LOCALE_COLORMENU_BACKGROUND, &g_settings.infobar_red, &g_settings.infobar_green, &g_settings.infobar_blue,&g_settings.infobar_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);
	
	// text
	CColorChooser * chInfobarTextcolor = new CColorChooser(LOCALE_COLORMENU_TEXTCOLOR, &g_settings.infobar_Text_red, &g_settings.infobar_Text_green, &g_settings.infobar_Text_blue,NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);
	
	// clolored events
	CColorChooser * chColored_Events = new CColorChooser(LOCALE_COLORMENU_TEXTCOLOR, &g_settings.infobar_colored_events_red, &g_settings.infobar_colored_events_green, &g_settings.infobar_colored_events_blue, NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);

	OSDinfobarColorSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_COLORSTATUSBAR_TEXT));

	OSDinfobarColorSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_BACKGROUND, true, NULL, chInfobarcolor ));

	OSDinfobarColorSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_TEXTCOLOR, true, NULL, chInfobarTextcolor ));
	
	// clored events
	OSDinfobarColorSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_MISCSETTINGS_INFOBAR_COLORED_EVENTS));
	OSDinfobarColorSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_TEXTCOLOR, true, NULL, chColored_Events ));
	
	OSDinfobarColorSettings.exec(NULL, "");
	OSDinfobarColorSettings.hide();
}

// osd language settings
CLanguageSettings::CLanguageSettings()
{
}

CLanguageSettings::~CLanguageSettings()
{
}

int CLanguageSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CLanguageSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	showMenu();
	
	return ret;
}

bool CLanguageSettings::changeNotify(const neutrino_locale_t OptionName, void */*data*/)
{
	if (ARE_LOCALES_EQUAL(OptionName, LOCALE_LANGUAGESETUP_SELECT)) 
	{
		dprintf(DEBUG_NORMAL, "CLanguageSettings::changeNotify: %s\n", g_settings.language);
		
		// setup font first
		if(strstr(g_settings.language, "arabic"))
		{
			//if( !strstr(g_settings.font_file, "nmsbd.ttf") || !strstr(g_settings.font_file, "ae_AlMateen.ttf") )
			{
				//if(MessageBox(LOCALE_MESSAGEBOX_INFO, "do you want to change your font to nmsbd.ttf\nthis font support your language\n", CMessageBox::mbrYes, CMessageBox::mbYes | CMessageBox::mbNo) == CMessageBox::mbrYes)
				{
					// check for nsmbd font
					if(!access(FONTDIR "/nmsbd.ttf", F_OK))
					{
						strcpy(g_settings.font_file, FONTDIR "/nmsbd.ttf");
						printf("CLanguageSettings::changeNotify:new font file %s\n", g_settings.font_file);
						CNeutrinoApp::getInstance()->SetupFonts();
					}
					else
					{
						HintBox(LOCALE_MESSAGEBOX_INFO, "install a font supporting your language (e.g nmsbd.ttf)");
					}
				}
			}
		}
		
		g_Locale->loadLocale(g_settings.language);
		return true;
	}
	
	return false;
}

void CLanguageSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CLanguageSettings::showMenu:\n");
	
	CMenuWidget languageSettings(LOCALE_LANGUAGESETUP_HEAD, NEUTRINO_ICON_LANGUAGE );
	
	// intros
	languageSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	languageSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE));

	struct dirent **namelist;
	int n;

	//printf("scanning locale dir now....(perhaps)\n");

	char *path[] = {(char *) DATADIR "/neutrino/locale", (char *) CONFIGDIR "/locale"};

	for(int p = 0; p < 2; p++) 
	{
		n = scandir(path[p], &namelist, 0, alphasort);
		
		if(n > 0)
		{
			for(int count = 0; count < n; count++) 
			{
				char * locale = strdup(namelist[count]->d_name);
				char * pos = strstr(locale, ".locale");

				if(pos != NULL) 
				{
					*pos = '\0';
				
					CMenuOptionLanguageChooser* oj = new CMenuOptionLanguageChooser((char*)locale, this, locale);
					oj->addOption(locale);
					languageSettings.addItem( oj );
				} 
				else
					free(locale);
				free(namelist[count]);
			}
			free(namelist);
		}
	}
	
	languageSettings.exec(NULL, "");
	languageSettings.hide();
}

// osd timing settings
static CTimingSettingsNotifier timingsettingsnotifier;

COSDTimingSettings::COSDTimingSettings()
{
}

COSDTimingSettings::~COSDTimingSettings()
{
}

int COSDTimingSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "COSDTimingSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	else if(actionKey == "osd.def") 
	{
		for (int i = 0; i < TIMING_SETTING_COUNT; i++)
			g_settings.timing[i] = default_timing[i];

		CNeutrinoApp::getInstance()->SetupTiming();
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void COSDTimingSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "COSDTimingSettings::showMenu:\n");
	
	CMenuWidget osdTimingSettings(LOCALE_COLORMENU_TIMING, NEUTRINO_ICON_SETTINGS);
	
	// intros
	osdTimingSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	osdTimingSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE));

	for (int i = 0; i < TIMING_SETTING_COUNT; i++)
	{
		CStringInput * colorSettings_timing_item = new CStringInput(timing_setting_name[i], g_settings.timing_string[i], 3, LOCALE_TIMING_HINT_1, LOCALE_TIMING_HINT_2, "0123456789 ", &timingsettingsnotifier);
		osdTimingSettings.addItem(new CMenuForwarder(timing_setting_name[i], true, g_settings.timing_string[i], colorSettings_timing_item));
	}

	osdTimingSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE));
	osdTimingSettings.addItem(new CMenuForwarder(LOCALE_OPTIONS_DEFAULT, true, NULL, this, "osd.def"));
	
	osdTimingSettings.exec(NULL, "");
	osdTimingSettings.hide();
}

// audioplayer settings
#define AUDIOPLAYER_DISPLAY_ORDER_OPTION_COUNT 2
const CMenuOptionChooser::keyval AUDIOPLAYER_DISPLAY_ORDER_OPTIONS[AUDIOPLAYER_DISPLAY_ORDER_OPTION_COUNT] =
{
	{ CAudioPlayerGui::ARTIST_TITLE, LOCALE_AUDIOPLAYER_ARTIST_TITLE, NULL },
	{ CAudioPlayerGui::TITLE_ARTIST, LOCALE_AUDIOPLAYER_TITLE_ARTIST, NULL }
};

CAudioPlayerSettings::CAudioPlayerSettings()
{
}

CAudioPlayerSettings::~CAudioPlayerSettings()
{
}

int CAudioPlayerSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CAudioPlayerSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	else if(actionKey == "audioplayerdir")
	{
		if(parent)
			parent->hide();
		
		CFileBrowser b;
		b.Dir_Mode = true;
		
		if (b.exec(g_settings.network_nfs_audioplayerdir))
			strncpy(g_settings.network_nfs_audioplayerdir, b.getSelectedFile()->Name.c_str(), sizeof(g_settings.network_nfs_audioplayerdir)-1);
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void CAudioPlayerSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CAudioPlayerSettings::showMenu:\n");
	
	CMenuWidget audioPlayerSettings(LOCALE_AUDIOPLAYERSETTINGS_GENERAL, NEUTRINO_ICON_SETTINGS);
	
	int shortcutAudioPlayer = 1;
	
	// intros
	audioPlayerSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	audioPlayerSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	// save settings
	audioPlayerSettings.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));
	audioPlayerSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );

	// Audio Player
	audioPlayerSettings.addItem(new CMenuOptionChooser(LOCALE_AUDIOPLAYER_DISPLAY_ORDER, &g_settings.audioplayer_display, AUDIOPLAYER_DISPLAY_ORDER_OPTIONS, AUDIOPLAYER_DISPLAY_ORDER_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcutAudioPlayer++), "", true ));

	// select ton pid
	audioPlayerSettings.addItem(new CMenuOptionChooser(LOCALE_AUDIOPLAYER_FOLLOW, &g_settings.audioplayer_follow, MESSAGEBOX_NO_YES_OPTIONS, MESSAGEBOX_NO_YES_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcutAudioPlayer++) ));

	// select by title
	audioPlayerSettings.addItem(new CMenuOptionChooser(LOCALE_AUDIOPLAYER_SELECT_TITLE_BY_NAME, &g_settings.audioplayer_select_title_by_name, MESSAGEBOX_NO_YES_OPTIONS, MESSAGEBOX_NO_YES_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcutAudioPlayer++) ));

	// repeat
	audioPlayerSettings.addItem(new CMenuOptionChooser(LOCALE_AUDIOPLAYER_REPEAT_ON, &g_settings.audioplayer_repeat_on, MESSAGEBOX_NO_YES_OPTIONS, MESSAGEBOX_NO_YES_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcutAudioPlayer++) ));

	// hide playlist
	audioPlayerSettings.addItem(new CMenuOptionChooser(LOCALE_AUDIOPLAYER_HIDE_PLAYLIST, &g_settings.audioplayer_hide_playlist, MESSAGEBOX_NO_YES_OPTIONS, MESSAGEBOX_NO_YES_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcutAudioPlayer++) ));

	// high prio
	audioPlayerSettings.addItem(new CMenuOptionChooser(LOCALE_AUDIOPLAYER_HIGHPRIO, &g_settings.audioplayer_highprio, MESSAGEBOX_NO_YES_OPTIONS, MESSAGEBOX_NO_YES_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcutAudioPlayer++) ));

	// start dir
	audioPlayerSettings.addItem(new CMenuForwarder(LOCALE_AUDIOPLAYER_DEFDIR, true, g_settings.network_nfs_audioplayerdir, this, "audioplayerdir", CRCInput::convertDigitToKey(shortcutAudioPlayer++)));
	
	audioPlayerSettings.exec(NULL, "");
	audioPlayerSettings.hide();
}

// pictureviewer settings
#define PICTUREVIEWER_SCALING_OPTION_COUNT 3
const CMenuOptionChooser::keyval PICTUREVIEWER_SCALING_OPTIONS[PICTUREVIEWER_SCALING_OPTION_COUNT] =
{
	{ CFrameBuffer::SIMPLE, LOCALE_PICTUREVIEWER_RESIZE_SIMPLE, NULL        },
	{ CFrameBuffer::COLOR , LOCALE_PICTUREVIEWER_RESIZE_COLOR_AVERAGE, NULL },
	{ CFrameBuffer::NONE  , LOCALE_PICTUREVIEWER_RESIZE_NONE, NULL         }
};

CPictureViewerSettings::CPictureViewerSettings()
{
}

CPictureViewerSettings::~CPictureViewerSettings()
{
}

int CPictureViewerSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CPicTureViewerSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	else if(actionKey == "picturedir")
	{
		if(parent)
			parent->hide();
		
		CFileBrowser b;
		b.Dir_Mode = true;
		
		if (b.exec(g_settings.network_nfs_picturedir))
			strncpy(g_settings.network_nfs_picturedir, b.getSelectedFile()->Name.c_str(), sizeof(g_settings.network_nfs_picturedir)-1);
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void CPictureViewerSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CPicTureViewerSettings::showMenu:\n");
	
	CMenuWidget PicViewerSettings(LOCALE_PICTUREVIEWERSETTINGS_GENERAL, NEUTRINO_ICON_SETTINGS );
	
	int shortcutPicViewer = 1;
	
	// intros
	PicViewerSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	PicViewerSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	// save settings
	PicViewerSettings.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));
	PicViewerSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );

	// Pic Viewer Scaling
	PicViewerSettings.addItem(new CMenuOptionChooser(LOCALE_PICTUREVIEWER_SCALING, &g_settings.picviewer_scaling, PICTUREVIEWER_SCALING_OPTIONS, PICTUREVIEWER_SCALING_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcutPicViewer++),"", true ));

	// slide Timeout
	CStringInput * pic_timeout= new CStringInput(LOCALE_PICTUREVIEWER_SLIDE_TIME, g_settings.picviewer_slide_time, 2, NONEXISTANT_LOCALE, NONEXISTANT_LOCALE, "0123456789 ");
	PicViewerSettings.addItem(new CMenuForwarder(LOCALE_PICTUREVIEWER_SLIDE_TIME, true, g_settings.picviewer_slide_time, pic_timeout, NULL, CRCInput::convertDigitToKey(shortcutPicViewer++)));

	// Pic Viewer Default Dir
	PicViewerSettings.addItem(new CMenuForwarder(LOCALE_PICTUREVIEWER_DEFDIR, true, g_settings.network_nfs_picturedir, this, "picturedir", CRCInput::convertDigitToKey(shortcutPicViewer++)));
	
	PicViewerSettings.exec(NULL, "");
	PicViewerSettings.hide();
}

// lcd settings
#if defined (ENABLE_LCD)
#define LCDMENU_STATUSLINE_OPTION_COUNT 4
const CMenuOptionChooser::keyval LCDMENU_STATUSLINE_OPTIONS[LCDMENU_STATUSLINE_OPTION_COUNT] =
{
	{ 0, LOCALE_LCDMENU_STATUSLINE_PLAYTIME, NULL   },
	{ 1, LOCALE_LCDMENU_STATUSLINE_VOLUME, NULL     },
	{ 2, LOCALE_LCDMENU_STATUSLINE_BOTH, NULL       },
	{ 3, LOCALE_LCDMENU_STATUSLINE_BOTH_AUDIO, NULL }
};

#define LCDMENU_EPG_OPTION_COUNT 6
const CMenuOptionChooser::keyval LCDMENU_EPG_OPTIONS[LCDMENU_EPG_OPTION_COUNT] =
{
	{ 1, LOCALE_LCDMENU_EPG_NAME, NULL		},
	{ 2, LOCALE_LCDMENU_EPG_TITLE, NULL		},
	{ 3, LOCALE_LCDMENU_EPG_NAME_TITLE, NULL	},
	{ 7, LOCALE_LCDMENU_EPG_NAME_SEPLINE_TITLE, NULL },
	{ 11, LOCALE_LCDMENU_EPG_NAMESHORT_TITLE, NULL },
	{ 15, LOCALE_LCDMENU_EPG_NAMESHORT_SEPLINE_TITLE, NULL }
};

#define LCDMENU_EPGALIGN_OPTION_COUNT 2
const CMenuOptionChooser::keyval LCDMENU_EPGALIGN_OPTIONS[LCDMENU_EPGALIGN_OPTION_COUNT] =
{
	{ 0, LOCALE_LCDMENU_EPGALIGN_LEFT, NULL   },
	{ 1, LOCALE_LCDMENU_EPGALIGN_CENTER, NULL }
};
#endif

#if defined (PLATFORM_GIGABLUE) 
#if !defined (ENABLE_LCD)
#define LCDMENU_LEDCOLOR_OPTION_COUNT 4
const CMenuOptionChooser::keyval LCDMENU_LEDCOLOR_OPTIONS[LCDMENU_LEDCOLOR_OPTION_COUNT] =
{
	{ CVFD::LED_OFF, LOCALE_OPTIONS_OFF, NULL },
	{ CVFD::LED_BLUE, LOCALE_LCDMENU_LEDCOLOR_BLUE, NULL },
	{ CVFD::LED_RED, LOCALE_LCDMENU_LEDCOLOR_RED, NULL },
	{ CVFD::LED_PURPLE, LOCALE_LCDMENU_LEDCOLOR_PURPLE, NULL },
};
#endif
#endif

CLCDSettings::CLCDSettings()
{
}

CLCDSettings::~CLCDSettings()
{
}

int CLCDSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CLCDSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void CLCDSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CLCDSettings::showMenu:\n");
	
	CMenuWidget lcdSettings(LOCALE_LCDMENU_HEAD, NEUTRINO_ICON_LCD );
	
	int shortcutVFD = 1;
	
	// intros
	lcdSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	lcdSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	// save settings
	lcdSettings.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));
	lcdSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE));
	
	CLcdNotifier * lcdnotifier = new CLcdNotifier();
	
	CVfdControler * lcdsliders = new CVfdControler(LOCALE_LCDMENU_HEAD, NULL);
	
	// LCD
#if defined (ENABLE_LCD)
	//option invert
	CMenuOptionChooser* oj_inverse = new CMenuOptionChooser(LOCALE_LCDMENU_INVERSE, &g_settings.lcd_setting[SNeutrinoSettings::LCD_INVERSE], OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true, lcdnotifier, CRCInput::convertDigitToKey(shortcutVFD++) );
	lcdSettings.addItem(oj_inverse);

	//status display
	CMenuOptionChooser* oj_status = new CMenuOptionChooser(LOCALE_LCDMENU_STATUSLINE, &g_settings.lcd_setting[SNeutrinoSettings::LCD_SHOW_VOLUME], LCDMENU_STATUSLINE_OPTIONS, LCDMENU_STATUSLINE_OPTION_COUNT, true);
	lcdSettings.addItem(oj_status);
	
	//lcd_epg
	CMenuOptionChooser* oj_epg = new CMenuOptionChooser(LOCALE_LCDMENU_EPG, &g_settings.lcd_setting[SNeutrinoSettings::LCD_EPGMODE], LCDMENU_EPG_OPTIONS, LCDMENU_EPG_OPTION_COUNT, true);
	lcdSettings.addItem(oj_epg);

	//align
	CMenuOptionChooser* oj_align = new CMenuOptionChooser(LOCALE_LCDMENU_EPGALIGN, &g_settings.lcd_setting[SNeutrinoSettings::LCD_EPGALIGN], LCDMENU_EPGALIGN_OPTIONS, LCDMENU_EPGALIGN_OPTION_COUNT, true);
	lcdSettings.addItem(oj_align);

	//dump to png
	CMenuOptionChooser* oj_dumppng = new CMenuOptionChooser(LOCALE_LCDMENU_DUMP_PNG, &g_settings.lcd_setting[SNeutrinoSettings::LCD_DUMP_PNG], OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true);
	lcdSettings.addItem(oj_dumppng);
	
	// lcd controller
	lcdSettings.addItem(new CMenuForwarder(LOCALE_LCDMENU_LCDCONTROLER, true, NULL, lcdsliders, NULL, CRCInput::convertDigitToKey(shortcutVFD++) ));
#else	
#if defined (PLATFORM_GIGABLUE)	
	// led color
	lcdSettings.addItem(new CMenuOptionChooser(LOCALE_LCDMENU_LEDCOLOR, &g_settings.lcd_ledcolor, LCDMENU_LEDCOLOR_OPTIONS, LCDMENU_LEDCOLOR_OPTION_COUNT, true, lcdnotifier, CRCInput::convertDigitToKey(shortcutVFD++) ));	
#elif !defined (PLATFORM_CUBEREVO_250HD) && !defined (PLATFORM_SPARK)
	// vfd power
	CMenuOptionChooser * oj2 = new CMenuOptionChooser(LOCALE_LCDMENU_POWER, &g_settings.lcd_setting[SNeutrinoSettings::LCD_POWER], OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true, lcdnotifier, CRCInput::convertDigitToKey(shortcutVFD++) );
	lcdSettings.addItem(oj2);
	
	// dimm-time
	CStringInput * dim_time = new CStringInput(LOCALE_LCDMENU_DIM_TIME, g_settings.lcd_setting_dim_time, 3, NONEXISTANT_LOCALE, NONEXISTANT_LOCALE,"0123456789 ");
	lcdSettings.addItem(new CMenuForwarder(LOCALE_LCDMENU_DIM_TIME,true, g_settings.lcd_setting_dim_time, dim_time, NULL, CRCInput::convertDigitToKey(shortcutVFD++)));

	// dimm brightness
	//CStringInput * dim_brightness = new CStringInput(LOCALE_LCDMENU_DIM_BRIGHTNESS, g_settings.lcd_setting_dim_brightness, 3,NONEXISTANT_LOCALE, NONEXISTANT_LOCALE,"0123456789 ");
	//lcdSettings.addItem(new CMenuForwarder(LOCALE_LCDMENU_DIM_BRIGHTNESS,true, g_settings.lcd_setting_dim_brightness, dim_brightness, NULL, CRCInput::convertDigitToKey(shortcutVFD++) ));

	// vfd controller
	lcdSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE));
	lcdSettings.addItem(new CMenuForwarder(LOCALE_LCDMENU_LCDCONTROLER, true, NULL, lcdsliders, NULL, CRCInput::convertDigitToKey(shortcutVFD++) ));	
#endif	
#endif	
	
	lcdSettings.exec(NULL, "");
	lcdSettings.hide();
}

// remote control settings
enum keynames {
	// zap
	KEY_TV_RADIO_MODE,
	KEY_PAGE_UP,
	KEY_PAGE_DOWN,
	KEY_LIST_START,
	KEY_LIST_END,
	KEY_CANCEL_ACTION,
	KEY_SORT,
	KEY_RELOAD,
	VKEY_SEARCH,
	KEY_ADD_RECORD,
	KEY_ADD_REMIND,
	KEY_BOUQUET_UP,
	KEY_BOUQUET_DOWN,
	KEY_CHANNEL_UP,
	KEY_CHANNEL_DOWN,
	KEY_SUBCHANNEL_UP,
	KEY_SUBCHANNEL_DOWN,
	KEY_ZAP_HISTORY,
	KEY_LASTCHANNEL,
	KEY_SAME_TP,
	
	// mp
        MPKEY_REWIND,
        MPKEY_FORWARD,
        MPKEY_PAUSE,
        MPKEY_STOP,
        MPKEY_PLAY,
        MPKEY_AUDIO,
        MPKEY_TIME,
        MPKEY_BOOKMARK,
	KEY_TIMESHIFT,
	
	// media
	KEY_EXTRAS_RECORDSBROWSER,
	KEY_EXTRAS_AUDIOPLAYER,
	KEY_EXTRAS_PICTUREVIEWER,
	KEY_EXTRAS_TIMERLIST,
	KEY_EXTRAS_INETRADIO,
	KEY_EXTRAS_MOVIEBROWSER,
	KEY_EXTRAS_FILEBROWSER,
	KEY_EXTRAS_WEBTV,
	KEY_EXTRAS_SCREENSHOT,
	
	// mb
	KEY_EXTRAS_MB_COPY_JUMP,
	KEY_EXTRAS_MB_CUT_JUMP,
	KEY_EXTRAS_MB_TRUNCATE
};

#define KEYBINDS_COUNT 41
const neutrino_locale_t keydescription_head[KEYBINDS_COUNT] =
{
	// zap
	LOCALE_KEYBINDINGMENU_TVRADIOMODE,
	LOCALE_KEYBINDINGMENU_PAGEUP,
	LOCALE_KEYBINDINGMENU_PAGEDOWN,
	LOCALE_EXTRA_KEY_LIST_START,
	LOCALE_EXTRA_KEY_LIST_END,
	LOCALE_KEYBINDINGMENU_CANCEL,
	LOCALE_KEYBINDINGMENU_SORT,
	LOCALE_KEYBINDINGMENU_RELOAD,
	LOCALE_KEYBINDINGMENU_SEARCH,
	LOCALE_KEYBINDINGMENU_ADDRECORD,
	LOCALE_KEYBINDINGMENU_ADDREMIND,
	LOCALE_KEYBINDINGMENU_BOUQUETUP,
	LOCALE_KEYBINDINGMENU_BOUQUETDOWN,
	LOCALE_KEYBINDINGMENU_CHANNELUP,
	LOCALE_KEYBINDINGMENU_CHANNELDOWN,
	LOCALE_KEYBINDINGMENU_SUBCHANNELUP,
	LOCALE_KEYBINDINGMENU_SUBCHANNELDOWN,
	LOCALE_KEYBINDINGMENU_ZAPHISTORY,
	LOCALE_KEYBINDINGMENU_LASTCHANNEL,
	LOCALE_KEYBINDINGMENU_PIP,
	
	// mp
        LOCALE_MPKEY_REWIND,
        LOCALE_MPKEY_FORWARD,
        LOCALE_MPKEY_PAUSE,
        LOCALE_MPKEY_STOP,
        LOCALE_MPKEY_PLAY,
        LOCALE_MPKEY_AUDIO,
        LOCALE_MPKEY_TIME,
        LOCALE_MPKEY_BOOKMARK,
	LOCALE_EXTRA_KEY_TIMESHIFT,

	// media
	LOCALE_KEYBINDINGMENU_RECORDSBROWSER,
	LOCALE_KEYBINDINGMENU_AUDIOPLAYER,
	LOCALE_KEYBINDINGMENU_PICTUREVIEWER,
	LOCALE_KEYBINDINGMENU_TIMERLIST,
	LOCALE_KEYBINDINGMENU_INETRADIO,
	LOCALE_KEYBINDINGMENU_MOVIEBROWSER,
	LOCALE_KEYBINDINGMENU_FILEBROWSER,
	LOCALE_KEYBINDINGMENU_WEBTV,
	LOCALE_KEYBINDINGMENU_SCREENSHOT,
	
	// mb
	LOCALE_KEYBINDINGMENU_MB_COPY_JUMP,
	LOCALE_KEYBINDINGMENU_MB_CUT_JUMP,
	LOCALE_KEYBINDINGMENU_MB_TRUNCATE
};

const neutrino_locale_t keydescription[KEYBINDS_COUNT] =
{
	// zap
	LOCALE_KEYBINDINGMENU_TVRADIOMODE,
	LOCALE_KEYBINDINGMENU_PAGEUP,
	LOCALE_KEYBINDINGMENU_PAGEDOWN,
	LOCALE_EXTRA_KEY_LIST_START,
	LOCALE_EXTRA_KEY_LIST_END,
	LOCALE_KEYBINDINGMENU_CANCEL,
	LOCALE_KEYBINDINGMENU_SORT,
	LOCALE_KEYBINDINGMENU_RELOAD,
	LOCALE_KEYBINDINGMENU_SEARCH,
	LOCALE_KEYBINDINGMENU_ADDRECORD,
	LOCALE_KEYBINDINGMENU_ADDREMIND,
	LOCALE_KEYBINDINGMENU_BOUQUETUP,
	LOCALE_KEYBINDINGMENU_BOUQUETDOWN,
	LOCALE_KEYBINDINGMENU_CHANNELUP,
	LOCALE_KEYBINDINGMENU_CHANNELDOWN,
	LOCALE_KEYBINDINGMENU_SUBCHANNELUP,
	LOCALE_KEYBINDINGMENU_SUBCHANNELDOWN,
	LOCALE_KEYBINDINGMENU_ZAPHISTORY,
	LOCALE_KEYBINDINGMENU_LASTCHANNEL,
	LOCALE_KEYBINDINGMENU_PIP,
	
	// mp
        LOCALE_MPKEY_REWIND,
        LOCALE_MPKEY_FORWARD,
        LOCALE_MPKEY_PAUSE,
        LOCALE_MPKEY_STOP,
        LOCALE_MPKEY_PLAY,
        LOCALE_MPKEY_AUDIO,
        LOCALE_MPKEY_TIME,
        LOCALE_MPKEY_BOOKMARK,
	LOCALE_EXTRA_KEY_TIMESHIFT,
	
	// media
	LOCALE_KEYBINDINGMENU_RECORDSBROWSER,
	LOCALE_KEYBINDINGMENU_AUDIOPLAYER,
	LOCALE_KEYBINDINGMENU_PICTUREVIEWER,
	LOCALE_KEYBINDINGMENU_TIMERLIST,
	LOCALE_KEYBINDINGMENU_INETRADIO,
	LOCALE_KEYBINDINGMENU_MOVIEBROWSER,
	LOCALE_KEYBINDINGMENU_FILEBROWSER,
	LOCALE_KEYBINDINGMENU_WEBTV,
	LOCALE_KEYBINDINGMENU_SCREENSHOT,
	
	// mb
	LOCALE_KEYBINDINGMENU_MB_COPY_JUMP,
	LOCALE_KEYBINDINGMENU_MB_CUT_JUMP,
	LOCALE_KEYBINDINGMENU_MB_TRUNCATE
};

CRemoteControlSettings::CRemoteControlSettings()
{
}

CRemoteControlSettings::~CRemoteControlSettings()
{
}

int CRemoteControlSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CRemoteControlSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void CRemoteControlSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CRemoteControlSettings::showMenu:\n");
	
	int shortcutkeysettings = 1;
	
	CMenuWidget remoteControlSettings(LOCALE_MAINSETTINGS_KEYBINDING, NEUTRINO_ICON_KEYBINDING );
	
	// intros
	remoteControlSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	remoteControlSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	// save settings
	remoteControlSettings.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));

	keySetupNotifier = new CKeySetupNotifier;
	
	// repeat generic blocker
	CStringInput * remoteControlSettings_repeat_genericblocker = new CStringInput(LOCALE_KEYBINDINGMENU_REPEATBLOCKGENERIC, g_settings.repeat_genericblocker, 3, LOCALE_REPEATBLOCKER_HINT_1, LOCALE_REPEATBLOCKER_HINT_2, "0123456789 ", keySetupNotifier);
	
	// repeat blocker
	CStringInput * remoteControlSettings_repeatBlocker = new CStringInput(LOCALE_KEYBINDINGMENU_REPEATBLOCK, g_settings.repeat_blocker, 3, LOCALE_REPEATBLOCKER_HINT_1, LOCALE_REPEATBLOCKER_HINT_2, "0123456789 ", keySetupNotifier);
	keySetupNotifier->changeNotify(NONEXISTANT_LOCALE, NULL);

	remoteControlSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_KEYBINDINGMENU_RC));
	
	// repeat blocker
	remoteControlSettings.addItem(new CMenuForwarder(LOCALE_KEYBINDINGMENU_REPEATBLOCK, true, g_settings.repeat_blocker, remoteControlSettings_repeatBlocker, NULL, CRCInput::convertDigitToKey(shortcutkeysettings++)));
	
	// repeat generic blocker
 	remoteControlSettings.addItem(new CMenuForwarder(LOCALE_KEYBINDINGMENU_REPEATBLOCKGENERIC, true, g_settings.repeat_genericblocker, remoteControlSettings_repeat_genericblocker, NULL, CRCInput::convertDigitToKey(shortcutkeysettings++)));

	// keybinding menu
	remoteControlSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_KEYBINDINGMENU_HEAD));
	
	remoteControlSettings.addItem(new CMenuForwarder(LOCALE_KEYBINDINGMENU_HEAD, true, NULL, new CKeysBindingSettings(), NULL, CRCInput::convertDigitToKey(shortcutkeysettings++)));

        // usermenu 
        remoteControlSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_USERMENU_HEAD));
	
        remoteControlSettings.addItem(new CMenuForwarder(LOCALE_USERMENU_BUTTON_RED, true, NULL, new CUserMenuMenu(LOCALE_USERMENU_BUTTON_RED, 0), NULL, CRCInput::convertDigitToKey(shortcutkeysettings++) ));
        remoteControlSettings.addItem(new CMenuForwarder(LOCALE_USERMENU_BUTTON_GREEN, true, NULL, new CUserMenuMenu(LOCALE_USERMENU_BUTTON_GREEN, 1), NULL, CRCInput::convertDigitToKey(shortcutkeysettings++) ));
        remoteControlSettings.addItem(new CMenuForwarder(LOCALE_USERMENU_BUTTON_YELLOW, true, NULL, new CUserMenuMenu(LOCALE_USERMENU_BUTTON_YELLOW, 2), NULL, CRCInput::convertDigitToKey(shortcutkeysettings++) ));
        remoteControlSettings.addItem(new CMenuForwarder(LOCALE_USERMENU_BUTTON_BLUE, true, NULL, new CUserMenuMenu(LOCALE_USERMENU_BUTTON_BLUE, 3), NULL, CRCInput::convertDigitToKey(shortcutkeysettings++) ));
#if defined (ENABLE_FUNCTIONKEYS)	
	remoteControlSettings.addItem(new CMenuForwarder(LOCALE_USERMENU_BUTTON_F1, true, NULL, new CUserMenuMenu(LOCALE_USERMENU_BUTTON_F1, 4) ));
        remoteControlSettings.addItem(new CMenuForwarder(LOCALE_USERMENU_BUTTON_F2, true, NULL, new CUserMenuMenu(LOCALE_USERMENU_BUTTON_F2, 5) ));
        remoteControlSettings.addItem(new CMenuForwarder(LOCALE_USERMENU_BUTTON_F3, true, NULL, new CUserMenuMenu(LOCALE_USERMENU_BUTTON_F3, 6) ));
        remoteControlSettings.addItem(new CMenuForwarder(LOCALE_USERMENU_BUTTON_F4, true, NULL, new CUserMenuMenu(LOCALE_USERMENU_BUTTON_BLUE, 7) ));	
#endif
	
	remoteControlSettings.exec(NULL, "");
	remoteControlSettings.hide();
}

// keys binding settings
CKeysBindingSettings::CKeysBindingSettings()
{
}

CKeysBindingSettings::~CKeysBindingSettings()
{
}

int CKeysBindingSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CKeysBindingSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	else if(actionKey == "savekeymap")
	{
		CHintBox * hintBox = new CHintBox(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_KEYBINDINGMENU_SAVEKEYMAP_HINT)); // UTF-8
		hintBox->paint();
		
		g_RCInput->configfile.setModifiedFlag(true);
		g_RCInput->saveKeyMap(NEUTRINO_KEYMAP_FILE);
		
		sleep(2);
		
		hintBox->hide();
		delete hintBox;
		hintBox = NULL;

		return menu_return::RETURN_REPAINT;	
	}
	
	showMenu();
	
	return ret;
}

void CKeysBindingSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CKeysBindingSettings::showMenu:\n");

	int * keyvalue_p[KEYBINDS_COUNT] =
	{
		// zap
		&g_settings.key_tvradio_mode,
		&g_settings.key_channelList_pageup,
		&g_settings.key_channelList_pagedown,
		&g_settings.key_list_start,
		&g_settings.key_list_end,
		&g_settings.key_channelList_cancel,
		&g_settings.key_channelList_sort,
		&g_settings.key_channelList_reload,
		&g_settings.key_channelList_search,
		&g_settings.key_channelList_addrecord,
		&g_settings.key_channelList_addremind,
		&g_settings.key_bouquet_up,
		&g_settings.key_bouquet_down,
		&g_settings.key_quickzap_up,
		&g_settings.key_quickzap_down,
		&g_settings.key_subchannel_up,
		&g_settings.key_subchannel_down,
		&g_settings.key_zaphistory,
		&g_settings.key_lastchannel,
		&g_settings.key_pip,

		// mp
		&g_settings.mpkey_rewind,
		&g_settings.mpkey_forward,
		&g_settings.mpkey_pause,
		&g_settings.mpkey_stop,
		&g_settings.mpkey_play,
		&g_settings.mpkey_audio,
		&g_settings.mpkey_time,
		&g_settings.mpkey_bookmark,
		&g_settings.key_timeshift,
		
		// media
		&g_settings.key_recordsbrowser,
		&g_settings.key_audioplayer,
		&g_settings.key_pictureviewer,
		&g_settings.key_timerlist,
		&g_settings.key_inetradio,
		&g_settings.key_moviebrowser,
		&g_settings.key_filebrowser,
		&g_settings.key_webtv,
		
		// misc
		&g_settings.key_screenshot,
		
		// mb
		&g_settings.mb_copy_jump,
		&g_settings.mb_cut_jump,
		&g_settings.mb_truncate
	};

	CKeyChooser * keychooser[KEYBINDS_COUNT];

	for (int i = 0; i < KEYBINDS_COUNT; i++)
		keychooser[i] = new CKeyChooser(keyvalue_p[i], keydescription_head[i], NEUTRINO_ICON_SETTINGS);
	
	// keybinding menu
	CMenuWidget bindSettings(LOCALE_KEYBINDINGMENU_HEAD, NEUTRINO_ICON_KEYBINDING );
	
	// intros
	bindSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	bindSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	// save settings
	bindSettings.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));

	bindSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_KEYBINDINGMENU_MODECHANGE));
	
	// tv/radio mode
	bindSettings.addItem(new CMenuForwarder(keydescription[KEY_TV_RADIO_MODE], true, NULL, keychooser[KEY_TV_RADIO_MODE]));

	// channellist
	bindSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_KEYBINDINGMENU_CHANNELLIST));

	for (int i = KEY_PAGE_UP; i <= KEY_BOUQUET_DOWN; i++)
		bindSettings.addItem(new CMenuForwarder(keydescription[i], true, NULL, keychooser[i]));

	// quick zap
	bindSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_KEYBINDINGMENU_QUICKZAP));

	for (int i = KEY_CHANNEL_UP; i <= KEY_SAME_TP; i++)
		bindSettings.addItem(new CMenuForwarder(keydescription[i], true, NULL, keychooser[i]));

	// mp keys
	bindSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_MAINMENU_MOVIEPLAYER));
	for (int i = MPKEY_REWIND; i <= KEY_TIMESHIFT; i++)
		bindSettings.addItem(new CMenuForwarder(keydescription[i], true, NULL, keychooser[i]));
	
	// media
	bindSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_MAINMENU_MEDIAPLAYER));
	for (int i = KEY_EXTRAS_RECORDSBROWSER; i <= KEY_EXTRAS_WEBTV; i++)
		bindSettings.addItem(new CMenuForwarder(keydescription[i], true, NULL, keychooser[i]));
	
	// mb
	bindSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_MOVIEBROWSER_HEAD));
	for (int i = KEY_EXTRAS_MB_COPY_JUMP; i <= KEY_EXTRAS_MB_TRUNCATE; i++)
		bindSettings.addItem(new CMenuForwarder(keydescription[i], true, NULL, keychooser[i]));

	// misc
	bindSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_MAINSETTINGS_MISC));
	
	// screenshot key
	bindSettings.addItem(new CMenuForwarder(keydescription[KEY_EXTRAS_SCREENSHOT], true, NULL, keychooser[KEY_EXTRAS_SCREENSHOT]));
	
	// save keymap
	bindSettings.addItem(new CMenuForwarder(LOCALE_KEYBINDINGMENU_SAVEKEYMAP, true, NULL, this, "savekeymap" ) );
	
	bindSettings.exec(NULL, "");
	bindSettings.hide();
}

// recording settings
extern char recDir[255];			// defined in neutrino.cpp
extern char timeshiftDir[255];			// defined in neutrino.cpp
extern bool autoshift;				// defined in neutrino.cpp
extern int startAutoRecord(bool addTimer);	// defined in neutrino.cpp
extern void stopAutoRecord();			// defined in neutrino.cpp

CRecordingSettings::CRecordingSettings()
{
}

CRecordingSettings::~CRecordingSettings()
{
}

int CRecordingSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CRecordingSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	else if(actionKey == "recording")
	{
		CHintBox * hintBox = new CHintBox(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_MAINSETTINGS_SAVESETTINGSNOW_HINT)); // UTF-8
		hintBox->paint();
		
		CNeutrinoApp::getInstance()->setupRecordingDevice();
		
		hintBox->hide();
		delete hintBox;
		hintBox = NULL;
		
		return ret;
	}
	else if(actionKey == "recordingdir")
	{
		if(parent)
			parent->hide();
		
		CFileBrowser b;
		b.Dir_Mode = true;

		if (b.exec(g_settings.network_nfs_recordingdir)) 
		{
			const char * newdir = b.getSelectedFile()->Name.c_str();
			dprintf(DEBUG_NORMAL, "CRecordingSettings::exec: New recordingdir: selected %s\n", newdir);

			if(check_dir(newdir))
				printf("CRecordingSettings::exec: Wrong/unsupported recording dir %s\n", newdir);
			else
			{
				strncpy(g_settings.network_nfs_recordingdir, b.getSelectedFile()->Name.c_str(), sizeof(g_settings.network_nfs_recordingdir)-1 );
				
				dprintf(DEBUG_NORMAL, "CRecordingSettings::exec: New recordingdir: %s\n", g_settings.network_nfs_recordingdir);
				
				sprintf(timeshiftDir, "%s/.timeshift", g_settings.network_nfs_recordingdir);
					
				safe_mkdir(timeshiftDir);
				dprintf(DEBUG_NORMAL, "CRecordingSettings::exec: New timeshift dir: %s\n", timeshiftDir);
			}
		}
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

bool CRecordingSettings::changeNotify(const neutrino_locale_t OptionName, void */*data*/)
{
	dprintf(DEBUG_NORMAL, "CRecordingSettings::changeNotify:\n");
	
	if(ARE_LOCALES_EQUAL(OptionName, LOCALE_EXTRA_AUTO_TIMESHIFT)) 
	{	  
		if(g_settings.auto_timeshift)
			startAutoRecord(true);
		else
		{
			if(autoshift) 
			{
				stopAutoRecord();
				
				CNeutrinoApp::getInstance()->recordingstatus = 0;
				CNeutrinoApp::getInstance()->timeshiftstatus = 0;
			}
		}
	
		return true;
	}

	return false;
}

void CRecordingSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CRecordingSettings::showMenu:\n");
	
	CMenuWidget recordingSettings(LOCALE_RECORDINGMENU_HEAD, NEUTRINO_ICON_RECORDING );
	
	int rec_pre = 0;
	int rec_post = 0;
	
	g_Timerd->getRecordingSafety(rec_pre, rec_post);

	sprintf(g_settings.record_safety_time_before, "%02d", rec_pre/60);
	sprintf(g_settings.record_safety_time_after, "%02d", rec_post/60);

	CRecordingSafetyNotifier *RecordingSafetyNotifier = new CRecordingSafetyNotifier;

	//safety time befor
	CStringInput * timerBefore = new CStringInput(LOCALE_TIMERSETTINGS_RECORD_SAFETY_TIME_BEFORE, g_settings.record_safety_time_before, 2, LOCALE_TIMERSETTINGS_RECORD_SAFETY_TIME_BEFORE_HINT_1, LOCALE_TIMERSETTINGS_RECORD_SAFETY_TIME_BEFORE_HINT_2,"0123456789 ", RecordingSafetyNotifier);
	CMenuForwarder *fTimerBefore = new CMenuForwarder(LOCALE_TIMERSETTINGS_RECORD_SAFETY_TIME_BEFORE, true, g_settings.record_safety_time_before, timerBefore);

	//safety time after
	CStringInput * timerAfter = new CStringInput(LOCALE_TIMERSETTINGS_RECORD_SAFETY_TIME_AFTER, g_settings.record_safety_time_after, 2, LOCALE_TIMERSETTINGS_RECORD_SAFETY_TIME_AFTER_HINT_1, LOCALE_TIMERSETTINGS_RECORD_SAFETY_TIME_AFTER_HINT_2,"0123456789 ", RecordingSafetyNotifier);
	CMenuForwarder *fTimerAfter = new CMenuForwarder(LOCALE_TIMERSETTINGS_RECORD_SAFETY_TIME_AFTER, true, g_settings.record_safety_time_after, timerAfter);

	//audiopids
	g_settings.recording_audio_pids_std = ( g_settings.recording_audio_pids_default & TIMERD_APIDS_STD ) ? 1 : 0 ;
	g_settings.recording_audio_pids_alt = ( g_settings.recording_audio_pids_default & TIMERD_APIDS_ALT ) ? 1 : 0 ;
	g_settings.recording_audio_pids_ac3 = ( g_settings.recording_audio_pids_default & TIMERD_APIDS_AC3 ) ? 1 : 0 ;
	
	CRecAPIDSettingsNotifier * an = new CRecAPIDSettingsNotifier;

	//default
	CMenuOptionChooser* aoj1 = new CMenuOptionChooser(LOCALE_RECORDINGMENU_APIDS_STD, &g_settings.recording_audio_pids_std, MESSAGEBOX_NO_YES_OPTIONS, MESSAGEBOX_NO_YES_OPTION_COUNT, true, an);

	//alt
	CMenuOptionChooser* aoj2 = new CMenuOptionChooser(LOCALE_RECORDINGMENU_APIDS_ALT, &g_settings.recording_audio_pids_alt, MESSAGEBOX_NO_YES_OPTIONS, MESSAGEBOX_NO_YES_OPTION_COUNT, true, an);

	//ac3
	CMenuOptionChooser* aoj3 = new CMenuOptionChooser(LOCALE_RECORDINGMENU_APIDS_AC3, &g_settings.recording_audio_pids_ac3, MESSAGEBOX_NO_YES_OPTIONS, MESSAGEBOX_NO_YES_OPTION_COUNT, true, an);

	//epg in name format
	CMenuOptionChooser* oj11 = new CMenuOptionChooser(LOCALE_RECORDINGMENU_EPG_FOR_FILENAME, &g_settings.recording_epg_for_filename, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true);

	// save in channeldir
	CMenuOptionChooser* oj13 = new CMenuOptionChooser(LOCALE_RECORDINGMENU_SAVE_IN_CHANNELDIR, &g_settings.recording_save_in_channeldir, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true);

	//RecDir
	CMenuForwarder* fRecDir = new CMenuForwarder(LOCALE_RECORDINGMENU_DEFDIR, true, g_settings.network_nfs_recordingdir, this, "recordingdir");

	// intros
	recordingSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	recordingSettings.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	// save settings
	recordingSettings.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));
	recordingSettings.addItem(new CMenuForwarder(LOCALE_RECORDINGMENU_SETUPNOW, true, NULL, this, "recording", CRCInput::RC_green, NEUTRINO_ICON_BUTTON_GREEN));

	recordingSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_TIMERSETTINGS_SEPARATOR));
	recordingSettings.addItem(fTimerBefore);
	recordingSettings.addItem(fTimerAfter);

	//apids
	recordingSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_RECORDINGMENU_APIDS));
	recordingSettings.addItem(aoj1);
	recordingSettings.addItem(aoj2);
	recordingSettings.addItem(aoj3);

	//
	recordingSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE));
	//epg in name format
	recordingSettings.addItem(oj11);
	
	// save in channeldir
	recordingSettings.addItem(oj13);

	recordingSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE));

	//recdir
	recordingSettings.addItem(fRecDir);
	
	// timeshift
	recordingSettings.addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_EXTRA_TIMESHIFT));
	
	// record time
	recordingSettings.addItem(new CMenuOptionNumberChooser(LOCALE_EXTRA_RECORD_TIME, &g_settings.record_hours, true, 1, 24, NULL) );

	// timeshift
	if (recDir != NULL)
	{
		// permanent timeshift
		recordingSettings.addItem(new CMenuOptionChooser(LOCALE_EXTRA_AUTO_TIMESHIFT, &g_settings.auto_timeshift, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true, this));
	}
	
	recordingSettings.exec(NULL, "");
	recordingSettings.hide();
}

// misc settings
extern Zapit_config zapitCfg;			//defined in neutrino.cpp
void setZapitConfig(Zapit_config * Cfg);
void getZapitConfig(Zapit_config *Cfg);

// option off1 on0
#define OPTIONS_OFF1_ON0_OPTION_COUNT 2
const CMenuOptionChooser::keyval OPTIONS_OFF1_ON0_OPTIONS[OPTIONS_OFF1_ON0_OPTION_COUNT] =
{
        { 1, LOCALE_OPTIONS_OFF, NULL },
        { 0, LOCALE_OPTIONS_ON, NULL }
};

#define MISCSETTINGS_FILESYSTEM_IS_UTF8_OPTION_COUNT 2
const CMenuOptionChooser::keyval MISCSETTINGS_FILESYSTEM_IS_UTF8_OPTIONS[MISCSETTINGS_FILESYSTEM_IS_UTF8_OPTION_COUNT] =
{
	{ 0, LOCALE_FILESYSTEM_IS_UTF8_OPTION_ISO8859_1, NULL },
	{ 1, LOCALE_FILESYSTEM_IS_UTF8_OPTION_UTF8, NULL      }
};

#define INFOBAR_SUBCHAN_DISP_POS_OPTIONS_COUNT 4
const CMenuOptionChooser::keyval  INFOBAR_SUBCHAN_DISP_POS_OPTIONS[INFOBAR_SUBCHAN_DISP_POS_OPTIONS_COUNT]=
{
	{ 0 , LOCALE_SETTINGS_POS_TOP_RIGHT, NULL },
	{ 1 , LOCALE_SETTINGS_POS_TOP_LEFT, NULL },
	{ 2 , LOCALE_SETTINGS_POS_BOTTOM_LEFT, NULL },
	{ 3 , LOCALE_SETTINGS_POS_BOTTOM_RIGHT, NULL }
};

#define SECTIONSD_SCAN_OPTIONS_COUNT 2
const CMenuOptionChooser::keyval SECTIONSD_SCAN_OPTIONS[SECTIONSD_SCAN_OPTIONS_COUNT] =
{
	{ 0, LOCALE_OPTIONS_OFF, NULL },
	{ 1, LOCALE_OPTIONS_ON, NULL }
};

// volumebar position
#define VOLUMEBAR_DISP_POS_OPTIONS_COUNT 6
const CMenuOptionChooser::keyval  VOLUMEBAR_DISP_POS_OPTIONS[VOLUMEBAR_DISP_POS_OPTIONS_COUNT]=
{
	{ 0 , LOCALE_SETTINGS_POS_TOP_RIGHT, NULL },
	{ 1 , LOCALE_SETTINGS_POS_TOP_LEFT, NULL },
	{ 2 , LOCALE_SETTINGS_POS_BOTTOM_LEFT, NULL },
	{ 3 , LOCALE_SETTINGS_POS_BOTTOM_RIGHT, NULL },
	{ 4 , LOCALE_SETTINGS_POS_DEFAULT_CENTER, NULL },
	{ 5 , LOCALE_SETTINGS_POS_HIGHER_CENTER, NULL }
};

#define MENU_CORNERSETTINGS_TYPE_OPTION_COUNT 2
const CMenuOptionChooser::keyval MENU_CORNERSETTINGS_TYPE_OPTIONS[MENU_CORNERSETTINGS_TYPE_OPTION_COUNT] =
{
	{ 0, LOCALE_EXTRA_ROUNDED_CORNERS_OFF, NULL },
	{ 1, LOCALE_EXTRA_ROUNDED_CORNERS_ON, NULL }	
};

#define MAINMENU_DESIGN_OPTION_COUNT 2
const CMenuOptionChooser::keyval MAINMENU_DESIGN_OPTIONS[MAINMENU_DESIGN_OPTION_COUNT] =
{
	{ 0, LOCALE_EXTRA_MAINMENU_DESIGN_STANDARD, NULL },
	{ 1, LOCALE_EXTRA_MAINMENU_DESIGN_SMART, NULL }	
};

CMenuOptionStringChooser * tzSelect;

CMiscSettings::CMiscSettings()
{
}

CMiscSettings::~CMiscSettings()
{
}

int CMiscSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CMiscSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	else if(actionKey == "logos_dir") 
	{
		if(parent)
			parent->hide();
		
		CFileBrowser b;
		b.Dir_Mode = true;
		
		if (b.exec(g_settings.logos_dir.c_str())) 
		{
			g_settings.logos_dir = b.getSelectedFile()->Name;

			dprintf(DEBUG_NORMAL, "CMiscSettings::exec: new logos dir %s\n", b.getSelectedFile()->Name.c_str());
		}

		return ret;
	}
	else if(actionKey == "epgdir") 
	{
		if(parent)
			parent->hide();
		
		CFileBrowser b;
		b.Dir_Mode = true;
		
		if ( b.exec(g_settings.epg_dir.c_str())) 
		{
			const char * newdir = b.getSelectedFile()->Name.c_str();
			if(check_dir(newdir))
				printf("CNeutrinoApp::exec: Wrong/unsupported epg dir %s\n", newdir);
			else
			{
				g_settings.epg_dir = b.getSelectedFile()->Name;
				CNeutrinoApp::getInstance()->SendSectionsdConfig();
			}
		}

		return ret;
	}
	
	showMenu();
	
	return ret;
}

void CMiscSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CMiscSettings::showMenu:\n");
	
	int shortcutMiscSettings = 1;
	
	CMenuWidget miscSettings(LOCALE_MISCSETTINGS_HEAD, NEUTRINO_ICON_SETTINGS);
	
	//miscSettings general
	miscSettings.addItem(new CMenuForwarderExtended(LOCALE_MISCSETTINGS_GENERAL, true, new CGeneralSettings(), NULL, CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED, NEUTRINO_ICON_MENUITEM_MAINSETTINGS, LOCALE_HELPTEXT_MISCSETTINGSGENERAL ));
	
	//channellist settings
	miscSettings.addItem(new CMenuForwarderExtended(LOCALE_MISCSETTINGS_CHANNELLIST, true, new CChannelListSettings(), NULL, CRCInput::RC_green, NEUTRINO_ICON_BUTTON_GREEN, NEUTRINO_ICON_MENUITEM_MAINSETTINGS, LOCALE_HELPTEXT_MISCSETTINGSCHANNELLIST ));

	// epg settings
	miscSettings.addItem(new CMenuForwarderExtended(LOCALE_MISCSETTINGS_EPG_HEAD, true, new CEPGSettings(), NULL, CRCInput::RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW, NEUTRINO_ICON_MENUITEM_MAINSETTINGS, LOCALE_HELPTEXT_MISCSETTINGSEPG ));

	// filebrowser settings
	miscSettings.addItem(new CMenuForwarderExtended(LOCALE_MISCSETTINGS_FILEBROWSER, true, new CFileBrowserSettings(), NULL, CRCInput::RC_blue, NEUTRINO_ICON_BUTTON_BLUE, NEUTRINO_ICON_MENUITEM_MAINSETTINGS, LOCALE_HELPTEXT_MISCSETTINGSFILEBROWSER ));
	
	// zapit setup (start channel)
	miscSettings.addItem(new CMenuForwarderExtended(LOCALE_MISCSETTINGS_ZAPIT, true, new CZapitSetup(), NULL, CRCInput::convertDigitToKey(shortcutMiscSettings++), NULL, NEUTRINO_ICON_MENUITEM_MAINSETTINGS, LOCALE_HELPTEXT_MISCSETTINGSZAPITSETUP ));
	
	// psi setup	
	CPSISetup * chPSISetup = new CPSISetup(LOCALE_VIDEOMENU_PSISETUP, &g_settings.contrast, &g_settings.saturation, &g_settings.brightness, &g_settings.tint);
	miscSettings.addItem( new CMenuForwarderExtended(LOCALE_VIDEOMENU_PSISETUP, true, chPSISetup, NULL, CRCInput::convertDigitToKey(shortcutMiscSettings++), NULL, NEUTRINO_ICON_MENUITEM_MAINSETTINGS, LOCALE_HELPTEXT_MISCSETTINGSPSISETUP ));
	
	miscSettings.exec(NULL, "");
	miscSettings.hide();
}

// general settings
extern CRemoteControl * g_RemoteControl;	// defined neutrino.cpp

CGeneralSettings::CGeneralSettings()
{
}

CGeneralSettings::~CGeneralSettings()
{
}

int CGeneralSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CGeneralSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	else if(actionKey == "logos_dir") 
	{
		if(parent)
			parent->hide();
		
		CFileBrowser b;
		b.Dir_Mode = true;
		
		if (b.exec(g_settings.logos_dir.c_str())) 
		{
			g_settings.logos_dir = b.getSelectedFile()->Name;

			dprintf(DEBUG_NORMAL, "CMiscSettings::exec: new logos dir %s\n", b.getSelectedFile()->Name.c_str());
		}

		return ret;
	}
	
	showMenu();
	
	return ret;
}

bool CGeneralSettings::changeNotify(const neutrino_locale_t OptionName, void */*data*/)
{
	dprintf(DEBUG_NORMAL, "CGeneralSettings::changeNotify:\n");
	
	if(ARE_LOCALES_EQUAL(OptionName, LOCALE_MISCSETTINGS_INFOBAR_RADIOTEXT)) 
	{
		bool usedBackground = CFrameBuffer::getInstance()->getuseBackground();
		
		if (g_settings.radiotext_enable) 
		{
			// hide radiomode background pic
			if (usedBackground) 
			{
				CFrameBuffer::getInstance()->saveBackgroundImage();
				CFrameBuffer::getInstance()->ClearFrameBuffer();

				CFrameBuffer::getInstance()->blit();
			}
			
			//
			if (g_Radiotext == NULL)
				g_Radiotext = new CRadioText;
			if (g_Radiotext && ((CNeutrinoApp::getInstance()->getMode()) == NeutrinoMessages::mode_radio))
				g_Radiotext->setPid(g_RemoteControl->current_PIDs.APIDs[g_RemoteControl->current_PIDs.PIDs.selected_apid].pid);
		} 
		else 
		{
			// Restore previous background
			if (usedBackground) 
			{
				CFrameBuffer::getInstance()->restoreBackgroundImage();
				CFrameBuffer::getInstance()->useBackground(true);
				CFrameBuffer::getInstance()->paintBackground();
				CFrameBuffer::getInstance()->blit();
			}
			
			if (g_Radiotext)
				g_Radiotext->radiotext_stop();
			delete g_Radiotext;
			g_Radiotext = NULL;
			
			CFrameBuffer::getInstance()->loadBackgroundPic("radiomode.jpg");
			CFrameBuffer::getInstance()->blit();
		}
		
		return true;
	}

	return false;
}

void CGeneralSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CGeneralSettings::showMenu:\n");
	
	CMenuWidget miscSettingsGeneral(LOCALE_MISCSETTINGS_GENERAL, NEUTRINO_ICON_SETTINGS);
	
	// intros
	miscSettingsGeneral.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	miscSettingsGeneral.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	// save settings
	miscSettingsGeneral.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));
	miscSettingsGeneral.addItem( new CMenuSeparator(CMenuSeparator::LINE) );

	// rc delay
	CMenuOptionChooser * m1 = new CMenuOptionChooser(LOCALE_MISCSETTINGS_SHUTDOWN_REAL_RCDELAY, &g_settings.shutdown_real_rcdelay, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, !g_settings.shutdown_real);

	CMiscNotifier * miscNotifier = new CMiscNotifier( m1 );

	// shutdown real
	miscSettingsGeneral.addItem(new CMenuOptionChooser(LOCALE_MISCSETTINGS_SHUTDOWN_REAL, &g_settings.shutdown_real, OPTIONS_OFF1_ON0_OPTIONS, OPTIONS_OFF1_ON0_OPTION_COUNT, true, miscNotifier ));

	// delayed shutdown
	miscSettingsGeneral.addItem(m1);

	// delay counter
	CStringInput * miscSettings_shutdown_count = new CStringInput(LOCALE_MISCSETTINGS_SHUTDOWN_COUNT, g_settings.shutdown_count, 3, LOCALE_MISCSETTINGS_SHUTDOWN_COUNT_HINT1, LOCALE_MISCSETTINGS_SHUTDOWN_COUNT_HINT2, "0123456789 ");
	miscSettingsGeneral.addItem(new CMenuForwarder(LOCALE_MISCSETTINGS_SHUTDOWN_COUNT, true, g_settings.shutdown_count, miscSettings_shutdown_count));

	// start to standby
	miscSettingsGeneral.addItem(new CMenuOptionChooser(LOCALE_EXTRA_STARTSTANDBY, &g_settings.power_standby, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true));

	// sig/snr
	miscSettingsGeneral.addItem(new CMenuOptionChooser(LOCALE_MISCSETTINGS_INFOBAR_SAT_DISPLAY, &g_settings.infobar_sat_display, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true));
	
	// radio text	
	miscSettingsGeneral.addItem(new CMenuOptionChooser(LOCALE_MISCSETTINGS_INFOBAR_RADIOTEXT, &g_settings.radiotext_enable, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true, this ));
	
	// logos dir
	miscSettingsGeneral.addItem( new CMenuForwarder(LOCALE_MISCSETTINGS_LOGOSDIR, true, g_settings.logos_dir, this, "logos_dir" ) );
	
	// epgplus logos
	miscSettingsGeneral.addItem(new CMenuOptionChooser(LOCALE_MISCSETTINGS_EPGPLUS_SHOW_LOGOS, &g_settings.epgplus_show_logo, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true ));
	
	// infobar show channelname
	miscSettingsGeneral.addItem(new CMenuOptionChooser(LOCALE_MISCSETTINGS_INFOBAR_SHOW_CHANNELNAME, &g_settings.show_channelname, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true ));
	
	// recording screenshot
	miscSettingsGeneral.addItem(new CMenuOptionChooser(LOCALE_MISCSETTINGS_RECORDING_SCREENSHOT, &g_settings.recording_screenshot, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true ));

	// subchan pos
	miscSettingsGeneral.addItem(new CMenuOptionChooser(LOCALE_INFOVIEWER_SUBCHAN_DISP_POS, &g_settings.infobar_subchan_disp_pos, INFOBAR_SUBCHAN_DISP_POS_OPTIONS, INFOBAR_SUBCHAN_DISP_POS_OPTIONS_COUNT, true, NULL, CRCInput::RC_nokey, "", true));
	
	// volumebar position
	miscSettingsGeneral.addItem(new CMenuOptionChooser(LOCALE_EXTRA_VOLUME_POS, &g_settings.volume_pos, VOLUMEBAR_DISP_POS_OPTIONS, VOLUMEBAR_DISP_POS_OPTIONS_COUNT, true, NULL, CRCInput::RC_nokey, "", true ));
	
	// corners
	miscSettingsGeneral.addItem(new CMenuOptionChooser(LOCALE_EXTRA_ROUNDED_CORNERS, &g_settings.rounded_corners, MENU_CORNERSETTINGS_TYPE_OPTIONS, MENU_CORNERSETTINGS_TYPE_OPTION_COUNT, true));
	
	// mainmenu design
	miscSettingsGeneral.addItem(new CMenuOptionChooser(LOCALE_EXTRA_MAINMENU_DESIGN, &g_settings.mainmenu_design, MAINMENU_DESIGN_OPTIONS, MAINMENU_DESIGN_OPTION_COUNT, true));
	
	// volume bar steps
	CStringInput * audio_step = new CStringInput(LOCALE_AUDIOMENU_VOLUMEBAR_AUDIOSTEPS,g_settings.audio_step, 2, NONEXISTANT_LOCALE, NONEXISTANT_LOCALE, "0123456789 " );
	CMenuForwarder *as = new CMenuForwarder(LOCALE_AUDIOMENU_VOLUMEBAR_AUDIOSTEPS, true, g_settings.audio_step, audio_step );
	miscSettingsGeneral.addItem(as);

	// timezone
	xmlDocPtr parser;

	parser = parseXmlFile("/etc/timezone.xml");
	if (parser != NULL) 
	{	
		tzSelect = new CMenuOptionStringChooser(LOCALE_MAINSETTINGS_TIMEZONE, g_settings.timezone, true, new CTZChangeNotifier(), CRCInput::RC_nokey, "", true);

		xmlNodePtr search = xmlDocGetRootElement(parser)->xmlChildrenNode;
		bool found = false;

		while (search) 
		{
			if (!strcmp(xmlGetName(search), "zone")) 
			{
				std::string name = xmlGetAttribute(search, (char *) "name");
				std::string zone = xmlGetAttribute(search, (char *) "zone");
				
				//printf("Timezone: %s -> %s\n", name.c_str(), zone.c_str());
				
				tzSelect->addOption(name.c_str());
				found = true;
			}
			search = search->xmlNextNode;
		}

		if(found)
			miscSettingsGeneral.addItem(tzSelect);
		else 
		{
			delete tzSelect;
			tzSelect = NULL;
		}	
		xmlFreeDoc(parser);
	}
	
	// reset factory setup
	miscSettingsGeneral.addItem(new CMenuSeparator(CMenuSeparator::LINE));
	
	CDataResetNotifier * resetNotifier = new CDataResetNotifier();
	miscSettingsGeneral.addItem(new CMenuForwarder(LOCALE_MISCSETTINGS_RESET, true, NULL, resetNotifier, "factory", CRCInput::RC_green, NEUTRINO_ICON_BUTTON_GREEN ));
	miscSettingsGeneral.addItem(new CMenuForwarder(LOCALE_SETTINGS_BACKUP,  true, NULL, resetNotifier, "backup", CRCInput::RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW ));
	miscSettingsGeneral.addItem(new CMenuForwarder(LOCALE_SETTINGS_RESTORE, true, NULL, resetNotifier, "restore", CRCInput::RC_blue, NEUTRINO_ICON_BUTTON_BLUE ));
	
	miscSettingsGeneral.exec(NULL, "");
	miscSettingsGeneral.hide();
}

// channellist settings
extern t_channel_id live_channel_id;

CChannelListSettings::CChannelListSettings()
{
}

CChannelListSettings::~CChannelListSettings()
{
}

int CChannelListSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CChannelListSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

bool CChannelListSettings::changeNotify(const neutrino_locale_t OptionName, void */*data*/)
{
	dprintf(DEBUG_NORMAL, "CChannelListSettings::changeNotify:\n");
	
	if(ARE_LOCALES_EQUAL(OptionName, LOCALE_CHANNELLIST_MAKE_HDLIST)) 
	{
		CNeutrinoApp::getInstance()->channelsInit();
		CNeutrinoApp::getInstance()->channelList->adjustToChannelID(live_channel_id);//FIXME
		
		return true;
	}
	else if(ARE_LOCALES_EQUAL(OptionName, LOCALE_EXTRA_ZAPIT_MAKE_BOUQUET)) 
	{
		setZapitConfig(&zapitCfg);
		
		CNeutrinoApp::getInstance()->channelsInit();
		CNeutrinoApp::getInstance()->channelList->adjustToChannelID(live_channel_id);//FIXME
		
		return true;
	}
	
	return false;
}

void CChannelListSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CChannelListSettings::showMenu:\n");
	
	CMenuWidget miscSettingsChannelList(LOCALE_MISCSETTINGS_CHANNELLIST, NEUTRINO_ICON_SETTINGS);
	
	int shortcutMiscChannel = 1;
	
	// intros
	miscSettingsChannelList.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	miscSettingsChannelList.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	// save settings
	miscSettingsChannelList.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));
	miscSettingsChannelList.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	// HD list
	miscSettingsChannelList.addItem(new CMenuOptionChooser(LOCALE_CHANNELLIST_MAKE_HDLIST, &g_settings.make_hd_list, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true, this, CRCInput::convertDigitToKey(shortcutMiscChannel++) ));
	
	// virtual zap
	miscSettingsChannelList.addItem(new CMenuOptionChooser(LOCALE_MISCSETTINGS_VIRTUAL_ZAP_MODE, &g_settings.virtual_zap_mode, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcutMiscChannel++) ));
	
	// zap cycle
	miscSettingsChannelList.addItem(new CMenuOptionChooser(LOCALE_EXTRA_ZAP_CYCLE, &g_settings.zap_cycle, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcutMiscChannel++) ));
	
	// sms channel
	//miscSettingsChannelList.addItem(new CMenuOptionChooser(LOCALE_EXTRA_SMS_CHANNEL, &g_settings.sms_channel, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, false, NULL, CRCInput::convertDigitToKey(shortcutMiscChannel++) ));
	
	// channellist ca
	miscSettingsChannelList.addItem(new CMenuOptionChooser(LOCALE_CHANNELLIST_SHOWCA, &g_settings.channellist_ca, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcutMiscChannel++) ));
	
	// extended channel list
	miscSettingsChannelList.addItem(new CMenuOptionChooser(LOCALE_CHANNELLIST_EXTENDED, &g_settings.channellist_extended, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcutMiscChannel++) ));
	
	//
	getZapitConfig(&zapitCfg);
	
	miscSettingsChannelList.addItem(new CMenuOptionChooser(LOCALE_EXTRA_ZAPIT_MAKE_BOUQUET, (int *)&zapitCfg.makeRemainingChannelsBouquet, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcutMiscChannel++) ));
	miscSettingsChannelList.addItem( new CMenuOptionChooser(LOCALE_ZAPIT_SCANSDT, (int *)&zapitCfg.scanSDT, SECTIONSD_SCAN_OPTIONS, SECTIONSD_SCAN_OPTIONS_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcutMiscChannel++)) );
	
	miscSettingsChannelList.exec(NULL, "");
	miscSettingsChannelList.hide();
}

// epg settings
CEPGSettings::CEPGSettings()
{
}

CEPGSettings::~CEPGSettings()
{
}

int CEPGSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CEPGSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	else if(actionKey == "epgdir") 
	{
		if(parent)
			parent->hide();
		
		CFileBrowser b;
		b.Dir_Mode = true;
		
		if ( b.exec(g_settings.epg_dir.c_str())) 
		{
			const char * newdir = b.getSelectedFile()->Name.c_str();
			if(check_dir(newdir))
				printf("CNeutrinoApp::exec: Wrong/unsupported epg dir %s\n", newdir);
			else
			{
				g_settings.epg_dir = b.getSelectedFile()->Name;
				CNeutrinoApp::getInstance()->SendSectionsdConfig();
			}
		}

		return ret;
	}
	
	showMenu();
	
	return ret;
}

void CEPGSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CEPGSettings::showMenu:\n");
	
	CMenuWidget miscSettingsEPG(LOCALE_MISCSETTINGS_EPG_HEAD, NEUTRINO_ICON_SETTINGS);
	
	int shortcutMiscEpg = 1;
	
	// intros
	miscSettingsEPG.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	miscSettingsEPG.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	// save settings
	miscSettingsEPG.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));
	miscSettingsEPG.addItem( new CMenuSeparator(CMenuSeparator::LINE) );

	// save epg
	CSectionsdConfigNotifier* sectionsdConfigNotifier = new CSectionsdConfigNotifier;
	miscSettingsEPG.addItem(new CMenuOptionChooser(LOCALE_MISCSETTINGS_EPG_SAVE, &g_settings.epg_save, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcutMiscEpg++) ));

	// epg cache
        CStringInput * miscSettings_epg_cache = new CStringInput(LOCALE_MISCSETTINGS_EPG_CACHE, &g_settings.epg_cache, 2,LOCALE_MISCSETTINGS_EPG_CACHE_HINT1, LOCALE_MISCSETTINGS_EPG_CACHE_HINT2 , "0123456789 ", sectionsdConfigNotifier);
        miscSettingsEPG.addItem(new CMenuForwarder(LOCALE_MISCSETTINGS_EPG_CACHE, true, g_settings.epg_cache, miscSettings_epg_cache, NULL, CRCInput::convertDigitToKey(shortcutMiscEpg++) ));

	// extended epg cache
        CStringInput * miscSettings_epg_cache_e = new CStringInput(LOCALE_MISCSETTINGS_EPG_EXTENDEDCACHE, &g_settings.epg_extendedcache, 3,LOCALE_MISCSETTINGS_EPG_EXTENDEDCACHE_HINT1, LOCALE_MISCSETTINGS_EPG_EXTENDEDCACHE_HINT2 , "0123456789 ", sectionsdConfigNotifier);
        miscSettingsEPG.addItem(new CMenuForwarder(LOCALE_MISCSETTINGS_EPG_EXTENDEDCACHE, true, g_settings.epg_extendedcache, miscSettings_epg_cache_e, NULL, CRCInput::convertDigitToKey(shortcutMiscEpg++)));

	// old events
        CStringInput * miscSettings_epg_old_events = new CStringInput(LOCALE_MISCSETTINGS_EPG_OLD_EVENTS, &g_settings.epg_old_events, 2,LOCALE_MISCSETTINGS_EPG_OLD_EVENTS_HINT1, LOCALE_MISCSETTINGS_EPG_OLD_EVENTS_HINT2 , "0123456789 ", sectionsdConfigNotifier);
        miscSettingsEPG.addItem(new CMenuForwarder(LOCALE_MISCSETTINGS_EPG_OLD_EVENTS, true, g_settings.epg_old_events, miscSettings_epg_old_events, NULL, CRCInput::convertDigitToKey(shortcutMiscEpg++) ));

	// max epg events
        CStringInput * miscSettings_epg_max_events = new CStringInput(LOCALE_MISCSETTINGS_EPG_MAX_EVENTS, &g_settings.epg_max_events, 5,LOCALE_MISCSETTINGS_EPG_MAX_EVENTS_HINT1, LOCALE_MISCSETTINGS_EPG_MAX_EVENTS_HINT2 , "0123456789 ", sectionsdConfigNotifier);
        miscSettingsEPG.addItem(new CMenuForwarder(LOCALE_MISCSETTINGS_EPG_MAX_EVENTS, true, g_settings.epg_max_events, miscSettings_epg_max_events, NULL, CRCInput::convertDigitToKey(shortcutMiscEpg++) ));

	// epg save dir
        miscSettingsEPG.addItem(new CMenuForwarder(LOCALE_MISCSETTINGS_EPG_DIR, true, g_settings.epg_dir, this, "epgdir", CRCInput::convertDigitToKey(shortcutMiscEpg++) ));
	
	// epglang
	miscSettingsEPG.addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_MISCSETTINGS_PREF_EPGS_HEAD));
	
	CMenuOptionStringChooser * epglangSelect[3];
	CEPGlangSelectNotifier * EPGlangNotifier = new CEPGlangSelectNotifier();
	
	for(int i = 0; i < 3; i++) 
	{
		epglangSelect[i] = new CMenuOptionStringChooser(LOCALE_MISCSETTINGS_PREF_EPGS, g_settings.pref_epgs[i], true, EPGlangNotifier, CRCInput::RC_nokey, "", true);
		std::map<std::string, std::string>::const_iterator it;
		
		epglangSelect[i]->addOption("");
		for(it = iso639rev.begin(); it != iso639rev.end(); it++) 
			epglangSelect[i]->addOption(it->first.c_str());
	}
	
	// epglang
	for(int i = 0; i < 3; i++) 
		miscSettingsEPG.addItem(epglangSelect[i]);
	
	miscSettingsEPG.exec(NULL, "");
	miscSettingsEPG.hide();
}

// filebrowser settings
CFileBrowserSettings::CFileBrowserSettings()
{
}

CFileBrowserSettings::~CFileBrowserSettings()
{
}

int CFileBrowserSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CFileBrowserSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = menu_return::RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	showMenu();
	
	return ret;
}

void CFileBrowserSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CFileBrowserSettings::showMenu:\n");
	
	CMenuWidget miscSettingsFileBrowser(LOCALE_FILEBROWSER_HEAD, NEUTRINO_ICON_SETTINGS);
	
	int shortcutMiscFileBrowser = 1;
	
	// intros
	miscSettingsFileBrowser.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	miscSettingsFileBrowser.addItem( new CMenuSeparator(CMenuSeparator::LINE) );
	
	// save settings
	miscSettingsFileBrowser.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));
	miscSettingsFileBrowser.addItem( new CMenuSeparator(CMenuSeparator::LINE) );

	// UTF 
	miscSettingsFileBrowser.addItem(new CMenuOptionChooser(LOCALE_FILESYSTEM_IS_UTF8, &g_settings.filesystem_is_utf8, MISCSETTINGS_FILESYSTEM_IS_UTF8_OPTIONS, MISCSETTINGS_FILESYSTEM_IS_UTF8_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcutMiscFileBrowser++), "", true ));

	// show rights
	miscSettingsFileBrowser.addItem(new CMenuOptionChooser(LOCALE_FILEBROWSER_SHOWRIGHTS, &g_settings.filebrowser_showrights, MESSAGEBOX_NO_YES_OPTIONS, MESSAGEBOX_NO_YES_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcutMiscFileBrowser++) ));

	// deny dir
	miscSettingsFileBrowser.addItem(new CMenuOptionChooser(LOCALE_FILEBROWSER_DENYDIRECTORYLEAVE, &g_settings.filebrowser_denydirectoryleave, MESSAGEBOX_NO_YES_OPTIONS, MESSAGEBOX_NO_YES_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcutMiscFileBrowser++) ));
	
	miscSettingsFileBrowser.exec(NULL, "");
	miscSettingsFileBrowser.hide();
}

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
	mainSettings.addItem(new CMenuForwarderExtended(LOCALE_MAINSETTINGS_NETWORK, true, new CNetworkSettings(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL, NEUTRINO_ICON_MENUITEM_UPNPBROWSER, LOCALE_HELPTEXT_NETWORKSETTINGS ));

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

