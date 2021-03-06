
/* File generated by gen_cfile.py. Should not be modified. */

#ifndef MTDCAN3MASTER_H
#define MTDCAN3MASTER_H

#include "data.h"

/* Prototypes of function provided by object dictionnary */
UNS32 MTDCAN3Master_valueRangeTest (UNS8 typeValue, void * value);
const indextable * MTDCAN3Master_scanIndexOD (UNS16 wIndex, UNS32 * errorCode, ODCallback_t **callbacks);

/* Master node data struct */
extern CO_Data MTDCAN3Master_Data;
extern UNS8 can3_SyncDevc;		/* Mapped at index 0x2000, subindex 0x00*/
extern UNS8 can3_TxTime[6];		/* Mapped at index 0x2001, subindex 0x01 - 0x06 */
extern UNS8 can3_TxDoorSpeedTimBit[2];		/* Mapped at index 0x2002, subindex 0x01 - 0x02 */
extern UNS8 can3_TxAccTempTimBit[2];		/* Mapped at index 0x2003, subindex 0x01 - 0x02 */
extern UNS8 can3_TxBCUCmd[5];		/* Mapped at index 0x2004, subindex 0x01 - 0x05 */
extern UNS8 can3_TxVVVFCmd[8];		/* Mapped at index 0x2005, subindex 0x01 - 0x08 */
extern UNS8 can3_TxSIVCmd[5];		/* Mapped at index 0x2006, subindex 0x01 - 0x05 */
extern UNS8 can3_TxATCSdr1[6];		/* Mapped at index 0x2007, subindex 0x01 - 0x06 */
extern UNS8 can3_TxATCSdr2[5];		/* Mapped at index 0x2008, subindex 0x01 - 0x05 */
extern UNS8 can3_TxPISSdr1[2];		/* Mapped at index 0x2009, subindex 0x01 - 0x02 */
extern UNS8 can3_TxPISSdr2[8];		/* Mapped at index 0x200A, subindex 0x01 - 0x08 */
extern UNS8 can3_RxMFD1DI[2];		/* Mapped at index 0x200B, subindex 0x01 - 0x02 */
extern UNS8 can3_RxMFD2DI[2];		/* Mapped at index 0x200C, subindex 0x01 - 0x02 */
extern UNS8 can3_RxMFD3DI[2];		/* Mapped at index 0x200D, subindex 0x01 - 0x02 */
extern UNS8 can3_RxDoorSta1[8];		/* Mapped at index 0x200E, subindex 0x01 - 0x08 */
extern UNS8 can3_RxDoorSta2[8];		/* Mapped at index 0x200F, subindex 0x01 - 0x08 */
extern UNS8 can3_RxAccSta[8];		/* Mapped at index 0x2010, subindex 0x01 - 0x08 */
extern UNS8 can3_RxBCUSta1[8];		/* Mapped at index 0x2011, subindex 0x01 - 0x08 */
extern UNS8 can3_RxBCUSta2[8];		/* Mapped at index 0x2012, subindex 0x01 - 0x08 */
extern UNS8 can3_RxBCUSta3[8];		/* Mapped at index 0x2013, subindex 0x01 - 0x08 */
extern UNS8 can3_RxATCSta1[7];		/* Mapped at index 0x2014, subindex 0x01 - 0x07 */
extern UNS8 can3_RxATCSta2[8];		/* Mapped at index 0x2015, subindex 0x01 - 0x08 */
extern UNS8 can3_RxATCSta3[8];		/* Mapped at index 0x2016, subindex 0x01 - 0x08 */
extern UNS8 can3_RxATCSta4[4];		/* Mapped at index 0x2017, subindex 0x01 - 0x04 */
extern UNS8 can3_RxPISSta1[8];		/* Mapped at index 0x2018, subindex 0x01 - 0x08 */
extern UNS8 can3_RxPISSta2[3];		/* Mapped at index 0x2019, subindex 0x01 - 0x03 */
extern UNS8 can3_RxVVVF1Sta1[8];		/* Mapped at index 0x201A, subindex 0x01 - 0x08 */
extern UNS8 can3_RxVVVF1Sta2[6];		/* Mapped at index 0x201B, subindex 0x01 - 0x06 */
extern UNS8 can3_RxVVVF1Sta3[8];		/* Mapped at index 0x201C, subindex 0x01 - 0x08 */
extern UNS8 can3_RxVVVF1Sta4[8];		/* Mapped at index 0x201D, subindex 0x01 - 0x08 */
extern UNS8 can3_RxVVVF1Sta5[6];		/* Mapped at index 0x201E, subindex 0x01 - 0x06 */
extern UNS8 can3_RxVVVF2Sta1[8];		/* Mapped at index 0x201F, subindex 0x01 - 0x08 */
extern UNS8 can3_RxVVVF2Sta2[6];		/* Mapped at index 0x2020, subindex 0x01 - 0x06 */
extern UNS8 can3_RxVVVF2Sta3[8];		/* Mapped at index 0x2021, subindex 0x01 - 0x08 */
extern UNS8 can3_RxVVVF2Sta4[8];		/* Mapped at index 0x2022, subindex 0x01 - 0x08 */
extern UNS8 can3_RxVVVF2Sta5[6];		/* Mapped at index 0x2023, subindex 0x01 - 0x06 */
extern UNS8 can3_RxSIVSd1[7];		/* Mapped at index 0x2024, subindex 0x01 - 0x07 */
extern UNS8 can3_RxSIVSd2[8];		/* Mapped at index 0x2025, subindex 0x01 - 0x08 */
extern UNS8 can3_RxSIVSd3[3];		/* Mapped at index 0x2026, subindex 0x01 - 0x03 */
extern UNS8 can3_RxSubDevFault[2];		/* Mapped at index 0x2027, subindex 0x01 - 0x02 */
extern UNS8 can3_RxAccSta2[1];		/* Mapped at index 0x2028, subindex 0x01 - 0x01 */
extern UNS8 can3_RxATCTime[6];		/* Mapped at index 0x2029, subindex 0x01 - 0x06 */
extern UNS8 can3_RxMFD1SubFault[2];		/* Mapped at index 0x202A, subindex 0x01 - 0x02 */
extern UNS8 can3_RxMFD2SubFault[1];		/* Mapped at index 0x202B, subindex 0x01 - 0x01 */
extern UNS8 can3_RxMFD3SubFault[1];		/* Mapped at index 0x202C, subindex 0x01 - 0x01 */
extern UNS8 can3_RxACPSta1[8];		/* Mapped at index 0x202D, subindex 0x01 - 0x08 */
extern UNS8 can3_RxACPSta2[3];		/* Mapped at index 0x202E, subindex 0x01 - 0x03 */
extern UNS8 IAP_S_IAP_SEG[256];		/* Mapped at index 0x5FFF, subindex 0x01 */
extern UNS64 IAP_S_IAP_CMD;		/* Mapped at index 0x5FFF, subindex 0x02 */
extern UNS64 IAP_S_IAP_CMD_R;		/* Mapped at index 0x5FFF, subindex 0x03 */

#endif // MTDCAN3MASTER_H
