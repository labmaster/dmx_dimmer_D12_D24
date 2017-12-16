#ifndef _DEBUGOUT_H
 #define _DEBUGOUT_H

extern void DBG_setStart(void);
extern void DBG_setStop(void);
extern unsigned char DBG_sendByte(unsigned char byte);
extern void DBG_Init(void);
extern void DBG_outputCPUData(void);



#endif

