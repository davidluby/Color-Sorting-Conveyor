/* color sensor stuff */
int ClrDet(){
	DDRL = 0xFF;
	DDRC = 0xFF;
	DDRD = 0x00;
	
	int MaxCnt = 0;
	int Test_Cnt;
	int ClrFr = 0; //reset color frequency variable
	
	//S2 in pinA1. S3 in pinA0 NOTE: change to another port
	PORTL = 0b00000000; //RED FILTER
	//PORTL = 0b00000001; //blue
	//PORTL = 0b00000010; //clear
	//PORTL = 0b00000011; //green
	
	for (Test_Cnt=0;Test_Cnt<=5;Test_Cnt++)
	{
		while ((PIND & 0b10000000) == 0b10000000);//Falling edge come, start timer counter CHECK AND MAKE SURE CLR SNSR OUTPUT IS PIN7 NOT PIN0
		
		TCNT5 = 0x0000;
		TCCR5B |= _BV(CS50);//start the timer counter
		
		while((PIND & 0b10000000) == 0b00000000);//during the low voltage, do nothing but count
		TCCR5B = 0x0000;//stop the timer counter
		
		if (TCNT5>MaxCnt)
		{MaxCnt = TCNT5;}
		ClrFr =  16000000/MaxCnt/600;//Calculate the Frequency 
			/*DIVISION FACTOR:  [600 for red mode]
								[2000 for clear mode]
								[600 for blue mode]
			*/
	} //end for loop (original end for loop was line 62)
	
	//NOTE: ClrFr = 0bRYGB0000
	
	
	
	//FOR NEW LOGIC
	int ClrFr_BlueFilter = 0;
	int ClrFr_GreenFilter = 0;
	
	//Red/yellow indicator
	if ((ClrFr>=210)) //originally set at 220
	{
		//if ((ClrFr<260)) //originally set at 255
		if ((ClrFr>270))
		{
			for (Test_Cnt=0;Test_Cnt<=5;Test_Cnt++)
			{
				while ((PIND & 0b10000000) == 0b10000000);//Falling edge come, start timer counter CHECK AND MAKE SURE CLR SNSR OUTPUT IS PIN7 NOT PIN0
				
				TCNT5 = 0x0000;
				TCCR5B |= _BV(CS50);//start the timer counter
				
				while((PIND & 0b10000000) == 0b00000000);//during the low voltage, do nothing but count
				TCCR5B = 0x0000;//stop the timer counter
				
				if (TCNT5>MaxCnt)
				{MaxCnt = TCNT5;}
				ClrFr =  16000000/MaxCnt/600;//Calculate the Frequency
			}
			if ((ClrFr>275))	//	265 original value
			{
				//PORTC = ClrFr;
				ClrFr = 0b01000000; //indicate yellow
			}
			else
			{
				//PORTC = ClrFr;
				ClrFr = 0b10000000; //indicate red
			}
						
		}
		else
		{
			//ClrFr = 0b01000000; //indicate yellow
			
			PORTL = 0b00000011; //GREEEN FILTER
			MaxCnt = 0;
			ClrFr = 0; //reset color frequency variable
		 
			for (Test_Cnt=0;Test_Cnt<=5;Test_Cnt++)
			{
				while ((PIND & 0b10000000) == 0b10000000);//Falling edge come, start timer counter CHECK AND MAKE SURE CLR SNSR OUTPUT IS PIN7 NOT PIN0
	
				TCNT5 = 0x0000;
				TCCR5B |= _BV(CS50);//start the timer counter
		
				while((PIND & 0b10000000) == 0b00000000);//during the low voltage, do nothing but count
				TCCR5B = 0x0000;//stop the timer counter
		
				if (TCNT5>MaxCnt)
				{MaxCnt = TCNT5;}
				ClrFr =  16000000/MaxCnt/800;//Calculate the Frequency 
						/*DIVISION FACTOR:  [600 for red mode]
											[2000 for clear mode]
											[600 for blue mode]
											[800 for green mode]
						*/					
			} //end for loop (original end for loop was line 62)
			
			if(ClrFr>=120)
			{
				//PORTC = ClrFr;
				ClrFr = 0b00100000; //indicate green
			}
			else
			{
				//PORTC = ClrFr;
				ClrFr = 0b10000000; //indicate red
			}
		} 
	}
	
	
	

	//blue/green indicator
	else
	{
		PORTL = 0b00000001; //BLUE FILTER
		MaxCnt = 0;
		ClrFr = 0; //reset color frequency variable
		 
		for (Test_Cnt=0;Test_Cnt<=5;Test_Cnt++)
		{
			while ((PIND & 0b10000000) == 0b10000000);//Falling edge come, start timer counter CHECK AND MAKE SURE CLR SNSR OUTPUT IS PIN7 NOT PIN0
	
			TCNT5 = 0x0000;
			TCCR5B |= _BV(CS50);//start the timer counter
		
			while((PIND & 0b10000000) == 0b00000000);//during the low voltage, do nothing but count
			TCCR5B = 0x0000;//stop the timer counter
		
			if (TCNT5>MaxCnt)
			{MaxCnt = TCNT5;}
			ClrFr =  16000000/MaxCnt/600;//Calculate the Frequency 
					/*DIVISION FACTOR:  [600 for red mode]
										[2000 for clear mode]
										[600 for blue mode]
					*/
			int ClrFr_BlueFilter = ClrFr; //FOR NEW LOGIC					
		} //end for loop (original end for loop was line 62)
		if ((ClrFr<222)) //222 worked well
		{
			PORTL = 0b00000011; //GREEN FILTER
			MaxCnt = 0;
			ClrFr = 0; //reset color frequency variable
		 
			for (Test_Cnt=0;Test_Cnt<=5;Test_Cnt++)
			{
				while ((PIND & 0b10000000) == 0b10000000);//Falling edge come, start timer counter CHECK AND MAKE SURE CLR SNSR OUTPUT IS PIN7 NOT PIN0
	
				TCNT5 = 0x0000;
				TCCR5B |= _BV(CS50);//start the timer counter
		
				while((PIND & 0b10000000) == 0b00000000);//during the low voltage, do nothing but count
				TCCR5B = 0x0000;//stop the timer counter
		
				if (TCNT5>MaxCnt)
				{MaxCnt = TCNT5;}
				ClrFr =  16000000/MaxCnt/600;//Calculate the Frequency 
						/*DIVISION FACTOR:  [600 for red mode]
											[2000 for clear mode]
											[600 for blue mode]
						*/
				ClrFr_GreenFilter = ClrFr; //FOR NEW LOGIC
			}
			if (ClrFr_GreenFilter > ClrFr_BlueFilter)
			{
				//PORTC = ClrFr;
				ClrFr = 0b00100000; //indicate green
			}
			else
			{
				//PORTC = ClrFr;
				ClrFr = 0b00010000; //indicate blue
			}
			/*
			PORTC = ClrFr;
			ClrFr = 0b00100000; //indicate green
			*/
		}
		else
		{
			//PORTC = ClrFr;
			ClrFr = 0b00010000; //indicate blue
		} 
	}
	return(ClrFr);
	}
	// end color sensor stuff