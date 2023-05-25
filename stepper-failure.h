///////////////////////////////////////
//STEPPER MOTOR RELATED SUB-FUNCTIONS//
//////////////////////////////////////

void mTimer(int count);

typedef struct {
	int step;
	int loc;
	} out;

//CW 90 degree turn function
out stepCW(int stepDelay, int step, int loc){
	out stpL;
	
	int i = 0;
	
	if (loc == 1){
		if (step == 4){
			step = 1;
		}
		else{
			step = step + 1;
			}
	}
	
	while (i < 50){
		if ((step != 5)){
			switch(step){
				case(1):
					PORTA = 0b00101011; //step 1
					mTimer(stepDelay);
					stpL.step = step;
					step++;
					i++;
					break;
				case(2):
					PORTA = 0b00101101; //step 2
					mTimer(stepDelay);
					stpL.step = step;
					step++;
					i++;
					break;
				case(3):
					PORTA = 0b00110101; //step 3
					mTimer(stepDelay);
					stpL.step = step;
					step++;
					i++;
					break;
				case(4):
					PORTA = 0b00110011; //step 4
					mTimer(stepDelay);
					stpL.step = step;
					step++;
					i++;
					break;
			}
		}
		else{
			step = 1;
		}
	}
	stpL.loc = 0;	//	location 0 is from cw
	return(stpL);
}//end CW 90 degree turn function

//CCW 90 degree turn function
out stepCCW(int stepDelay, int step, int loc){
	out stpL;
	int i = 0;
	
	if (loc == 0){
		if (step == 1){
			step = 4;
		}
		else{
			step = step + 1;
		}
	}
	
	while (i < 50){
		if ((step != 0)){
			switch(step){
				case(4):
					PORTA = 0b00110011; //step 4
					mTimer(stepDelay);
					stpL.step = step;
					step--;
					i++;
					break;
				case(3):
					PORTA = 0b00110101; //step 3
					mTimer(stepDelay);
					stpL.step = step;
					step--;
					i++;
					break;
				case(2):
					PORTA = 0b00101101; //step 2
					mTimer(stepDelay);
					stpL.step = step;
					step--;
					i++;
					break;
				case(1):
					PORTA = 0b00101011; //step 1
					mTimer(stepDelay);
					stpL.step = step;
					step--;
					i++;
					break;
			}
		}
		else{
			step = 4;
		}
	}
	stpL.loc = 1;	//	location 1 is from ccw
	return(stpL);
}

//CCW 180 degree turn function (opposite side turn)
out stepOpposite(int stepDelay, int step, int loc){
	out stpL;
	stpL = stepCCW(stepDelay, step, loc);
	stpL = stepCCW(stepDelay, step, loc);
	return (stpL);
}//end CCW 180 degree turn function (opposite side turn)

//Step Color Function
out stepColor(int incomingClr, int currentClr, int step, int loc){
	out stpL;
	int stepDelay = 11;
	
	//Calculate rotation variable
	int rotateVar;
	rotateVar = incomingClr - currentClr;

	//Calculate necessary rotation
	switch(rotateVar) {

		case 1  :
		//CCW 90 degree turn
		stpL = stepCCW(stepDelay, step, loc);
		break;
	
		case -3  :
		//CCW 90 degree turn
		stpL = stepCCW(stepDelay, step, loc);
		break;
	
		case -1  :
		//CW 90 degree turn
		stpL = stepCW(stepDelay, step, loc);
		break;
	
		case 3  :
		//CW 90 degree turn
		stpL = stepCW(stepDelay, step, loc);
		break;
	
		case 2  :
		//CCW 180 degree turn
		stpL = stepOpposite(stepDelay, step, loc);
		break;
	
		case -2  :
		//CCW 180 degree turn
		stpL  = stepOpposite(stepDelay, step, loc);
		break;
	}
	return(stpL);	
}//end Step Color Function
