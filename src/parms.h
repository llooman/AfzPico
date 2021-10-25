class EEParms: public EEUtil  // TODO
{
public:
	int				maxDouchTemp = 47;		 
	int				maxKraanTemp = 47;	

	volatile int  	vochtHoog = 90;
	volatile int  	vochtLaag = 75;
	
	volatile int  	highDelayMin = 3;	
	volatile int  	mediumDelayMin = 5;	
	volatile int  	lowDelayMin = 7;	

	volatile int  	manualTimerMin = 5;	
	volatile int    statsTimerMin = 2;

	volatile int    minDeviation = 9;

	long chk1  = 0x04010204;   

	virtual ~EEParms(){}
	EEParms(){ }

    void setup( ) //class HomeNet &netw
    {

		if( readLong(offsetof(EEParms, chk1)) == chk1  )
    	{
    		readAll();
    		vochtHoog = readInt(offsetof(EEParms, vochtHoog));
    		vochtLaag = readInt(offsetof(EEParms, vochtLaag));

    		highDelayMin = readInt(offsetof(EEParms, highDelayMin));
    		mediumDelayMin = readInt(offsetof(EEParms, mediumDelayMin));
    		lowDelayMin = readInt(offsetof(EEParms, lowDelayMin));

    		manualTimerMin = readInt(offsetof(EEParms, manualTimerMin));
			statsTimerMin = readInt(offsetof(EEParms, statsTimerMin));
			minDeviation = readInt(offsetof(EEParms, minDeviation));
 
    		// maxDouchTemp = readInt(offsetof(EEParms, maxDouchTemp));
    		// maxKraanTemp = readInt(offsetof(EEParms, maxKraanTemp));									

    		changed = false;

//    		resync();
			#ifdef DEBUG
				Serial.println(F("EEProm.readAll"));
			#endif
    	}
		else
		{
			bootCount=0;
		}
    }

	void loop()
	{
		if(changed)
		{
			#ifdef DEBUG
				Serial.println(F("Parms.loop.changed"));
			#endif
			write(offsetof(EEParms, vochtHoog), vochtHoog);
			write(offsetof(EEParms, vochtLaag), vochtLaag);

			write(offsetof(EEParms, highDelayMin), highDelayMin);
			write(offsetof(EEParms, mediumDelayMin), mediumDelayMin);
			write(offsetof(EEParms, lowDelayMin), lowDelayMin);			

			write(offsetof(EEParms, manualTimerMin), manualTimerMin);
			write(offsetof(EEParms, statsTimerMin), statsTimerMin);
			write(offsetof(EEParms, minDeviation), minDeviation);
 
			// write(offsetof(EEParms, maxDouchTemp), maxDouchTemp);
			// write(offsetof(EEParms, maxKraanTemp), maxKraanTemp);

			write(offsetof(EEParms, chk1), chk1);
			EEUtil::writeAll();
			changed = false;
		}
	}

	void setHighDelay( long newVal)
	{
		if( newVal < 0 ) newVal = 0;
		if( newVal > 30 ) newVal = 30;

		highDelayMin =  (int)newVal;
		changed = true;
	}

	void setMediumDelay( long newVal)
	{
		if( newVal < 0 ) newVal = 0;
		if( newVal > 30 ) newVal = 30;

		mediumDelayMin =  (int)newVal;
		changed = true;
	}

	void setLowDelay( long newVal)
	{
		if( newVal < 0 ) newVal = 0;
		if( newVal > 30 ) newVal = 30;

		lowDelayMin =  (int)newVal;
		changed = true;
	}	

	void setStatsTimer( long newVal)
	{
		if( newVal < 1 ) newVal = 1;
		if( newVal > 5 ) newVal = 5;

		statsTimerMin =  (int)newVal;
		changed = true;
	}
	void setManualTimer( long newVal)
	{
		if( newVal < 0 ) newVal = 0;
		if( newVal > 30 ) newVal = 30;

		manualTimerMin =  (int)newVal;
		changed = true;
	}
	void setMinDeviation( long newVal)
	{
		if( newVal < 5 ) newVal = 5;
		if( newVal > 300 ) newVal = 300;

		minDeviation =  (int)newVal;
		changed = true;
	}
	
 
	// void setDouchTemp( long newVal)
	// {
	// 	if( newVal < 20 ) newVal = 20;
	// 	if( newVal > 70 ) newVal = 70;

	// 	maxDouchTemp =  (int)newVal;
	// 	changed = true;
	// }	
	// void setKraanTemp( long newVal)
	// {
	// 	if( newVal < 20 ) newVal = 20;
	// 	if( newVal > 70 ) newVal = 70;

	// 	maxKraanTemp =  (int)newVal;
	// 	changed = true;
	// }	


	void setVochtHoog( long newVal)
	{
		if( newVal < 40 ) newVal = 40;
		if( newVal > 99 ) newVal = 99;

		vochtHoog =  (int)newVal;

		if(vochtLaag > (vochtHoog - 3))  vochtLaag = vochtHoog - 3;
		 
		changed = true;
	}
	void setVochtLaag( long newVal)
	{
		if( newVal < 40 )				newVal = 40;
		if( newVal > ( vochtHoog - 3)) 	newVal =( vochtHoog - 3);

		vochtLaag =  (int)newVal;
		changed = true;
	}
};
