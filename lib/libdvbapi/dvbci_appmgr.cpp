/* DVB CI Application Manager */
#include <stdio.h>
#include <string.h>
#include <system/debug.h>

#include "dvbci_appmgr.h"

eDVBCIApplicationManagerSession::eDVBCIApplicationManagerSession(tSlot *tslot)
{
	dprintf(DEBUG_DEBUG, "%s >\n", __func__);

	slot = tslot;

	slot->hasAppManager = true;
	slot->appSession = this;

	dprintf(DEBUG_DEBUG, "%s <\n", __func__);
}

eDVBCIApplicationManagerSession::~eDVBCIApplicationManagerSession()
{
	dprintf(DEBUG_DEBUG, "%s >\n", __func__);

	slot->hasAppManager = false;
	slot->appSession = NULL;

	dprintf(DEBUG_DEBUG, "%s <\n", __func__);
}

int eDVBCIApplicationManagerSession::receivedAPDU(const unsigned char *tag,const void *data, int len)
{
	dprintf(DEBUG_DEBUG, "eDVBCIApplicationManagerSession::%s >\n", __func__);

	printf("SESSION(%d)/APP %02x %02x %02x: ", session_nb, tag[0], tag[1], tag[2]);
	for (int i=0; i<len; i++)
		printf("%02x ", ((const unsigned char*)data)[i]);
	printf("\n");

	if ((tag[0]==0x9f) && (tag[1]==0x80))
	{
		switch (tag[2])
		{
		case 0x21:
		{
			int dl;
			printf("application info:\n");
			printf("  len: %d\n", len);
			printf("  application_type: %d\n", ((unsigned char*)data)[0]);
			printf("  application_manufacturer: %02x %02x\n", ((unsigned char*)data)[2], ((unsigned char*)data)[1]);
			printf("  manufacturer_code: %02x %02x\n", ((unsigned char*)data)[4],((unsigned char*)data)[3]);
			printf("  menu string: ");
			dl=((unsigned char*)data)[5];
			if ((dl + 6) > len)
			{
				printf("warning, invalid length (%d vs %d)\n", dl+6, len);
				dl=len-6;
			}
			char str[dl + 1];
			memcpy(str, ((char*)data) + 6, dl);
			str[dl] = '\0';
			for (int i = 0; i < dl; ++i)
				printf("%c", ((unsigned char*)data)[i+6]);
			printf("\n");

			strcpy(slot->name, str);
			printf("set name %s on slot %d, %p\n", slot->name, slot->slot, slot);
			break;
		}
		default:
			printf("unknown APDU tag 9F 80 %02x\n", tag[2]);
			break;
		}
	}
	dprintf(DEBUG_DEBUG, "%s <", __func__);
	return 0;
}

int eDVBCIApplicationManagerSession::doAction()
{
	dprintf(DEBUG_DEBUG, "%s >", __func__);
	switch (state)
	{
		case stateStarted:
		{
	    		const unsigned char tag[3]={0x9F, 0x80, 0x20}; // application manager info e    sendAPDU(tag);
			sendAPDU(tag);
			state=stateFinal;
			dprintf(DEBUG_DEBUG, "%s <", __func__);
			return 1;
		}
		case stateFinal:
			dprintf(DEBUG_DEBUG, "in final state.");
			wantmenu = 0;
			if (wantmenu)
			{
				printf("wantmenu: sending Tenter_menu\n");
				const unsigned char tag[3]={0x9F, 0x80, 0x22};  // Tenter_menu
				sendAPDU(tag);
				wantmenu=0;
				dprintf(DEBUG_DEBUG, "%s <\n", __func__);
				return 0;
			} else
				return 0;
		default:
			dprintf(DEBUG_DEBUG, "%s <\n", __func__);
			return 0;
	}
	dprintf(DEBUG_DEBUG, "%s <\n", __func__);
}

int eDVBCIApplicationManagerSession::startMMI()
{
	dprintf(DEBUG_DEBUG, "%s >\n", __func__);
	dprintf(DEBUG_INFO, "in appmanager -> startmmi()\n");
	const unsigned char tag[3]={0x9F, 0x80, 0x22};  // Tenter_menu
	sendAPDU(tag);

	slot->mmiOpened = true;

	//fixme slot->mmiOpened();
	dprintf(DEBUG_DEBUG, "%s <\n", __func__);
	return 0;
}

