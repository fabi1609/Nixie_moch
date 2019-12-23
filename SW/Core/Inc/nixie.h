/*
 * nixie.h
 *
 *  Created on: Oct 29, 2019
 *      Author: Fabian
 */

#ifndef INC_NIXIE_H_
#define INC_NIXIE_H_

#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_system.h"

#define DAC_TARGET 2700 //2940 approx. 160V   2670=145V   4095 -> 223V
//--------------------------------------------------------------
// DCF77 time defines (ms)
// lo [soll=100ms, ist=138...150ms]
// hi [soll=200ms, ist=244...260ms]
//--------------------------------------------------------------
#define  DCF77_TIMOUT    2000
#define  DCF77_SYNC      1500
#define  DCF77_LO_MIN    75
#define  DCF77_LO_MAX    170
#define  DCF77_HI_MIN    175
#define  DCF77_HI_MAX    280

//--------------------------------------------------------------
// enum vom DCF77-Status
//--------------------------------------------------------------
typedef enum {
  DCF77_NO_SIGNAL =0, // there is no DCF77-Signal
  DCF77_READING,      // wait until time-data is complete
  DCF77_TIME_ERROR,   // error
  DCF77_TIME_OK       // time-data is ok
}DCF77_Status_t;

//--------------------------------------------------------------
// interne Struktur
//--------------------------------------------------------------
typedef enum {
  M77_Init = 0,     // init
  M77_Wait4Signal,  // waiting for DCF77-Signal
  M77_Wait4Sync,    // waiting for Sync Puls
  M77_SyncOk,       // Sync Puls ok
  M77_Wait4Time,    // waiting until Time is ready
  M77_Ready,        // Time is ready
  M77_Error,        // Error in DCF77-Signal
}DCF77_Mode_t;

typedef struct {
  DCF77_Mode_t mode;
  uint8_t ok;    // flag
  uint8_t min;   // minuten
  uint8_t std;   // stunden
  uint8_t tag;   // tag
  uint8_t monat; // monat
  uint8_t jahr; // jahr
}DCF77_interal_t;
//--------------------------------------------------------------

//--------------------------------------------------------------
// globale DCF77-Struktur
//--------------------------------------------------------------
typedef struct {
  uint8_t sek;   // sekunden [0..59]
  uint8_t min;   // minuten [0..59]
  uint8_t std;   // stunden [0..23]
  uint8_t tag;   // tag [1..31]
  uint8_t monat; // monat [1..12]
  uint8_t jahr;  // jahr [0..99]
}DCF77_TIME_t;
DCF77_TIME_t DCF77_TIME;

void nixie_init();
void boost_op();
void nixie_set_time(uint8_t h, uint8_t m, uint8_t s);

//DCF77
void UB_DCF77_Init(void);
DCF77_Status_t UB_DCF77_ReadTime(void);

#endif /* INC_NIXIE_H_ */
