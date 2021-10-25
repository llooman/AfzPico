void localSetVal(int id, long val)
{
	switch(id )
	{
	case 12:
		secOfTheDay = val;
		secOfTheDay = secOfTheDay % 86400;
		nextSecond = millis()+ 1000;
		myTimers.nextTimer(TIMER_SECOND_OF_THE_DAY, 3600);
		break;

 
 
	// case SET_HIGH_DELAY_ID:
	// 	eeParms.setHighDelay(val);
	// 	resetFan();
	// 	break;

	default:
		eeParms.setVal( id,  val);
		parentNode.setVal( id,  val);
		break;
	}

	if(id==2)
	{
		// tempDouch.setSyncInterval(eeParms.samplePeriode_sec);
		// #ifndef DEBUG
		// 	tempBuiten.setSyncInterval(eeParms.samplePeriode_sec);
		// 	tempKraan.setSyncInterval(eeParms.samplePeriode_sec);
		// #endif
	}
}



void nextUpload(int id){

	switch( id ){
		case FAN_ID: 		myTimers.nextTimer(TIMER_UPLOAD_FAN);		break;
		case LAMP_ID: 		myTimers.nextTimer(TIMER_LAMP_UPLOAD);		break;
		case NIGHTLY_ID: 	myTimers.nextTimer(TIMER_NIGHTLY_UPLOAD);	break;
 
		case REL_HUM_ID:  	myTimers.nextTimer(TIMER_UPLOAD_REL_HUM);	break;
		case HUM_TEMP_ID: 	myTimers.nextTimer(TIMER_UPLOAD_HUM_TEMP);	break;
		case ABS_HUM_ID: 	myTimers.nextTimer(TIMER_UPLOAD_ABS_HUM );	break;
		case PRESS_ID:    	myTimers.nextTimer(TIMER_UPLOAD_PRESS ); 	break;			
	}
}

int upload(int id)
{
	int ret = 0;
	nextUpload(id);

	switch( id )
	{
	case 2:
		upload(id, NODE_TYPE );   
		break;		
	case 8:
		upload(id, JL_VERSION );   
		break;
		
	case 12:
		upload(id,  secOfTheDay );   
		break;

	#ifdef SPI_CS_BME
		case REL_HUM_ID:
			
			if(bme.health > 1 ){
				upload(id, bme.relHum);
				humUploaded = bme.relHum;
			} else {
				myTimers.nextTimer(TIMER_READ_BME, 3);	
			}
			break;

		case HUM_TEMP_ID:
			
			if(bme.health > 1 ){
				upload(id, bme.temperature);
				tempUploaded = bme.temperature;
			}
			break;

		case ABS_HUM_ID:
			if(bme.health > 1 ){
				upload(id, bme.absHum  ); 				
				absHumUploaded = bme.absHum;
			}
			break;

		case PRESS_ID:
			if(bme.health > 1 ){
				upload(id, bme.pressure/100);
				pressUploaded = bme.pressure/100;
			}
			break;
	#endif

	case NIGHTLY_ID:
		upload(id, nightly );   
		break;
 	

	// case FAN_ID:
	// 	upload(id, fanSpeed );  
	// 	break;

	// case LAMP_ID:
	//     if(! parentNode.isTxFull() ){
	// 		upload(id, lampOn);
	// 		lampUploaded = lampOn;
	// 		myTimers.nextTimer(TIMER_LAMP_UPLOAD);
	// 	} else {
	// 		myTimers.nextTimerMillis(TIMER_LAMP_UPLOAD, 100);
	// 	}
	// 	break;

	// case DOUCH_TEMP_ID: tempDouch.upload(); break;
	// #ifndef DEBUG
	// 	case BUITEN_TEMP_ID: tempBuiten.upload(); break;
	// 	case KRAAN_TEMP_ID: tempKraan.upload(); break;
	// #endif
 
	case VOCHT_HOOG_ID:
		upload(id,  eeParms.vochtHoog );
		break;
	case VOCHT_LAAG_ID:
		upload(id,  eeParms.vochtLaag );
		break;
	case MANUAL_TIMER_ID:
		upload(id,  eeParms.manualTimerMin );
		break;
	case DEVIATION_TIMER_ID:
		upload(id,  eeParms.statsTimerMin );
		break;

	case DEVIATION_ID:
		upload(id,  eeParms.minDeviation );
		break;	
 	 
	case SET_HIGH_DELAY_ID:
 		upload(id,  eeParms.highDelayMin );
		break;
	case SET_MEDIUM_DELAY_ID:
 		upload(id,  eeParms.mediumDelayMin );
		break;
	case SET_LOW_DELAY_ID:
 		upload(id,  eeParms.lowDelayMin );
		break;

	// #ifdef VOLTAGE_PIN
	// 	case 11: upload(id, vin.val);    		break;
	// #endif

	default:

		if( 1==2
		 ||	eeParms.upload(id)>0
		 ||	parentNode.upload(id)>0
		){}
		break;
	}
	return ret;
}

int upload(int id, long val) { return upload(id, val, millis()); }
int upload(int id, long val, unsigned long timeStamp)
{
	#ifdef DEBUG
		// Serial.print( "@" );
		// Serial.print( millis() / 1000 );
		// Serial.print ( " upl: " );
 		// Serial.print(id);Serial.print("-");Serial.println(val);
	#endif
 
	nextUpload(id);
	return parentNode.txUpload(id, val, timeStamp);
}


int uploadError(int id, long val)
{
	#ifdef DEBUG
		Serial.print("err-");Serial.print(id);Serial.print(": ");Serial.println(val);
	#endif	
	return parentNode.txError(id, val);
}

int upload2(int id, long val, unsigned long timeStamp ){
	// Serial.print( "@" );
	// Serial.print( millis() / 1000 );
	// Serial.print ( " upl2: " );
	// Serial.print(id);Serial.print("-");Serial.println( val);
	return 0;
}

// int uploadError2(int id, long val)
// {
// 	#ifdef DEBUG
// 		// Serial.print("err-");Serial.print(id);Serial.print(": ");Serial.println(val);
// 	#endif	
// 	// return hubNode.txError(id, val);
// }

int handleParentReq2( RxItem *rxItem)   
{
	 
	// parentNode.debug("Prnt2<", rxItem);
	

 
	return 0;
}



int handleParentReq( RxItem *rxItem)   
{
	#ifdef DEBUG
		// parentNode.debug("Prnt<", rxItem);
	#endif

	if( rxItem->data.msg.node == 2 
	 || rxItem->data.msg.node == parentNode.nodeId
	){
		return localRequest(rxItem);
	}

	if(parentNode.nodeId==0)
	{
		#ifdef DEBUG
			parentNode.debug("skip", rxItem);
		#endif
		return 0;
	}

	// #ifdef DEBUG
	// 	parentNode.debug("forward", rxItem);
	// #endif

	// #ifndef SKIP_CHILD_NODES
	// 	return childNodes.putTxBuf( rxItem );
	// #endif
	return 0;
}


int localRequest(RxItem *rxItem)
{
	#ifdef DEBUG
		parentNode.debug("locReq", rxItem);
	#endif

	int ret=0;
	int subCmd;
	int val;

	switch (  rxItem->data.msg.cmd)
	{
	// case 't':trace(); break;

	case 'L':
		switch (rxItem->data.msg.id)
		{
		case 1:
			break;
		}
		break;

	case 'l':
	    subCmd = rxItem->data.msg.val & 0x000000ff ;
    	val = rxItem->data.msg.val >> 8;
		switch (rxItem->data.msg.id)
		{
		case 1:
			parentNode.localCmd(subCmd, val);
			break;
 		case 2:
			parentNode.localCmd(subCmd+100, val);
			break;
		}
		break; 

	case 's':
	case 'S':
		localSetVal(rxItem->data.msg.id, rxItem->data.msg.val);
		upload(rxItem->data.msg.id);

		break;
	case 'r':
	case 'R':
		upload(rxItem->data.msg.id);
		break;
	case 'b':
		// eeParms.resetBootCount();
 		upload(3);
		break;
	case 'B':
		// wdt_enable(WDTO_15MS);
		// while(true){
		// 	delay(500);
		// 	asm volatile ("  jmp 0");
		// }
		break;
	default:
		// eeParms.handleRequest(rxItem);
 
		break;
	}

	return ret;
}
