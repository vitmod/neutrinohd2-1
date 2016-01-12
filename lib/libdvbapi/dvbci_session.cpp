/* DVB CI Transport Connection */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
//#ifdef __sh__
#include <stdio.h>
//#endif

#include "dvbci_session.h"
#include "dvbci_resmgr.h"
#include "dvbci_appmgr.h"
#include "dvbci_camgr.h"
#include "dvbci_datetimemgr.h"
#include "dvbci_mmi.h"

#include <neutrinoMessages.h>
#include <driver/rcinput.h>
#include <system/debug.h>

extern CRCInput *g_RCInput;



eDVBCISession* eDVBCISession::sessions[SLMS];

int eDVBCISession::buildLengthField(unsigned char *pkt, int len)
{
	dprintf(DEBUG_DEBUG, "%s >\n", __func__);

	if (len < 127)
	{
		*pkt++=len;
		return 1;
	} else if (len < 256)
	{
		*pkt++=0x81;
		*pkt++=len;
		return 2;
	} else if (len < 65535)
	{
		*pkt++=0x82;
		*pkt++=len>>8;
		*pkt++=len;
		return 3;
	} else
	{
		printf("too big length\n");
		exit(0);
	}
	dprintf(DEBUG_DEBUG, "%s <\n", __func__);
}

int eDVBCISession::parseLengthField(const unsigned char *pkt, int &len)
{
	dprintf(DEBUG_DEBUG, "%s >\n", __func__);

	len=0;
	if (!(*pkt&0x80)) 
	{
		len = *pkt;
		dprintf(DEBUG_DEBUG, "%s <\n", __func__);
		return 1;
	}
	for (int i=0; i<(pkt[0]&0x7F); ++i)
	{
		len <<= 8;
		len |= pkt[i + 1];
	}
	dprintf(DEBUG_DEBUG, "%s <\n", __func__);
	return (pkt[0] & 0x7F) + 1;
}

void eDVBCISession::sendAPDU(const unsigned char *tag, const void *data, int len)
{
	unsigned char pkt[len+3+4];
	int l;
	dprintf(DEBUG_DEBUG, "%s >\n", __func__);
	memcpy(pkt, tag, 3);
	l=buildLengthField(pkt+3, len);
	if (data)
		memcpy(pkt+3+l, data, len);
	sendSPDU(0x90, 0, 0, pkt, len+3+l);
	dprintf(DEBUG_DEBUG, "%s <\n", __func__);
}

void eDVBCISession::sendSPDU(unsigned char tag, const void *data, int len, const void *apdu, int alen)
{
	dprintf(DEBUG_DEBUG, "%s >\n", __func__);
	sendSPDU(slot, tag, data, len, session_nb, apdu, alen);
	dprintf(DEBUG_DEBUG, "%s <\n", __func__);
}

void eDVBCISession::sendSPDU(tSlot *slot, unsigned char tag, const void *data, int len, unsigned short session_nb, const void *apdu,int alen)
{
	unsigned char pkt[4096];
	unsigned char *ptr=pkt;
	dprintf(DEBUG_DEBUG, "%s >\n", __func__);

	*ptr++=tag;
	ptr+=buildLengthField(ptr, len+2);
	if (data)
		memcpy(ptr, data, len);
	ptr += len;
	*ptr++ = session_nb>>8;
	*ptr++ = session_nb;

	if (apdu)
		memcpy(ptr, apdu, alen);

	ptr += alen;
	//slot->send(pkt, ptr - pkt);
        sendData(slot, pkt, ptr - pkt);

	dprintf(DEBUG_DEBUG, "%s <\n", __func__);
}

void eDVBCISession::sendOpenSessionResponse(tSlot *slot, unsigned char session_status, const unsigned char *resource_identifier, unsigned short session_nb)
{
	char pkt[6];
	pkt[0]=session_status;
	
	dprintf(DEBUG_INFO, "sendOpenSessionResponse\n");
	
	memcpy(pkt + 1, resource_identifier, 4);
	sendSPDU(slot, 0x92, pkt, 5, session_nb);
	dprintf(DEBUG_DEBUG, "%s <\n", __func__);
}

void eDVBCISession::recvCreateSessionResponse(const unsigned char *data)
{
	dprintf(DEBUG_DEBUG, "%s >\n", __func__);
	status = data[0];
	state = stateStarted;
	action = 1;
	printf("create Session Response, status %x\n", status);
	dprintf(DEBUG_DEBUG, "%s <\n", __func__);
}

void eDVBCISession::recvCloseSessionRequest(const unsigned char *data)
{
	dprintf(DEBUG_DEBUG, "%s >\n", __func__);
	state = stateInDeletion;
	action = 1;
	printf("close Session Request\n");
	dprintf(DEBUG_DEBUG, "%s <\n", __func__);
}

void eDVBCISession::deleteSessions(const tSlot *slot)
{
	dprintf(DEBUG_DEBUG, "%s >\n", __func__);
	for (unsigned short session_nb=0; session_nb < SLMS; ++session_nb)
	{
		if (sessions[session_nb] && sessions[session_nb]->slot == slot)
			sessions[session_nb]=0;
	}
	dprintf(DEBUG_DEBUG, "%s <\n", __func__);
}

eDVBCISession* eDVBCISession::createSession(tSlot *slot, const unsigned char *resource_identifier, unsigned char &status)
{
	unsigned long tag;
	unsigned short session_nb;

	dprintf(DEBUG_DEBUG, "%s >\n", __func__);
	for (session_nb=1; session_nb < SLMS; ++session_nb)
		if (!sessions[session_nb-1])
			break;
	dprintf(DEBUG_NORMAL, "use session_nb = %d\n", session_nb);
	if (session_nb == SLMS)
	{
		status=0xF3;
	        dprintf(DEBUG_DEBUG, "%s <\n", __func__);
		return NULL;
	}

	tag = resource_identifier[0] << 24;
	tag|= resource_identifier[1] << 16;
	tag|= resource_identifier[2] << 8;
	tag|= resource_identifier[3];

	dprintf(DEBUG_NORMAL, "Tag: %08lx\n", tag);

	switch (tag)
	{
	case 0x00010041:
		sessions[session_nb - 1]=new eDVBCIResourceManagerSession;
		printf("RESOURCE MANAGER\n");
		break;
	case 0x00020041:
		sessions[session_nb - 1]=new eDVBCIApplicationManagerSession(slot);
		printf("APPLICATION MANAGER\n");
		break;
	case 0x00030041:
		sessions[session_nb - 1] = new eDVBCICAManagerSession(slot);
		printf("CA MANAGER\n");
		break;
	case 0x00240041:
		sessions[session_nb - 1]=new eDVBCIDateTimeSession(slot);
		printf("DATE-TIME\n");
		break;
	case 0x00400041:
		sessions[session_nb - 1] = new eDVBCIMMISession(slot);
		printf("MMI - create session\n");
		break;
	case 0x00100041:
//		session=new eDVBCIAuthSession;
		printf("AuthSession\n");
//		break;
	case 0x00200041:
	default:
		printf("unknown resource type %02x %02x %02x %02x\n", resource_identifier[0], resource_identifier[1], resource_identifier[2],resource_identifier[3]);
		sessions[session_nb - 1]=0;
		status=0xF0;
	}

	if (!sessions[session_nb - 1])
	{
		printf("unknown session.. expect crash\n");
		return NULL;
	}

	dprintf(DEBUG_INFO, "new session nb %d %p\n", session_nb, sessions[session_nb - 1]);
	sessions[session_nb - 1]->session_nb = session_nb;

	if (sessions[session_nb - 1])
	{
		sessions[session_nb - 1]->slot = slot;
		status = 0;
	}
	sessions[session_nb - 1]->state = stateInCreation;
	dprintf(DEBUG_DEBUG, "%s <\n", __func__);
        return sessions[session_nb - 1];   
}

void eDVBCISession::handleClose()
{
	unsigned char data[1]={0x00};
	dprintf(DEBUG_NORMAL, "%s >\n", __func__);
	sendSPDU(0x96, data, 1, 0, 0);

	dprintf(DEBUG_DEBUG, "%s <\n", __func__);
}

int eDVBCISession::pollAll()
{
	dprintf(DEBUG_INFO, "%s >\n", __func__);
	for (int session_nb=1; session_nb < SLMS; ++session_nb)
        {
		if (sessions[session_nb-1])
		{
			int r;

			if (sessions[session_nb-1]->state == stateInDeletion)
			{
				sessions[session_nb-1]->handleClose();
				sessions[session_nb-1]=0;
				r=1;
			} else
				r=sessions[session_nb-1]->poll();

			if (r)
			{
				dprintf(DEBUG_INFO, "%s <\n", __func__);
				return 1;
			}
		}
	}
	dprintf(DEBUG_DEBUG, "%s <\n", __func__);
	return 0;
}

void eDVBCISession::receiveData(tSlot *slot, const unsigned char *ptr, size_t len)
{
	const unsigned char *pkt = (const unsigned char*)ptr;
	unsigned char tag = *pkt++;
	int llen, hlen;
	dprintf(DEBUG_INFO, "%s >\n", __func__);

	dprintf(DEBUG_DEBUG, "slot: %p\n",slot);

	for(unsigned int i=0;i<len;i++)
		dprintf(DEBUG_INFO, "%02x ",ptr[i]);
	dprintf(DEBUG_INFO, "\n");
	
	llen = parseLengthField(pkt, hlen);
	pkt += llen;
	
	eDVBCISession* session = NULL;
	
	if(tag == 0x91)
	{
		unsigned char status;

		session = createSession(slot, pkt, status);
		sendOpenSessionResponse(slot, status, pkt, session ? session->session_nb : 0);
		
		if (session)
		{
			session->state=stateStarted;
			session->action=1;
		}
	}
	else
	{
		unsigned session_nb;
		dprintf(DEBUG_DEBUG, "hlen = %d, %d, %d\n", hlen,  pkt[hlen-2], pkt[hlen-1]);
		session_nb=pkt[hlen-2]<<8;
		session_nb|=pkt[hlen-1]&0xFF;
		
		if ((!session_nb) || (session_nb >= SLMS))
		{
			printf("PROTOCOL: illegal session number %x\n", session_nb);
			return;
		}
		
		session=sessions[session_nb-1];
		if (!session)
		{
			printf("PROTOCOL: data on closed session %x\n", session_nb);
			return;
		}

		switch (tag)
		{
		case 0x90:
			break;
		case 0x94:
			session->recvCreateSessionResponse(pkt);
			break;
		case 0x95:
			printf("recvCloseSessionRequest");
			session->recvCloseSessionRequest(pkt);
			break;
		default:
			printf("INTERNAL: nyi, tag %02x.\n", tag);
			return;
		}
	}

	hlen += llen + 1; // lengthfield and tag

	pkt = ((const unsigned char*)ptr) + hlen;
	len -= hlen;

	if (session)
	{
		dprintf(DEBUG_DEBUG, "len %d\n", len);
		while (len > 0)
		{
			int alen;
			const unsigned char *tag=pkt;
			pkt+=3; // tag
			len-=3;
			hlen=parseLengthField(pkt, alen);
			pkt+=hlen;
			len-=hlen;

			dprintf(DEBUG_DEBUG, "len = %d, hlen = %d, alen = %d\n", len, hlen, alen);

			//if (eDVBCIModule::getInstance()->workarounds_active & eDVBCIModule::workaroundMagicAPDULength)
			{
				if (((len-alen) > 0) && ((len - alen) < 3))
				{
					printf("WORKAROUND: applying work around MagicAPDULength\n");
					alen=len;
				}
			}
			dprintf(DEBUG_INFO, "1. Call receivedAPDU tag = 0x%2x, len = %d\n", (int) *tag, alen);
			if (session->receivedAPDU(tag, pkt, alen))
				session->action = 1;
			pkt+=alen;
			len-=alen;
		}
		
	}
	if (len)
		printf("PROTOCOL: warning, TL-Data has invalid length\n");
	dprintf(DEBUG_DEBUG, "%s <\n", __func__);
}

eDVBCISession::~eDVBCISession()
{
//	printf("destroy %p", this);
}

