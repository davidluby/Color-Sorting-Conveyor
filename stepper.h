///////////////////////////////////////
//STEPPER MOTOR RELATED SUB-FUNCTIONS//
//////////////////////////////////////

void mTimer(int count);

int StepperMove90CW(int CurStep){
	int i;
	int StepArray[4] = {0b00101011, 0b00101101, 0b00110101, 0b00110011};
	for(i=0;i<50;i++){
		CurStep ++;
		if (CurStep > 3){
			CurStep = 0;
		}
		PORTA = StepArray[CurStep];
		mTimer(11);
	}
	return(CurStep);
}
// turn the stepper 90 degrees in CCW
int StepperMove90CCW(int CurStep){
	int i;
	int StepArray[4] = {0b00101011, 0b00101101, 0b00110101, 0b00110011};
	for(i=0;i<50;i++){
		CurStep --;
		if (CurStep < 0){
			CurStep = 3;
		}
		PORTA = StepArray[CurStep];
		mTimer(11);
	}
	return(CurStep);
}

// turn the stepper 180 degrees in CW
int StepperMove180CW(int CurStep){
	int i;
	int StepArray[4] = {0b00101011, 0b00101101, 0b00110101, 0b00110011};
	for(i=0;i<100;i++){
		CurStep ++;
		if (CurStep > 3){
			CurStep = 0;
		}
		PORTA = StepArray[CurStep];
		mTimer(11);
	}
	return(CurStep);
}

int stepColor(int incomingClr, int currentClr, int CurStep){
	int stepDelay = 11;
	
	//Calculate rotation variable
	int rotateVar;
	rotateVar = incomingClr - currentClr;

	//Calculate necessary rotation
	switch(rotateVar) {

		case 1  :
		//CCW 90 degree turn
		CurStep = StepperMove90CCW(CurStep);
		break;
		
		case -3  :
		//CCW 90 degree turn
		CurStep = StepperMove90CCW(CurStep);
		break;
		
		case -1  :
		//CW 90 degree turn
		CurStep = StepperMove90CW(CurStep);
		break;
		
		case 3  :
		//CW 90 degree turn
		CurStep = StepperMove90CW(CurStep);
		break;
		
		case 2  :
		//CCW 180 degree turn
		CurStep = StepperMove180CW(CurStep);
		break;
		
		case -2  :
		//CCW 180 degree turn
		CurStep = StepperMove180CW(CurStep);
		break;
	}
	return(CurStep);
}//end Step Color Function