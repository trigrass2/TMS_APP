/*
This file is part of CanFestival, a library implementing CanOpen Stack. 

Copyright (C): Edouard TISSERANT and Francis DUPIN

See COPYING file for copyrights details.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "CcuLedSlave.h"
#include "CcuLedMasterSlave.h"
#include "emcy.h"


extern s_BOARD SlaveBoard;

/*****************************************************************************/
void Ccu1LedSlave_heartbeatError(CO_Data* d, UNS8 heartbeatID)
{
	g_CCU1HeartFlag = NoRxHeart;			//����վû�м�ؽ��ܵ���վ��������ʱ�򣬱�ʾ��վû������
	eprintf("Ccu1LedSlave_heartbeatError %d\r\n", heartbeatID);
}

void Ccu1LedSlave_initialisation(CO_Data* d)
{
	eprintf("Ccu1LedSlave_initialisation\r\n");
	
}

void Ccu1LedSlave_preOperational(CO_Data* d)
{
	eprintf("Ccu1LedSlave_preOperational\r\n");
	
}

void Ccu1LedSlave_operational(CO_Data* d)
{
	eprintf("Ccu1LedSlave_operational\r\n");
}

void Ccu1LedSlave_stopped(CO_Data* d)
{
	eprintf("Ccu1LedSlave_stopped\r\n");
}

void Ccu1LedSlave_post_sync(CO_Data* d)
{
   eprintf("Ccu1LedSlave_post_sync\r\n");   
}

void Ccu1LedSlave_post_TPDO(CO_Data* d)
{
	eprintf("Ccu1LedSlave_post_TPDO\r\n");
}

void Ccu1LedSlave_storeODSubIndex(CO_Data* d, UNS16 wIndex, UNS8 bSubindex)
{
	/*TODO : 
	 * - call getODEntry for index and subindex, 
	 * - save content to file, database, flash, nvram, ...
	 * 
	 * To ease flash organisation, index of variable to store
	 * can be established by scanning d->objdict[d->ObjdictSize]
	 * for variables to store.
	 * 
	 * */
	eprintf("Ccu1LedSlave_storeODSubIndex : %4.4x %2.2x\r\n", wIndex,  bSubindex);
}

void Ccu1LedSlave_post_emcy(CO_Data* d, UNS8 nodeID, UNS16 errCode, UNS8 errReg)
{
	eprintf("Slave received EMCY message. Node: %2.2x  ErrorCode: %4.4x  ErrorRegister: %2.2x\r\n", nodeID, errCode, errReg);
}
