/*
	$Id: dvb-ci.cpp,v 1.0 2013/08/18 11:23:30 mohousch Exp $
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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <asm/types.h>

#include <linux/dvb/ca.h>
#include <poll.h>
 
#include <list>
#include <queue>

#include <unistd.h>

#include "dvb-ci.h"
#include "dvbci_session.h"
#include "dvbci_appmgr.h"
#include "dvbci_camgr.h"

#include "dvbci_mmi.h"

#include <neutrinoMessages.h>
#include <driver/rcinput.h>

#include <connection/messagetools.h>   /* get_length_field_size */
#include <zapit/include/zapit/ci.h>

#include <config.h>
#include <system/debug.h>


static const char * FILENAME = "dvb-ci.cpp";

#define TAG_MENU_ANSWER                      0x9f880b
#define TAG_ENTER_MENU                       0x9f8022

extern CRCInput *g_RCInput;
extern int FrontendCount;

bool cDvbCi::checkQueueSize(tSlot* slot)
{
	return (slot->sendqueue.size() > 0);
}

void cDvbCi::CI_MenuAnswer(unsigned char bSlotIndex,unsigned char choice)
{
	dprintf(DEBUG_NORMAL, "%s:%s: %d %c\n", FILENAME, __FUNCTION__, bSlotIndex, choice);

	std::list<tSlot*>::iterator it;
	
        for(it = slot_data.begin(); it != slot_data.end(); ++it)
        {
		if ((*it)->slot == bSlotIndex) 
		{
			if ((*it)->hasMMIManager)
				(*it)->mmiSession->answerText((int) choice);
		}
	}

}

void cDvbCi::CI_Answer(unsigned char bSlotIndex,unsigned char *pBuffer,unsigned char nLength)
{
	printf("%s:%s: %d\n", FILENAME, __FUNCTION__, bSlotIndex);

	std::list<tSlot*>::iterator it;
	
	//FIXME: currently not tested
        for(it = slot_data.begin(); it != slot_data.end(); ++it)
        {
		if ((*it)->slot == bSlotIndex) 
		{
			if ((*it)->hasMMIManager)
				(*it)->mmiSession->answerEnq((char*) pBuffer, nLength);

			break;
		}
	}
}

void cDvbCi::CI_CloseMMI(unsigned char bSlotIndex)
{
	printf("%s:%s %d\n", FILENAME, __FUNCTION__, bSlotIndex);

	std::list<tSlot*>::iterator it;
	

        for(it = slot_data.begin(); it != slot_data.end(); ++it)
        {
		if ((*it)->slot == bSlotIndex) 
		{
			if ((*it)->hasMMIManager)
				(*it)->mmiSession->stopMMI();
			break;
		}
	}
}

void cDvbCi::CI_EnterMenu(unsigned char bSlotIndex)
{
	printf("%s:%s %d\n", FILENAME, __FUNCTION__, bSlotIndex);

	std::list<tSlot*>::iterator it;
	
        for(it = slot_data.begin(); it != slot_data.end(); ++it)
        {
#if 0
		if ((strstr((*it)->name, "unknown module") != NULL) && ((*it)->slot == bSlotIndex))
		{
			//the module has no real name, this is the matter if something while initializing went wrong
			//so let this take as a reset action for the module so we do not need to add a reset
			//feature to the neutrino menu
			reset(bSlotIndex);

			return;
		}
#endif
		if ((*it)->slot == bSlotIndex) 
		{
			if ((*it)->hasAppManager)
				(*it)->appSession->startMMI();

			break;
		}
	}
}


/* from dvb-apps
 */
int asn_1_decode(uint16_t * length, unsigned char * asn_1_array, uint32_t asn_1_array_len)
{
	uint8_t length_field;

	if (asn_1_array_len < 1)
		return -1;
	length_field = asn_1_array[0];

	if (length_field < 0x80) 
	{
		// there is only one word
		*length = length_field & 0x7f;
		return 1;
	} else if (length_field == 0x81) 
	{
		if (asn_1_array_len < 2)
			return -1;

		*length = asn_1_array[1];
		return 2;
	} else if (length_field == 0x82) 
	{
		if (asn_1_array_len < 3)
			return -1;

		*length = (asn_1_array[1] << 8) | asn_1_array[2];
		return 3;
	}

	return -1;
}

//wait for a while for some data und read it if some
eData waitData(int fd, unsigned char* buffer, int* len)
{
	int        retval;
	struct      pollfd fds;

	//printf("%s: %d\n", __func__, *len);
	
	fds.fd = fd;
	fds.events = POLLOUT | POLLPRI | POLLIN;
	
	retval = poll(&fds, 1, 100);

	if (retval < 0)
	{
		printf("data error\n");
		return eDataError;
	}
	else if (retval == 0)
	{
		return eDataTimeout;
	}
	else if (retval > 0)
	{
		if (fds.revents & POLLIN)
		{ 
			int n = read (fd, buffer, *len);
		      
			if (n > 0)
			{
				*len = n;
				return eDataReady;
			}
			*len = 0;
			return eDataError;
		} 
		else if (fds.revents & POLLOUT)
		{ 
			return eDataWrite;
		} 
		else if (fds.revents & POLLPRI)
		{ 
			return eDataStatusChanged;
		}
	}

	return eDataError;
}

static bool transmitData(tSlot* slot, unsigned char* d, int len)
{
#ifdef direct_write
	int res = write(slot->fd, d, len);

	free(d);
	if (res < 0 || res != len)
	{
		printf("error writing data to fd %d, slot %d: %m\n", slot->fd, slot->slot);
		return eDataError;
	}
#else
	dprintf(DEBUG_NORMAL, "SendData with data (len %d) >\n", len);
	for (int i = 0; i < len; i++)
		dprintf(DEBUG_NORMAL, "%02x ", d[i]);
	dprintf(DEBUG_NORMAL, "\n");

	slot->sendqueue.push(queueData(d, len));
#endif
	return true;
}

static bool sendDataLast(tSlot* slot)
{
	unsigned char data[5];
	slot->pollConnection = false;
	slot->DataLast = false;
	data[0] = slot->slot;
	data[1] = slot->connection_id;
	data[2] = T_DATA_LAST;
	data[3] = 1;
	data[4] = slot->connection_id;

	dprintf(DEBUG_DEBUG, "*** > Data Last: ");
	for (int i = 0; i < 5; i++)
		dprintf(DEBUG_DEBUG, "%02x ", data[i]);
	dprintf(DEBUG_DEBUG, "\n");

	write(slot->fd, data, 5);
	return true;
}

static bool sendRCV(tSlot* slot)
{
	unsigned char send_data[5];
	slot->pollConnection = false;
	slot->DataRCV = false;
	send_data[0] = slot->slot;
	send_data[1] = slot->connection_id;
	send_data[2] = T_RCV;
	send_data[3] = 1;
	send_data[4] = slot->connection_id;

	dprintf(DEBUG_INFO, "*** > T_RCV: ");
	for (int i = 0; i < 5; i++)
		dprintf(DEBUG_INFO, "%02x ", send_data[i]);
	dprintf(DEBUG_INFO, "\n");

	write(slot->fd, send_data, 5);
	return true;
}

//send some data on an fd, for a special slot and connection_id
eData sendData(tSlot* slot, unsigned char* data, int len)
{	
        dprintf(DEBUG_DEBUG, "%s: %p, %d\n", __func__, data, len);
       
	// only poll connection if we are not awaiting an answer
	slot->pollConnection = false;

	//send data_last and data
	if (len < 127) {
		unsigned char *d = (unsigned char*) malloc(len + 5);
		memcpy(d + 5, data, len);
		d[0] = slot->slot;
		d[1] = slot->connection_id;
		d[2] = T_DATA_LAST;
		d[3] = len + 1;
		d[4] = slot->connection_id;
		len += 5;
		transmitData(slot, d, len);
	}
	else if (len > 126 && len < 255) {
		unsigned char *d = (unsigned char*) malloc(len + 6);
		memcpy(d + 6, data, len);
		d[0] = slot->slot;
		d[1] = slot->connection_id;
		d[2] = T_DATA_LAST;
		d[3] = 0x81;
		d[4] = len + 1;
		d[5] = slot->connection_id;
		len += 6;
		transmitData(slot, d, len);
	}
	else if (len > 254) {
		unsigned char *d = (unsigned char*) malloc(len + 7);
		memcpy(d + 7, data, len);
		d[0] = slot->slot;
		d[1] = slot->connection_id;
		d[2] = T_DATA_LAST;
		d[3] = 0x82;
		d[4] = len >> 8;
		d[5] = len + 1;
		d[6] = slot->connection_id;
		len += 7;
		transmitData(slot, d, len);
	}

	return eDataReady;
}

//send a transport connection create request
bool sendCreateTC(tSlot* slot)
{
	unsigned char data[5];
	data[0] = slot->slot;
	data[1] = slot->slot + 1; 	/* conid */
	data[2] = T_CREATE_T_C;
	data[3] = 1;
	data[4] = slot->slot + 1 	/*conid*/;
	printf("Create TC: ");
	for (int i = 0; i < 5; i++)
		printf("%02x ", data[i]);
	printf("\n");
	write(slot->fd, data, 5);
	return true;
}

void cDvbCi::process_tpdu(tSlot* slot, unsigned char tpdu_tag, __u8* data, int asn_data_length, int con_id)
{
	switch (tpdu_tag) 
	{
		case T_C_T_C_REPLY:
			dprintf(DEBUG_INFO, "Got CTC Replay (slot %d, con %d)\n", slot->slot, slot->connection_id);
			/*answer with data last (and if we have with data)
			--> DataLast flag will be generated in next loop from received APDU*/
			break;
		case T_DELETE_T_C:
			//FIXME: close sessions etc; slot->reset ?
			//we must answer here with t_c_replay
			printf("Got \"Delete Transport Connection\" from module ->currently not handled!\n");
			break;
		case T_D_T_C_REPLY:
			printf("Got \"Delete Transport Connection Replay\" from module!\n");
			break;
		case T_REQUEST_T_C:
			printf("Got \"Request Transport Connection\" from Module ->currently not handled!\n");
			break;
		case T_DATA_MORE:
		{
			int new_data_length = slot->receivedLen + asn_data_length;
			printf("Got \"Data More\" from Module\n");
			__u8 *new_data_buffer = (__u8*)realloc(slot->receivedData, new_data_length);
			slot->receivedData = new_data_buffer;
			memcpy(slot->receivedData + slot->receivedLen, data, asn_data_length);
			slot->receivedLen = new_data_length;
			break;
		}
		case T_DATA_LAST:	
			/* single package */
			if (slot->receivedData == NULL) 
			{
				dprintf(DEBUG_INFO, "->single package\n");

				dprintf(DEBUG_INFO, "%s -> calling receiveData with data (len %d)\n", FILENAME, asn_data_length);
				for (int i = 0; i < asn_data_length; i++)
					dprintf(DEBUG_INFO, "%02x ", data[i]);
				dprintf(DEBUG_INFO, "\n");

				/* to avoid illegal session number: only if > 0 */
				if (asn_data_length)
				{
					eDVBCISession::receiveData(slot, data, asn_data_length);
					eDVBCISession::pollAll();
				}
			} 
			else 
			{
				/* chained package 
				?? DBO: I never have seen one */
				int new_data_length = slot->receivedLen + asn_data_length;
				printf("->chained data\n");
				__u8 *new_data_buffer = (__u8*) realloc(slot->receivedData, new_data_length);
				slot->receivedData = new_data_buffer;
				memcpy(slot->receivedData + slot->receivedLen, data, asn_data_length);
				slot->receivedLen = new_data_length;

				dprintf(DEBUG_INFO, "%s -> calling receiveData with data (len %d)\n", FILENAME, asn_data_length);
				for (int i = 0; i < slot->receivedLen; i++)
					dprintf(DEBUG_INFO, "%02x ", slot->receivedData[i]);
				dprintf(DEBUG_INFO, "\n");

				eDVBCISession::receiveData(slot, slot->receivedData, slot->receivedLen);
				eDVBCISession::pollAll();

				free(slot->receivedData);
				slot->receivedData = NULL;
				slot->receivedLen = 0;
			}
			break;
			
		case T_SB:
		{	
			if (data[0] & 0x80)
			{
				//we now wait for an answer so dont poll
				slot->pollConnection = false;
				/* set the RCV Flag and set DataLast Flag false */
				slot->DataRCV = true;
				slot->DataLast = false;
			} 
			else
			{
				/* set DataLast Flag if it is false*/
				if (!slot->DataLast)
				{
					slot->DataLast = true;
					dprintf(DEBUG_DEBUG, "**** > T_SB\n");
				}
			}
			break;
		}
		
		default:
			printf("unhandled tpdu_tag 0x%0x\n", tpdu_tag);
	}
}


void cDvbCi::setSource(int slot, int source)
{
	char buf[64];
	snprintf(buf, 64, "/proc/stb/tsmux/ci%d_input", slot);
	FILE *ci = fopen(buf, "wb");

	// set source
	if(ci > 0)
	{
		switch(source)
		{
			case TUNER_A:
				fprintf(ci, "A");
				break;
				
			case TUNER_B:
				fprintf(ci, "B");
				break;
				
			case TUNER_C:
				fprintf(ci, "C");
				break;
				
			case TUNER_D:
				fprintf(ci, "D");
				break;
		}

		fclose(ci);
	}
}

void cDvbCi::slot_pollthread(void *c)
{
	ca_slot_info_t info;
	unsigned char data[1024];
	tSlot* slot = (tSlot*) c;
	
	while (1)
	{
		int len = 1024;
		unsigned char* d;
		eData status;
		    
		switch (slot->status)
		{
			case eStatusReset:
				while (slot->status == eStatusReset)
				{
					usleep(1000);
				}
				break;
			case eStatusNone:
			{
				if (slot->camIsReady)
				{
					if (sendCreateTC(slot))
					{
						slot->status = eStatusWait;
						slot->camIsReady = true;
					} 
					else
					{
						usleep(100000);
					}
				} 
				else
				{
					/* wait for pollpri */
					status = waitData(slot->fd, data, &len);
					if (status == eDataStatusChanged)
					{
						info.num = slot->slot;

						if (ioctl(slot->fd, CA_GET_SLOT_INFO, &info) < 0)
							printf("IOCTL CA_GET_SLOT_INFO failed for slot %d\n", slot->slot);

						printf("flags %d %d %d ->slot %d\n", info.flags, CA_CI_MODULE_READY, info.flags & CA_CI_MODULE_READY, slot->slot);

						if (info.flags & CA_CI_MODULE_READY)
						{
							printf("1. cam (%d) status changed ->cam now present\n", slot->slot);

							slot->mmiSession = NULL;
							slot->hasMMIManager = false;
							slot->hasCAManager = false;
							slot->hasDateTime = false;
							slot->hasAppManager = false;
							slot->mmiOpened = false;
							slot->init = false;
							sprintf(slot->name, "unknown module %d", slot->slot);
							slot->status = eStatusNone;

							if (g_RCInput)
								g_RCInput->postMsg(NeutrinoMessages::EVT_CI_INSERTED, slot->slot);
							slot->camIsReady = true;
							//setSource(slot);
						} 
						else
						{
							//noop
						}
					}
				}
			} /* case statusnone */
			break;
			
			case eStatusWait:
			{    
				status = waitData(slot->fd, data, &len);
				if (status == eDataReady)
				{
					slot->pollConnection = false;
					d = data;

					if ((len == 6 && d[4] == 0x80) || len > 6) { 
						dprintf(DEBUG_DEBUG, "slot: %d con-id: %d tpdu-tag: %02X len: %d\n", d[0], d[1], d[2], len);
						dprintf(DEBUG_DEBUG, "received data: >");
						for (int i = 0; i < len; i++)
							dprintf(DEBUG_DEBUG, "%02x ", data[i]);
						dprintf(DEBUG_DEBUG, "\n");
					}

					/* taken from the dvb-apps */
					int data_length = len - 2;
					d += 2; /* remove leading slot and connection id */
					while (data_length > 0)
					{
						unsigned char tpdu_tag = d[0];
						unsigned short asn_data_length;
						int length_field_len;

						if ((length_field_len = asn_1_decode(&asn_data_length, d + 1, data_length - 1)) < 0) 
						{
							printf("Received data with invalid asn from module on slot %02x\n", slot->slot);
							break;
						}

						if ((asn_data_length < 1) ||
						    (asn_data_length > (data_length - (1 + length_field_len)))) 
						{
							printf("Received data with invalid length from module on slot %02x\n", slot->slot);
							break;
						}

						slot->connection_id = d[1 + length_field_len];

						dprintf(DEBUG_DEBUG, "Setting connection_id from received data to %d\n", slot->connection_id);

						d += 1 + length_field_len + 1;
						data_length -= (1 + length_field_len + 1);
						asn_data_length--;

						dprintf(DEBUG_DEBUG, "****tpdu_tag: 0x%02X\n", tpdu_tag);
						process_tpdu(slot, tpdu_tag, d, asn_data_length, slot->connection_id);
						// skip over the consumed data
						d += asn_data_length;
						data_length -= asn_data_length;

					} // while (data_length)
				} /* data ready */
				else if (status == eDataWrite)
				{
					if (!slot->sendqueue.empty()) 
					{
						const queueData &qe = slot->sendqueue.top();
						
						int res = write(slot->fd, qe.data, qe.len);
						if (res >= 0 && (unsigned int)res == qe.len)
						{
							delete [] qe.data;
							slot->sendqueue.pop();
						}
						else
						{
							printf("r = %d, %m\n", res);
						}			
					}
					/* check for activate the pollConnection */
					if (!checkQueueSize(slot) && (slot->DataRCV || slot->mmiOpened || slot->counter > 5))
					{
						slot->pollConnection = true;
					}
					if (slot->counter < 6)
						slot->counter++;
					else
						slot->counter = 0;
					/* if Flag: send a DataLast */
					if (!checkQueueSize(slot) && slot->pollConnection && slot->DataLast)
					{
						sendDataLast(slot);
					}
					/* if Flag: send a RCV */
					if (!checkQueueSize(slot) && slot->pollConnection && slot->DataRCV)
					{
						sendRCV(slot);
					}
				}
				else if (status == eDataStatusChanged)
				{
					info.num = slot->slot;

					if (ioctl(slot->fd, CA_GET_SLOT_INFO, &info) < 0)
						printf("IOCTL CA_GET_SLOT_INFO failed for slot %d\n", slot->slot);

					printf("flags %d %d %d ->slot %d\n", info.flags, CA_CI_MODULE_READY, info.flags & CA_CI_MODULE_READY, slot->slot);

					if ((slot->camIsReady == false) && (info.flags & CA_CI_MODULE_READY))
					{
						printf("2. cam (%d) status changed ->cam now present\n", slot->slot);

						slot->mmiSession = NULL;
						slot->hasMMIManager = false;
						slot->hasCAManager = false;
						slot->hasDateTime = false;
						slot->hasAppManager = false;

						slot->mmiOpened = false;

						slot->init = false;

						sprintf(slot->name, "unknown module %d", slot->slot);

						slot->status = eStatusNone;

						if (g_RCInput)
							g_RCInput->postMsg(NeutrinoMessages::EVT_CI_INSERTED, slot->slot);

						slot->camIsReady = true;
					} 
					else if ((slot->camIsReady == true) && (!(info.flags & CA_CI_MODULE_READY)))
					{
						printf("cam (%d) status changed ->cam now _not_ present\n", slot->slot);

						eDVBCISession::deleteSessions(slot);

						slot->mmiSession = NULL;
						slot->hasMMIManager = false;
						slot->hasCAManager = false;
						slot->hasDateTime = false;
						slot->hasAppManager = false;

						slot->mmiOpened = false;

						slot->init = false;

						slot->DataLast = false;
						slot->DataRCV = false;

						slot->counter = 0;
						slot->pollConnection = false;
						sprintf(slot->name, "unknown module %d", slot->slot);

						slot->status = eStatusNone;

						if (g_RCInput)
							g_RCInput->postMsg(NeutrinoMessages::EVT_CI_REMOVED, slot->slot);

						while(slot->sendqueue.size())
						{
							delete [] slot->sendqueue.top().data;
							slot->sendqueue.pop();
						}

						slot->camIsReady = false;
						usleep(100000);		
					}
				}
			}
			break;
			
			default:
				printf("unknown state %d\n", slot->status);		
			break;
		}
	   
		if (slot->hasCAManager && slot->hasAppManager && !slot->init) //declare this as init, but remeber we are still not complete!
		{

			slot->init = true;
			
			if (g_RCInput)
				g_RCInput->postMsg(NeutrinoMessages::EVT_CI_INIT_OK, slot->slot);
		    
			//resend a capmt if we have one. this is not very proper but I cant any mechanism in
			//neutrino currently. so if a cam is inserted a pmt is not resend
			if (slot->caPmt != NULL)
			{
				SendCaPMT(slot->caPmt, slot->source);
			}
		}
	}
}

/* helper function to call the cpp thread loop */
void* execute_thread(void *c)
{
	tSlot* slot = (tSlot*) c;
	cDvbCi *obj=(cDvbCi*)slot->pClass;

	obj->slot_pollthread(c);

	return NULL;
}

bool cDvbCi::SendCaPMT(CCaPmt *caPmt, int source)
{
	printf("%s:%s\n", FILENAME, __FUNCTION__);

	std::list<tSlot*>::iterator it;

        for(it = slot_data.begin(); it != slot_data.end(); it++)
        {
		//FIXME: ask cammanagr if module can handle this caids
		if (((*it)->fd > 0) && ((*it)->camIsReady)) 
		{
			unsigned int size = caPmt->getLength();
			
			unsigned char buffer[3 + get_length_field_size(size) + size];
			
			// get len and fill buffer
			dprintf(DEBUG_DEBUG, " %d, %d\n", get_length_field_size(size), size);
			int len = caPmt->writeToBuffer(buffer, 0, 0xff);

			dprintf(DEBUG_NORMAL, "capmt(%d): > \n", len);

			if ((*it)->hasCAManager)
				(*it)->camgrSession->sendSPDU(0x90, 0, 0, buffer, len);
			
			//set source
			setSource((*it)->slot, source);
		}
               
		/* konfetti: if cam is not ready we must store caPmt too, because
		* changing the slot should also descramble the channel
		*/
		if ((*it)->fd > 0) 
		{
			//FIXME: hmmm is this a copy or did I only save the pointer.
			//in copy case I must do some delete etc before if set and in destruct case
			(*it)->caPmt = caPmt;
			(*it)->source = source;
		}
	}
	
        return true;
}

//
cDvbCi::cDvbCi(int Slots) 
{
	printf("%s:%s\n", FILENAME, __FUNCTION__);

	int fd, i;
	char filename[128];
	ci_num = 0;

	for (i = 0; i < Slots; i++)
	{
		sprintf(filename, "/dev/dvb/adapter0/ci%d", i);

		fd = open(filename, O_RDWR | O_NONBLOCK);
		
		if(fd < 0)
		{
			sprintf(filename, "/dev/ci%d", i);
			fd = open(filename, O_RDWR | O_NONBLOCK);
		}
	    
		if (fd > 0)
		{
			tSlot *slot = (tSlot*) malloc(sizeof(tSlot));
			
			ci_num++;

			slot->slot   = i;
			slot->fd     = fd;
			slot->connection_id = 0;
			slot->status = eStatusNone;
			slot->receivedLen = 0;
			slot->receivedData = NULL;
			slot->pClass = this;
			slot->pollConnection = false;
			slot->camIsReady = false;

			slot->DataLast = false;
			slot->DataRCV = false;

			slot->hasMMIManager = false;
			slot->hasCAManager = false;
			slot->hasDateTime = false;
			slot->hasAppManager = false;

			slot->mmiOpened = false;

			slot->counter = 0;
			slot->init = false;
	    
			slot->caPmt = NULL;
			slot->source = TUNER_A;

			sprintf(slot->name, "unknown module %d", i);

			slot_data.push_back(slot);
			
			/* now reset the slot so the poll pri can happen in the thread */
			reset(i); 

			/* create a thread for each slot */	  
			if (pthread_create(&slot->slot_thread, 0, execute_thread,  (void*)slot)) 
			{
				printf("pthread_create\n");
			}
		}
	}
}

cDvbCi::~cDvbCi()
{
	printf("%s:%s\n", FILENAME, __FUNCTION__);
}


static cDvbCi* pDvbCiInstance = NULL;

cDvbCi * cDvbCi::getInstance()
{
	printf("%s:%s\n", FILENAME, __FUNCTION__);
	
	if (pDvbCiInstance == NULL)
		pDvbCiInstance = new cDvbCi(MAX_SLOTS);		
	
	return pDvbCiInstance;
}

bool cDvbCi::CamPresent(int slot)
{
	dprintf(DEBUG_NORMAL, "%s:%s(slot %d)\n", FILENAME, __FUNCTION__, slot);

	std::list<tSlot*>::iterator it;
	
        for(it = slot_data.begin(); it != slot_data.end(); ++it)
        {
		if ((*it)->slot == slot) 
		{
			return (*it)->camIsReady;
			break;
		}
	}
	
	return false; 
}

bool cDvbCi::GetName(int slot, char * name)
{
	dprintf(DEBUG_NORMAL, "%s:%s\n", FILENAME, __FUNCTION__);

	std::list<tSlot*>::iterator it;
	
        for(it = slot_data.begin(); it != slot_data.end(); ++it)
        {
		//printf("%d. name = %s, %p\n", (*it)->slot, (*it)->name, (*it));
		
		if ((*it)->slot == slot) 
		{
			strcpy(name, (*it)->name);
			break;
		}
	}
	
	return true;
}

void cDvbCi::reset(int slot)
{
	std::list<tSlot*>::iterator it;
	bool haveFound = false;

	for(it = slot_data.begin(); it != slot_data.end(); ++it)
	{
		if ((*it)->slot == slot) 
		{
			haveFound = true;
			break;
		}
	}

	if (haveFound)
	{
		(*it)->status = eStatusReset;
		usleep(200000);
		eDVBCISession::deleteSessions((tSlot*)(*it));
		(*it)->mmiSession = NULL;
		(*it)->hasMMIManager = false;
		(*it)->hasCAManager = false;
		(*it)->hasDateTime = false;
		(*it)->hasAppManager = false;
		(*it)->mmiOpened = false;
		(*it)->camIsReady = false;

		(*it)->DataLast = false;
		(*it)->DataRCV = false;

		(*it)->counter = 0;
		(*it)->init = false;
		(*it)->pollConnection = false;
		sprintf((*it)->name, "unknown module %d", (*it)->slot);

		(*it)->source = TUNER_A;

		while((*it)->sendqueue.size())
		{
			delete [] (*it)->sendqueue.top().data;
			(*it)->sendqueue.pop();
		}

		if (ioctl((*it)->fd, CA_RESET, (*it)->slot) < 0)
			printf("IOCTL CA_RESET failed for slot %d\n", slot);
		usleep(200000);
		(*it)->status = eStatusNone;
	}    
}
