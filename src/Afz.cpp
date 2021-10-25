#include "Afz.h"
 
#include "parms.h" 
EEParms eeParms;
#include "netw.h" 

void setup1() {
  	pinMode(MY_LED, OUTPUT);


	// hubNode.onReceive( handleParentReq2);
	// hubNode.onError(uploadError2);
	// hubNode.onUpload(upload2);
	// hubNode.nodeId = 9;
	// hubNode.isParent = false;
	// hubNode.pullUp = false;
	// hubNode.autoPing = 0;

	myTimers.nextTimer(TIMER_LED, 1);
	// hubNode.begin(9);
}

void loop1(){

	if(myTimers.isTime(TIMER_LED)){

		gpio_put(MY_LED, !digitalRead(MY_LED));		 

		myTimers.nextTimer(TIMER_LED, 3);
		// hubNode.trace("hup");

		// hubNode.txCmd('S', 45, 77, 0);
		// hubNode.txCmd('R', 45, 8);

	}

	// hubNode.loop();

}


void setup() {

  	Serial.begin(115200);  //9600  115200
	// while(!Serial) {} // Wait
    // stdio_init_all();


	// pinMode(FAN_1_PIN, OUTPUT);
	// pinMode(FAN_2_PIN, OUTPUT);
	// pinMode(FAN_3_PIN, OUTPUT);

	// #ifdef DETECT_230V_PIN
	// 	pinMode(DETECT_230V_PIN, INPUT_PULLUP);
	// #endif

	eeParms.onUpload(upload);
	eeParms.setup( );

	#ifdef DEBUG
		Serial.println(F("DEBUG Afz...")); Serial.flush();
	#else
		Serial.println(F("Afz..."));Serial.flush();
	#endif

	int nodeId = NODE_ID;

	parentNode.onReceive( handleParentReq);
	parentNode.onError(uploadError);
	parentNode.onUpload(upload);
	parentNode.nodeId = nodeId;
	parentNode.isParent = true;
 
	// parentNode.autoPing = 2;
	// parentNode.twUploadNode = 0x10;
	parentNode.pullUp = false;





    // one_wire.init();

	#ifdef TEMPS_PIN
	tempDouch.tempMin = 500;
	// tempDouch.setResolution(12);
	tempDouch.onUpload(upload, DOUCH_TEMP_ID);
	tempDouch.onError(uploadError);
	tempDouch.setSyncInterval(eeParms.samplePeriode_sec);

		#ifndef DEBUG
			tempBuiten.onUpload(upload, 21);
			tempBuiten.onError(uploadError);
			tempBuiten.setSyncInterval(eeParms.samplePeriode_sec);

			tempKraan.tempMin = 500;
			tempKraan.onUpload(upload, 23);
			tempKraan.onError(uploadError);
			tempKraan.setSyncInterval(eeParms.samplePeriode_sec);
		#endif
	#endif

	#ifdef SPI_CS_BME
		bme.senceTiming = 21;  // sense every 21 seconds
	#endif

	// myTimers.nextTimer(TIMER_TEST, 1);
	myTimers.nextTimer(TIMER_TRACE, TRACE_SEC);
	myTimers.nextTimer(TIMER_UPLOAD_ON_BOOT, 0);
	// myTimers.nextTimer(TIMER_230, 0);
	myTimers.nextTimer(TIMER_SECOND_OF_THE_DAY, 3);

	myTimers.nextTimer(TIMER_UPLOAD_REL_HUM, 1);
	myTimers.nextTimer(TIMER_UPLOAD_ABS_HUM, 1);
	myTimers.nextTimer(TIMER_UPLOAD_HUM_TEMP, 1);
	myTimers.nextTimer(TIMER_UPLOAD_PRESS, 1);		



	// sleep_ms(5000);
	parentNode.begin(nodeId);
}

// void dayTimeLoop(){

// 	if(millis()>=nextSecond)
// 	{
// 		secOfTheDay++;
// 		secOfTheDay = secOfTheDay % 86400;
// 		nextSecond = millis()+ 1000;
// 	}
// }

// void v230loop(){

// 	if(myTimers.isTime(TIMER_230)){
// 		myTimers.nextTimerMillis(TIMER_230, 70);
// 		lampOn =  curr230vcount > prev230vcount + 5;

// 		prev230vcount = curr230vcount;
// 		if( lampOn != lampUploaded ){
// 			myTimers.nextTimer(TIMER_LAMP_UPLOAD, 0);
// 			if(lampOn) myTimers.nextTimer(TIMER_LAMP_ON, 3);			
// 		}

// 		if(!lampOn) myTimers.timerOff(TIMER_LAMP_ON);
// 	}
// }

// void fanLoop(){

// 	int currSpeed = fanSpeed;
// 	int currHumidity = 50;
	 
// 	bool currNightly = nightly;

// 	if( secOfTheDay > 25200L   //  7:00 am 25200L  9:00  32400L
// 	   && secOfTheDay < 82800L // 23:00 pm 82800    23:30 84600
// 	){

// 		nightly = false;
// 	} else {

// 		nightly = true;
// 	}

// 	if(nightly != currNightly) myTimers.nextTimer(TIMER_NIGHTLY_UPLOAD,0);
 
 
// 	if( bme.relHum > 0){
// 		currHumidity = bme.relHum;
// 	}

 
// 	/*
// 	 *  set timers on max tempratures, humidity and standard deviation
// 	 */
// 	if( currHumidity >= eeParms.vochtHoog 
 
// 	){
// 		myTimers.nextTimer(TIMER_FAN_HIGH,   eeParms.highDelayMin * 60);
// 	} 


// 	if( myTimers.isTime(TIMER_LAMP_ON)
// 	 || currHumidity >= eeParms.vochtLaag
// 	//  || ( statsOnHumidity._cnt < 7 && ( humDeviation * 10 > eeParms.minDeviation ) )
// 	){
// 		myTimers.nextTimer(TIMER_FAN_MEDIUM,  eeParms.mediumDelayMin * 60);
// 	}	

// 	if( myTimers.isTimerActive( TIMER_FAN_HIGH ) ){

// 		myTimers.nextTimer( TIMER_FAN_MEDIUM, eeParms.mediumDelayMin * 60);
// 		myTimers.nextTimer( TIMER_FAN_LOW, eeParms.lowDelayMin * 60);

// 		if(nightly) fanSpeed = 2; else fanSpeed = 3;

// 		if( myTimers.isTime( TIMER_FAN_HIGH) ){
// 			myTimers.timerOff( TIMER_FAN_HIGH );
// 			fanSpeed = 2;			
// 		}

// 	} else if( myTimers.isTimerActive( TIMER_FAN_MEDIUM ) ){

// 		myTimers.nextTimer( TIMER_FAN_LOW,  eeParms.lowDelayMin * 60);
// 		fanSpeed = 2;

// 		if( myTimers.isTime( TIMER_FAN_MEDIUM ) ){
// 			myTimers.timerOff( TIMER_FAN_MEDIUM );
// 			fanSpeed = 1;			
// 		}

// 	} else if( myTimers.isTimerActive( TIMER_FAN_LOW ) ){

// 		fanSpeed = 1;

// 		if( myTimers.isTime( TIMER_FAN_LOW ) ){
// 			myTimers.timerOff( TIMER_FAN_LOW );
// 			fanSpeed = 0;
// 		}
// 	} else {

// 		fanSpeed = 0;
// 	}	


// 	/*
// 	 *  translate FAN_timers to actual speed HIGH, MEDIUM, LOW, OFF
// 	 */
// 	if( fanSpeed != currSpeed){
// 		myTimers.nextTimer(TIMER_UPLOAD_FAN, 0);
// 	}

// 	{
// 		if( digitalRead(FAN_1_PIN) && fanSpeed!=1  ) digitalWrite(FAN_1_PIN, 0); 
// 		if( digitalRead(FAN_2_PIN) && fanSpeed!=2  ) digitalWrite(FAN_2_PIN, 0); 
// 		if( digitalRead(FAN_3_PIN) && fanSpeed!=3  ) digitalWrite(FAN_3_PIN, 0); 

// 		if( fanSpeed==1 && !digitalRead(FAN_1_PIN) ) digitalWrite(FAN_1_PIN, 1); 
// 		if( fanSpeed==2 && !digitalRead(FAN_2_PIN) ) digitalWrite(FAN_2_PIN, 1); 
// 		if( fanSpeed==3 && !digitalRead(FAN_3_PIN) ) digitalWrite(FAN_3_PIN, 1); 
// 	}
// }


void loop() {

  	// rp2040.idleOtherCore();

	// dayTimeLoop();
	// v230loop();
	// fanLoop();

  	parentNode.loop();
	// hubNode.loop();


	#ifdef SPI_CS_BME
		bme.loop();
	#endif

	#ifdef TEMPS_PIN
	tempDouch.loop();

	#ifndef DEBUG
		tempBuiten.loop();
		tempKraan.loop();
	#endif
	#endif

	if( parentNode.isReady() 
	 && parentNode.isTxEmpty()
	){
		if( myTimers.isTime(TIMER_UPLOAD_ON_BOOT)
		){
			myTimers.nextTimerMillis(TIMER_UPLOAD_ON_BOOT,TWI_SEND_ERROR_INTERVAL ); //
			switch( uploadOnBootCount )
			{
			case 1:
			    if(millis()<60000) upload(1);
				break;    // boottimerr
			case 4: upload(3); break; //bootcount
			// case 5: upload(50); break; //buildin led
			// settings
			case 7: upload(2); break;
			case 8: upload(8); break;	// version 

			case 9: upload(VOCHT_HOOG_ID); break;
			case 10: upload(VOCHT_LAAG_ID); break;
			case 11: upload(MANUAL_TIMER_ID); break;
			case 12: upload(DEVIATION_TIMER_ID); break;
			case 13: upload(DEVIATION_ID); break;
			case 14: upload(MAX_DOUCH_TEMP_ID); break;
			case 15: upload(MAX_KRAAN_TEMP_ID); break;

			case 16: upload(FAN_ID); break;

			case 18: upload(LAMP_ID); break;

			case 20: upload(SET_HIGH_DELAY_ID); 	break;
			case 21: upload(SET_MEDIUM_DELAY_ID); 	break;
			case 22: upload(SET_LOW_DELAY_ID); 		break;

			case 23: upload(NIGHTLY_ID); 			break;

			// case 24: upload(REL_HUM_ID); 			break; 
			// case 25: upload(ABS_HUM_ID); 			break; 
			// case 26: upload(HUM_TEMP_ID); 			break; 
			// case 27: upload(PRESS_ID); 			break; 

			case 30: myTimers.timerOff(TIMER_UPLOAD_ON_BOOT); break;			
			}

			uploadOnBootCount++;
			return;
		}

		if( myTimers.isTimerActive(TIMER_UPLOAD_ON_BOOT) ) return;

		if( myTimers.isTime(TIMER_UPLOAD_REL_HUM)) {
			upload(REL_HUM_ID);
			myTimers.nextTimer(TIMER_UPLOAD_REL_HUM );
			return;
		}

		if( myTimers.isTime(TIMER_UPLOAD_ABS_HUM)) {
			upload(ABS_HUM_ID);
			myTimers.nextTimer(TIMER_UPLOAD_ABS_HUM );
			return;
		}

		if( myTimers.isTime(TIMER_UPLOAD_HUM_TEMP)) {
			upload(HUM_TEMP_ID);
			myTimers.nextTimer(TIMER_UPLOAD_HUM_TEMP );
			return;
		}

		if( myTimers.isTime(TIMER_UPLOAD_PRESS)) {
			upload(PRESS_ID);
			myTimers.nextTimer(TIMER_UPLOAD_PRESS );
			return;
		}

		if( myTimers.isTime(TIMER_UPLOAD_FAN)) {
			upload(FAN_ID);
			myTimers.nextTimer(TIMER_UPLOAD_FAN);
			return;
		}
		if( myTimers.isTime(TIMER_LAMP_UPLOAD)) {
			upload(LAMP_ID);
			myTimers.nextTimer(TIMER_LAMP_UPLOAD);
			return;
		}

		if( myTimers.isTime(TIMER_NIGHTLY_UPLOAD)) {
			upload(NIGHTLY_ID);
			myTimers.nextTimer(TIMER_NIGHTLY_UPLOAD);
			return;
		}


 		if( myTimers.isTime(TIMER_SECOND_OF_THE_DAY)){
			parentNode.txCmd('D', parentNode.nodeId, 12);
			myTimers.nextTimer(TIMER_SECOND_OF_THE_DAY, 7);
			return;
		}
	}


	if(myTimers.isTime(TIMER_TEST)){

		// ...

		myTimers.nextTimer(TIMER_TEST, 3);
	}


	#ifdef DEBUG		
		if( myTimers.isTime(TIMER_TRACE)){ trace();}
	#endif
}

#ifdef DEBUG
void trace()
	{

		myTimers.nextTimer(TIMER_TRACE, TRACE_SEC); //TRACE_SEC

	// 	Serial.print(F("@"));
	// 	Serial.print( millis() / 1000 );
	// 	Serial.print (F(" "));

	// //	Serial.print(F(", free="));	Serial.print(System::ramFree());
	// //	Serial.println();

	// 	Serial.print(F("fan: "));	Serial.print(fanSpeed);
	// 	Serial.print(F(", n@"));	Serial.print(myTimers.timers[TIMER_FAN_HIGH ]/1000);
	// 	Serial.print(F("-"));	Serial.print(myTimers.timers[TIMER_FAN_MEDIUM ]/1000);
	// 	Serial.print(F("-"));	Serial.print(myTimers.timers[TIMER_FAN_LOW ]/1000);

	// 	Serial.print(F(", hoog:")); Serial.print( eeParms.vochtHoog );
	// 	Serial.print(F("%, laag:")); Serial.print( eeParms.vochtLaag );
	// 	// Serial.print(F("%, ")); Serial.print( eeParms.maxDouchTemp ); Serial.print(" C");
	// 	// Serial.print(F(", std:"));	Serial.print(statsOnHumidity.std());
	// 	// Serial.print(F(", minDevi:")); Serial.print( eeParms.minDeviation );
	// 	Serial.print(F(", onBt:"));	Serial.print(uploadOnBootCount);
	
 	// 	Serial.print(F(", night:"));	Serial.print(nightly);	
	// 	#ifdef DETECT_230V_PIN
	// 		Serial.print(", 230v:"); Serial.print( lampOn );
	// 		Serial.print("-"); Serial.print( curr230vcount );
	// 	#endif

	// 	// Serial.print(F(", time "));	Serial.print(secOfTheDay/3600);	
	// 	// Serial.print(F(":"));	Serial.print( ( secOfTheDay - ( secOfTheDay/3600) * 3600) / 60);	
 

	// 	Serial.println();	

 
		parentNode.trace("pNd");
		// hubNode.trace("hup");

		// Serial.printf("#txP:%d-%d #txN:%d-%d  \n",parentNode.sendCount, hubNode.receiveCount,hubNode.sendCount, parentNode.receiveCount    );



		#ifdef SPI_CS_BME
			bme.trace("bme");
		#endif

		// digitalWrite(50, !digitalRead(50) );
		// digitalWrite(51, !digitalRead(51) );
		// digitalWrite(52, !digitalRead(52) );
		// digitalWrite(53, !digitalRead(53) );

		// Serial.printf("Core temperature: %2.1fC\n", analogReadTemp());

		#ifdef TEMPS_PIN
		tempDouch.trace("temp");	
		#endif
		// tempAfzuiging.trace("afz");
 
        // one_wire.single_device_read_rom(address);
        // Serial.printf("Device Address: %02x%02x%02x%02x%02x%02x%02x%02x\n", address.rom[0], address.rom[1], address.rom[2], address.rom[3], address.rom[4], address.rom[5], address.rom[6], address.rom[7]);
        // one_wire.convert_temperature(address, true, false);
        // Serial.printf("Temperature: %3.1foC\n", one_wire.temperature(address));


	}
#endif