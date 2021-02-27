#define delayTime 5
class stepper{
	public:
		stepper();
		stepper(int in1, int in2, int in3, int in4);	
		void rotate(int step_to_move);
		void one_step();
		void stop();
	private:

		int stepper_state;
		int motorPin1;
		int motorPin2;
		int motorPin3;
		int motorPin4;

};

stepper::stepper(int in1, int in2, int in3, int in4){
	this->motorPin1 = in1;
	this->motorPin2 = in2;
	this->motorPin3 = in3;
	this->motorPin4 = in4;
	stepper_state = 0;
}
void stepper::stop(){
	digitalWrite(motorPin1, LOW);
	digitalWrite(motorPin2, LOW);
	digitalWrite(motorPin3, LOW);
	digitalWrite(motorPin4, LOW);
}
void stepper::one_step(){	
	
    switch(this->stepper_state){
	    case 0:
    	    digitalWrite(motorPin1, HIGH);
	        digitalWrite(motorPin2, LOW);
   		    digitalWrite(motorPin3, LOW);
   	    	digitalWrite(motorPin4, LOW);
        	delay(delayTime);
        	break;
        case 1:
        	digitalWrite(motorPin1, LOW);
        	digitalWrite(motorPin2, HIGH);
        	digitalWrite(motorPin3, LOW);
        	digitalWrite(motorPin4, LOW);
        	delay(delayTime);
        	break;
        case 2:
        	digitalWrite(motorPin1, LOW);
        	digitalWrite(motorPin2, LOW);
        	digitalWrite(motorPin3, HIGH);
        	digitalWrite(motorPin4, LOW);
        	delay(delayTime);
        	break;
        case 3:
        	digitalWrite(motorPin1, LOW);
        	digitalWrite(motorPin2, LOW);
        	digitalWrite(motorPin3, LOW);
        	digitalWrite(motorPin4, HIGH);
        	delay(delayTime);
        	break;
      }
    
}
void stepper::rotate(int step_to_move){
	bool direction = false;// false for clockwise   true for counterclockwise
	int i = 0;
	if(step_to_move < 0) direction = true;
	while(i < abs(step_to_move)){
		if(!direction){
			if(stepper_state == 4){
				stepper_state = 0;
			}
			else stepper_state++;
			one_step();
			i++;
		}
		else{
			if(stepper_state == 0){
				stepper_state = 4;
			}
			else stepper_state--;
			one_step();
			i++;
		}
	}
}
