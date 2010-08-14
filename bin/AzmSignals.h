/*
 * =====================================================================================
 *
 *       Filename:  AzmSignals.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/08/2010 17:25:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */


/* #####   HEADER FILE INCLUDES   ################################################### */
#define _AZMSIGNALS_H_
#ifndef __COMMON_H__
#include <common.h>
#endif

#ifndef _SCHEDULERTYPES_H_
#include <SchedulerTypes.h>
#endif

extern void          sig_hup();
extern void          sig_int();
extern void          sig_kill();
extern void          sig_quit();
extern Scheduler_t * sig_sc( Scheduler_t *sc );
extern void sig_init( Scheduler_t *sc );

