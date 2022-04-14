/*
 * Functions for using the mailbox property interface
 */
#ifndef _HW_PROPERTIES_H_
#define _HW_PROPERTIES_H_

#include <stdint.h>

uint32_t getVCFirmwareVersion();

uint32_t getBoardModel();
uint32_t getBoardRevision();
void getMACAddr();  // needs to return 6 byte array
uint64_t getBoardSerialNum();


#endif  // _HW_PROPERTIES_H_
