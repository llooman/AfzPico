
#define JL_VERSION 10000
#define DEBUG
/*

2021-08-26 0.01.00 start with pico-i2c and pico-bme280 
*/


#define NODE_ID 45
#define NODE_TYPE 45
#define TRACE_SEC 3
 
#define DETECT_230V_PIN 2

// #define TEMP_PIN 3
 
#define FAN_1_PIN 4 
#define FAN_2_PIN 5 
#define FAN_3_PIN 6

#define TEMPS_PIN D6

#define MY_LED D10
 
#define SPI_CS_BME D17  // CSB   10 53 			Chip Select             wit
#define SPI_DO     D27  // MOSI  11 51 SDI SDA 	Rx		Data in		geel
#define SPI_DI     D28  // MISO  12 50 SDO 	Tx		Data out	groen
#define SPI_SCK    D26	// Clock 13 52 SCK SCL 	                           zwart
 
#include "arduino.h"
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
// #include "hardware/spi.h"
// #include "pico/binary_info.h"

#include "BME280Pi.h"
#include <NetwPico.h>
#include <MyTimers.h>
#include <EEUtil.h>

#include <DS18B20.h>
// #include <OneWirePi.h>

#define FAN_ID 50  			// vertilator aan/uit  131 sleep 231 manual on
// #define SET_FAN_MANUAL_ID 51
#define FAN_TEMP 55
#define NIGHTLY_ID 56

#define LAMP_ID 45

#define VOCHT_HOOG_ID 32
#define VOCHT_LAAG_ID 33
#define MANUAL_TIMER_ID 36
#define DEVIATION_TIMER_ID 37
#define DEVIATION_ID 43

#define MAX_DOUCH_TEMP_ID 38
#define MAX_KRAAN_TEMP_ID 39

#define SET_HIGH_DELAY_ID 40
#define SET_MEDIUM_DELAY_ID 41
#define SET_LOW_DELAY_ID 42

 
#define REL_HUM_ID 24
#define HUM_TEMP_ID 25
#define ABS_HUM_ID 26
#define HUM_MEDIAN_ID 27
#define HUM_DEVIATION_ID 28
#define PRESS_ID 29

// DS18B20's
#define BUITEN_TEMP_ID 21		// Buiten
#define DOUCH_TEMP_ID 22		// Douch
#define KRAAN_TEMP_ID 23		// Kraan Bad en wastafel (lange draad terug naar de ketel !!!!)


#define TIMERS_COUNT 21
MyTimers myTimers(TIMERS_COUNT);
#define TIMER_TRACE 0
#define TIMER_TEST 1
#define TIMER_UPLOAD_ON_BOOT 2
#define TIMER_UPLOAD_FAN 3
#define TIMER_FAN_HIGH 4
#define TIMER_FAN_MEDIUM 5
#define TIMER_FAN_LOW 6
#define TIMER_230 7
#define TIMER_LAMP_UPLOAD 8
#define TIMER_LAMP_ON 9
#define TIMER_SECOND_OF_THE_DAY 10
#define TIMER_NIGHTLY_UPLOAD 11
#define TIMER_READ_BME 12
#define TIMER_UPLOAD_REL_HUM 13
#define TIMER_UPLOAD_HUM_TEMP 14
#define TIMER_UPLOAD_ABS_HUM 15
#define TIMER_UPLOAD_HUM_MEDIAN 16
#define TIMER_UPLOAD_DEVIATION 17
#define TIMER_UPLOAD_PRESS 18
#define TIMER_STATS_ON_HUMID 19
#define TIMER_LED 20

const byte dsKraan[] PROGMEM = {0x28, 0xff, 0xab, 0x19, 0xa0, 0x16, 0x03, 0xf2};  //28 FF AB 19 A0 16 3 F2
const byte ds104[] PROGMEM = {0x28, 0xde, 0xc3, 0x57, 0x05, 0x00, 0x00, 0xce};  // waterdicht
const byte ds107[] PROGMEM = {0x28, 0x37, 0xDB, 0x8C, 0x06, 0x00, 0x00, 0x42};  // waterdicht zelf   28 37 DB 8C 6 0 0 42
const byte dsTest[] PROGMEM = {0x28, 0xbf, 0x8e, 0xf3, 0x1f, 0x13, 0x01, 0x5f};  //	 28 bf 8e f3 1f 13 01 5f

// NetwPico parentNode(  D2    ); // SDA D2 defaults to port 1 ! same as: NetwPico parentNode( 1, D2, D3   );
NetwPico parentNode(  D2, D3, D12, D13    ); // SDA D2 defaults to port 1 ! same as: NetwPico parentNode( 1, D2, D3   );

// NetwPico hubNode( 0, D12, D13  );
 
#ifdef SPI_CS_BME
	BME280Pi bme( 1, SPI_CS_BME, SPI_SCK, SPI_DO, SPI_DI);
#endif


// One_wire one_wire(7); //GP15 - Pin 20 on Pi Pico
// rom_address_t address{};

#ifdef TEMPS_PIN
	#ifdef DEBUG  // invalid shading by IDE !!!!
		DS18B20 tempDouch( TEMPS_PIN);  // dsTest 
	#else
		DS18B20 tempBuiten(TEMPS_PIN, ds107);  //, ds107  wit , 21
		DS18B20 tempDouch( TEMPS_PIN, ds104);  // ds04, ds103 TEMP_SENSOR_CV , 22
		DS18B20 tempKraan( TEMPS_PIN, dsKraan);  //  ds103 TEMP_SENSOR_CV , 23
	#endif
#endif

int32_t humUploaded = 0;
int32_t tempUploaded = 0;
int32_t absHumUploaded = 0;
int32_t pressUploaded = 0;

							
int 	uploadOnBootCount=0;

unsigned long  	secOfTheDay = 0;
unsigned long 	nextSecond = 0;

unsigned long 	testMinute = 0;

bool nightly = false;
int fanSpeed = 0;
int prevVocht = 0;

bool lampOn = false;
bool lampUploaded = false;

volatile unsigned long curr230vcount = 0;
		 unsigned long prev230vcount = 0;
		 unsigned long curr230vcountStamp = 0;



int  upload(int id, long val, unsigned long timeStamp);
int  upload(int id, long val) ;
int  upload(int id);
int  uploadError(int id, long val);

int  upload2(int id, long val, unsigned long timeStamp);
int  uploadError2(int id, long val);
int  handleParentReq2( RxItem *rxItem) ;

void localSetVal(int id, long val);

int  handleParentReq( RxItem *rxItem) ;
int  localRequest(RxItem *rxItem);
void trace(void);


void refreshSettings(void);
int evaluateFanOnOff(bool on, bool isRunning);
 
void resetFan(void);
void fanLoop(void);
void isr230v(void);
void isrMHZ19(void);