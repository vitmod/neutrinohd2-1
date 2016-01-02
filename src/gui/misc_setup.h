/*
	Neutrino-GUI  -   DBoxII-Project

	$id: misc_setup.h 2016.01.02 21:53:28 mohousch $
	
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

#ifndef __misc_setup__
#define __misc_setup__

#include <gui/widget/menue.h>

#include <string>


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

#endif //__misc_setup__
