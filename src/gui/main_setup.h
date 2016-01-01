/*
	Neutrino-GUI  -   DBoxII-Project

	$id: main_setup.h 2015.12.22 21:25:28 mohousch $
	
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

#ifndef __main_setup__
#define __main_setup__

#include <gui/widget/menue.h>
#include <system/configure_network.h>
#include <system/setting_helpers.h>

#include <string>


// main settings
class CMainSetup : public CMenuTarget
{
	private:
		void showMenu();
		
	public:
		CMainSetup();
		~CMainSetup();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// video settings
class CVideoSettings : public CMenuTarget
{
	private:
		void showMenu();
	  
	public:
		CVideoSettings();
		~CVideoSettings();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// audio settings
class CAudioSettings : public CMenuTarget
{
	private:
		void showMenu();
		
	public:
		CAudioSettings();
		~CAudioSettings();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// parentallock settings
class CParentalLockSettings : public CMenuTarget
{
	private:
		void showMenu();
		
	public:
		CParentalLockSettings();
		~CParentalLockSettings();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// network settings
class CNetworkSettings : public CMenuTarget, CChangeObserver
{
	private:
		CIPChangeNotifier* MyIPChanger;
		
		void showMenu();
		
	public:
		CNetworkSettings();
		~CNetworkSettings();
		
		CNetworkConfig *networkConfig;
		CMenuItem * wlanEnable[3];
		
		int network_dhcp;
		int network_automatic_start;
		
		std::string network_hostname;
		std::string mac_addr;
		
		std::string network_ssid;
		std::string network_key;
		int network_encryption;
		
		static CNetworkSettings* getInstance();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// movieplayer settings
class CMoviePlayerSettings : public CMenuTarget
{
	private:
		void showMenu();
		
	public:
		CMoviePlayerSettings();
		~CMoviePlayerSettings();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// osd settings
class COSDSettings : public CMenuTarget
{
	private:
		void showMenu();
		
	public:
		COSDSettings();
		~COSDSettings();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// osd menucolor settings
class COSDMenuColorSettings : public CMenuTarget
{
	private:
		void showMenu();
		
	public:
		COSDMenuColorSettings();
		~COSDMenuColorSettings();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// osd infobarcolor settings
class COSDInfoBarColorSettings : public CMenuTarget
{
	private:
		void showMenu();
		
	public:
		COSDInfoBarColorSettings();
		~COSDInfoBarColorSettings();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// osd language settings
class CLanguageSettings : public CMenuTarget, CChangeObserver
{
	private:
		void showMenu();
		
	public:
		CLanguageSettings();
		~CLanguageSettings();
		
		bool changeNotify(const neutrino_locale_t OptionName, void *);
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// osd timing settings
class COSDTimingSettings : public CMenuTarget
{
	private:
		void showMenu();
		
	public:
		COSDTimingSettings();
		~COSDTimingSettings();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// audioplayer settings
class CAudioPlayerSettings : public CMenuTarget
{
	private:
		void showMenu();
		
	public:
		CAudioPlayerSettings();
		~CAudioPlayerSettings();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// pictureviewer settings
class CPictureViewerSettings : public CMenuTarget
{
	private:
		void showMenu();
		
	public:
		CPictureViewerSettings();
		~CPictureViewerSettings();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// lcd settings
class CLCDSettings : public CMenuTarget
{
	private:
		void showMenu();
		
	public:
		CLCDSettings();
		~CLCDSettings();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// remote control
class CRemoteControlSettings : public CMenuTarget, CChangeObserver
{
	private:
		void showMenu();
		
		CKeySetupNotifier       	*keySetupNotifier;
		
	public:
		CRemoteControlSettings();
		~CRemoteControlSettings();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// keys binding settings
class CKeysBindingSettings : public CMenuTarget
{
	private:
		void showMenu();
		
	public:
		CKeysBindingSettings();
		~CKeysBindingSettings();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// recording settings
class CRecordingSettings : public CMenuTarget, CChangeObserver
{
	private:
		void showMenu();
		
		bool changeNotify(const neutrino_locale_t OptionName, void *);
		
	public:
		CRecordingSettings();
		~CRecordingSettings();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// misc settings
class CMiscSettings : public CMenuTarget, CChangeObserver
{
	private:
		void showMenu();
		
	public:
		CMiscSettings();
		~CMiscSettings();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// general settings
class CGeneralSettings : public CMenuTarget, CChangeObserver
{
	private:
		void showMenu();
		
		bool changeNotify(const neutrino_locale_t OptionName, void *);
		
	public:
		CGeneralSettings();
		~CGeneralSettings();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// channellist settings
class CChannelListSettings : public CMenuTarget, CChangeObserver
{
	private:
		void showMenu();
		
		bool changeNotify(const neutrino_locale_t OptionName, void *);
		
	public:
		CChannelListSettings();
		~CChannelListSettings();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// epg settings
class CEPGSettings : public CMenuTarget, CChangeObserver
{
	private:
		void showMenu();
		
	public:
		CEPGSettings();
		~CEPGSettings();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

// epg settings
class CFileBrowserSettings : public CMenuTarget, CChangeObserver
{
	private:
		void showMenu();
		
	public:
		CFileBrowserSettings();
		~CFileBrowserSettings();
		
		int exec(CMenuTarget* parent, const std::string& actionKey);
};

#endif //__main_setup__
