#include "MagRotaryEncoding.h"

/*
This library was written based on the rotation detection method described @ ""
This library also features a haptics controller, that allows you to connect and control a vibration motor as a step count haptic feedback.arduino

....Written By Ahmed Oyenuga (Ahmsville 2019).
*/


volatile bool sleepmode = false;

bool MagRotaryEncoder::passSynctest(int newspos)
{
	if (encoderResolution > 0)
	{
		if (startposition == 1)
		{
			if (newspos == 4 || newspos == 2)
			{
				return true;
			}
		}
		else if (startposition == 2)
		{
			if (newspos == 1 || newspos == 3)
			{
				return true;
			}
		}
		else if (startposition == 3)
		{
			if (newspos == 2 || newspos == 4)
			{
				return true;
			}
		}
		else if (startposition == 4)
		{
			if (newspos == 3 || newspos == 1)
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		return true;
	}
}


int MagRotaryEncoder::sensor1_INT() {
	if (!use_extended_resolution) { //extended res is not used
		get_sensorValue(2); //read sensor 2

		//set new startposition
		if (sensor2 != 0) { //value read successfully, process here

			startposition = get_encodingState(1);
			if (startposition == prevstartposition)
			{
				activesensorINT = 2;
				INTProcessed = false;
				return 2;
			}
			prevsensor1 = Mid;
			INTProcessed = true;
			//keep count

			rotationrate = millis() - timetracker;  //rotation step rate
			if (startposition == 2) { //new startposition is not == 1 or 4
				if (prevstartposition == 1) { //clockwise
					count++;
				}
				else if (prevstartposition == 3) { //counterclockwise
					count--;
				}
				prevstartposition = startposition;
			}

			else if (startposition == 4) { // new startposition == 4
				if (prevstartposition == 3) { //clockwise
					count++;
				}
				else if (prevstartposition == 1) { //counterclockwise
					count--;
				}
				prevstartposition = startposition;
			}
			prevstartposition = startposition;
			timetracker = millis();
			//SerialUSB.println(startposition);
			/*
			SerialUSB.print("fired1");
			SerialUSB.print("\t");
			SerialUSB.print(count);
			SerialUSB.print("\t");
			SerialUSB.print(sensor2);
			*/
			activesensorINT = 2;
			return 2;
		}
		else { //process in loop
			INTProcessed = false;
			INT1fired = true;
			INT2fired = false;
			rotationrate = millis() - timetracker;  //rotation step rate
			timetracker = millis();
			activesensorINT = 2;
			return 2;
		}
	}
	else {
		INT1fired = true;
		INT2fired = false;
	}
	activesensorINT = 2;
	return 2;
}

int MagRotaryEncoder::sensor2_INT() {;
	if (!use_extended_resolution) { //extended res is not used
		get_sensorValue(1); //read sensor 1

		//set new startposition
		if (sensor1 != 0) { //value read successfully, process here

			startposition = get_encodingState(2);
			if (startposition == prevstartposition)
			{
				activesensorINT = 1;
				INTProcessed = false;
				return 1;
			}
			prevsensor2 = Mid;
			INTProcessed = true;

			//keep count

			rotationrate = millis() - timetracker;  //rotation step rate
			if (startposition == 3) { //new startposition is not == 1 or 4
				if (prevstartposition == 2) { //clockwise
					count++;
				}
				else if (prevstartposition == 4) { //counterclockwise
					count--;
				}
				prevstartposition = startposition;
			}
			else if (startposition == 1) { // new startposition == 1
				if (prevstartposition == 4) { //clockwise
					count++;
				}
				else if (prevstartposition == 2) { //counterclockwise
					count--;
				}
				prevstartposition = startposition;
			}
			prevstartposition = startposition;
			timetracker = millis();
			//SerialUSB.println(startposition);
			/*
			SerialUSB.print("fired2");
			SerialUSB.print("\t");
			SerialUSB.print(count);
			SerialUSB.print("\t");
			SerialUSB.print(sensor1);
			*/
			activesensorINT = 1;
			return 1;
		}
		else { //process in loop
			INTProcessed = false;
			INT2fired = true;
			INT1fired = false;
			rotationrate = millis() - timetracker;  //rotation step rate
			timetracker = millis();
			activesensorINT = 1;
			return 1;
		}
	}
	else {
		INT2fired = true;
		INT1fired = false;
	}
	activesensorINT = 1;
	return 1;
}

int MagRotaryEncoder::getActiveSensorInterrupt() {
	return activesensorINT;
}




MagRotaryEncoder::MagRotaryEncoder(int s1, int s2) {  //set sensor pins
	sensor1_pin = s1;
	//pinMode(s1, INPUT);
	sensor2_pin = s2;
	//pinMode(s2, INPUT);
	useInterrupt = false;

}
MagRotaryEncoder::MagRotaryEncoder(int s1, int s2, int s1INTpin, int s2INTpin) {  //set sensor pins
	sensor1_pin = s1;
	//pinMode(s1, INPUT);
	sensor2_pin = s2;
	//pinMode(s2, INPUT);
	SensorINTpin[0] = s1INTpin;
	//pinMode(s1INTpin, INPUT);
	SensorINTpin[1] = s2INTpin;
	//pinMode(s2INTpin, INPUT);
	useInterrupt = true;

}

MagRotaryEncoder::MagRotaryEncoder(int s1, int s2, int s1INTpin, int s2INTpin, bool usefalling) {  //set sensor pins
	sensor1_pin = s1;
	//pinMode(s1, INPUT);
	sensor2_pin = s2;
	//pinMode(s2, INPUT);
	SensorINTpin[0] = s1INTpin;
	//pinMode(s1INTpin, INPUT);
	SensorINTpin[1] = s2INTpin;
	//pinMode(s2INTpin, INPUT);
	useInterrupt = true;
	useFallingEdge = usefalling;
}


void MagRotaryEncoder::setResistorDivider(float R1, float R2, float vcc) {
	float newR2 = R1;
	float newR1 = R1 + R2;
	
	Neutral[0] = (1023 / vcc) * (vcc * newR2 / (newR1 + newR2));

	newR1 = R1;
	newR2 = R1 + R2;

	Neutral[1] = (1023 / vcc) * (vcc * newR2 / (newR1 + newR2));
	bound = Neutral[1] - (ADCMaxRes / 2);

	initialize_encoder();
}

int MagRotaryEncoder::get_sensorINTpin(int sensornum) {
	if (sensornum == 1) {
		return SensorINTpin[0];
	}
	else if (sensornum == 2) {
		return SensorINTpin[1];
	}
}

void MagRotaryEncoder::set_adcresolution(int res) {
	if (res == 12)
	{
		ADCMaxRes = 4095;
		absoluteNeutral = 2048;
		bound = 20;
		maxsway = 252;
		adcVal = ADCRead(12);
	}
	else if (res == 10) {
		ADCMaxRes = 1023;
		absoluteNeutral = 512;
		bound = 10;
		maxsway = 50;
		adcVal = ADCRead(10);
	}
	initialize_encoder();
	
}

void MagRotaryEncoder::invertCount(bool inv) {
	invertcount = inv;
}

void MagRotaryEncoder::set_poleStateValues(int sensornum, int np, int nu, int sp) {  //set ADC values for the poles (northpole, neutral, southpole)
	if (sensornum == 1)
	{
		S1_Neutral[0] = nu - bound;
		S1_Neutral[1] = nu + bound;
		S1_North = S1_Neutral[0];
		S1_South = S1_Neutral[1];
		S1_absoluteneutral = nu;
	}
	if (sensornum == 2)
	{
		S2_Neutral[0] = nu - bound;
		S2_Neutral[1] = nu + bound;
		S2_North = S2_Neutral[0];
		S2_South = S2_Neutral[1];
		S2_absoluteneutral = nu;
	}

	southRegion = Neutral[1] + stablerange;
	northRegion = Neutral[0] - stablerange;
	Neutral[0] = nu - bound;
	Neutral[1] = nu + bound;
	Mid = nu;

}

void MagRotaryEncoder::initialize_encoder() {   //initialize encoder
	set_poleStateValues(1, absoluteNeutral - bound - stepres, absoluteNeutral, absoluteNeutral + bound + stepres); // set the peak ADC values for the (sensor1, northpole, neutralstate, southpole)
	set_poleStateValues(2, absoluteNeutral - bound - stepres, absoluteNeutral, absoluteNeutral + bound + stepres); // set the peak ADC values for the (sensor2, northpole, neutralstate, southpole)
	get_sensorValue(1);
	get_sensorValue(2);
	recaliberate_startPosition();
	prevsensor1 = sensor1;
	prevsensor2 = sensor2;

}

int MagRotaryEncoder::CalibrateSensors(int snum)
{
	int slevel = 0;
	locksensorlevels = true;
	if (snum == 1)
	{
		for (size_t i = 0; i < 10; i++)
		{
			slevel += adcVal.Read(sensor1_pin);
		}
		S1_absoluteneutral = slevel / 10;
		S1_Neutral[0] = S1_absoluteneutral - bound;
		S1_Neutral[1] = S1_absoluteneutral + bound;
		return S1_absoluteneutral;
	}
	else if (snum == 2)
	{
		for (size_t i = 0; i < 10; i++)
		{
			slevel += adcVal.Read(sensor2_pin);
		}
		S2_absoluteneutral = slevel / 10;
		S2_Neutral[0] = S2_absoluteneutral - bound;
		S2_Neutral[1] = S2_absoluteneutral + bound;
		return S2_absoluteneutral;
	}
}
bool MagRotaryEncoder::LoadCalibrationData(int s1, int s2)
{
	//printf("s1 %d s2 %d \n",s1,s2);
	S1_absoluteneutral = s1;
	S1_Neutral[0] = S1_absoluteneutral - bound;
	S1_Neutral[1] = S1_absoluteneutral + bound;

	S2_absoluteneutral = s2;
	S2_Neutral[0] = S2_absoluteneutral - bound;
	S2_Neutral[1] = S2_absoluteneutral + bound;

	locksensorlevels = true;
	return true;
}

void MagRotaryEncoder::recaliberate_startPosition() {  //sets the start position based on the ADC values
	haptics(0);
	if (!useInterrupt) { //interrupt detection is not used
		startposition = get_encodingState();
		if (prevstartposition != startposition) { //if startposition didnt change
			if (startposition == 1 || startposition == 3)
			{
				prevsensor2 = Mid;
			}
			else if (startposition == 2 || startposition == 4)
			{
				prevsensor1 = Mid;
			}
			prevstartposition = startposition;
		}


		count = 0;

	}
	else {
		if (use_extended_resolution) { //extended res and interrupt is in use, update sensor values from temp values
			if (INT1fired) {
				get_sensorValue(2); //read sensor 2
				//set new startposition
				if (sensor2 > S2_Neutral[1]) { //sensor1 = neutral , sensor2 = south
					startposition = 2;
					prevsensor1 = Mid;
				}
				else if (sensor2 < S2_Neutral[0]) { //sensor1 = neutral , sensor2 = north
					startposition = 4;
					prevsensor1 = Mid;
				}
				INT1fired = false;
			}
			else if (INT2fired) {
				get_sensorValue(1); //read sensor 1
				//set new startposition
				if (sensor1 > S1_Neutral[1]) { //sensor2 = neutral , sensor1 = south
					startposition = 3;
					prevsensor2 = Mid;
				}
				else if (sensor1 < S1_Neutral[0]) { //sensor2 = neutral , sensor1 = north
					startposition = 1;
					prevsensor2 = Mid;
				}
				INT2fired = false;
			}

		}
		else if (startposition == 0) { //STARTING
			get_sensorValue(1); //read sensor 1
			get_sensorValue(2); //read sensor 2
			int comp1 = sensor1 - absoluteNeutral;
			int comp2 = sensor2 - absoluteNeutral;
			if (comp1 < 0)
			{
				comp1 = comp1 * (-1);
			}
			if (comp2 < 0)
			{
				comp2 = comp2 * (-1);
			}
			if (comp1 < comp2)//sensor1 is in neutral
			{
				//set new startposition
				if (sensor2 > Neutral[1]) { //sensor1 = neutral , sensor2 = south
					startposition = 2;
					prevstartposition = 2;
					prevsensor1 = Mid;
				}
				else if (sensor2 < Neutral[0]) { //sensor1 = neutral , sensor2 = north
					startposition = 4;
					prevstartposition = 4;
					prevsensor1 = Mid;
				}
			}
			else if (comp2 < comp1) //sensor2 is in neutral
			{
				//set new startposition
				if (sensor1 > Neutral[1]) { //sensor2 = neutral , sensor1 = south
					startposition = 3;
					prevstartposition = 3;
					prevsensor2 = Mid;
				}
				else if (sensor1 < Neutral[0]) { //sensor2 = neutral , sensor1 = north
					startposition = 1;
					prevstartposition = 1;
					prevsensor2 = Mid;
				}
			}
			//SerialUSB.println(startposition);
		}
	}




	
}

void MagRotaryEncoder::DebugEncoder() {
	/*SerialUSB.print(sensor1);
	SerialUSB.print("\t");
	SerialUSB.print(sensor2);
	SerialUSB.print("\t");
	SerialUSB.print(S1_Neutral[0]);
	SerialUSB.print("\t");
	SerialUSB.print(S1_Neutral[1]);
	SerialUSB.print("\t");
	SerialUSB.print(S2_Neutral[0]);
	SerialUSB.print("\t");
	SerialUSB.print(S2_Neutral[1]);
	SerialUSB.print("\t");
	SerialUSB.print(S1_absoluteneutral);
	SerialUSB.print("\t");
	SerialUSB.print(S2_absoluteneutral);
	SerialUSB.print("\t");
	SerialUSB.print(Neutral[0]);
	SerialUSB.print("\t");
	SerialUSB.print(Neutral[1]);
	SerialUSB.print("\t");
	SerialUSB.print(Mid);
	SerialUSB.print("\t");
	SerialUSB.print(setresolution);
	SerialUSB.print("\t");
	SerialUSB.print(bound);
	SerialUSB.print("\t");
	SerialUSB.print(maxsway);
	SerialUSB.print("\t");
	SerialUSB.println(startposition);*/


}



int MagRotaryEncoder::get_sensorValue(int sensornum) {
	if (sensornum == 1)
	{
		
		sensor1 = adcVal.Read(sensor1_pin);
		//dynamic sensor value update
		if (!locksensorlevels)
		{
			if (sensor1 > S1_South || sensor1 < S1_North)
			{
				sensor2 = adcVal.Read(sensor2_pin);
				if (sensor2 < absoluteNeutral + maxsway && sensor2 > absoluteNeutral - maxsway)
				{
					if (sensor1 > S1_South)
					{
						S1_South = sensor1;
					}
					else if (sensor1 < S1_North)
					{
						S1_North = sensor1;
					}
					S2_absoluteneutral = sensor2;
					S2_Neutral[0] = S2_absoluteneutral - bound;
					S2_Neutral[1] = S2_absoluteneutral + bound;
				}
			}
		}

		if (use_extended_resolution)
		{
			sensor1 = (sensor1 * alpha) + (prevsmoothsensor1 * (1 - alpha));
			prevsmoothsensor1 = sensor1;
		}
		return sensor1;
	}
	else if (sensornum == 2)
	{
		sensor2 = adcVal.Read(sensor2_pin);
		//dynamic sensor value update
		if (!locksensorlevels)
		{
			if (sensor2 > S2_South || sensor2 < S2_North)
			{
				sensor1 = adcVal.Read(sensor1_pin);
				if (sensor1 < absoluteNeutral + maxsway && sensor1 > absoluteNeutral - maxsway)
				{
					if (sensor2 > S2_South)
					{
						S2_South = sensor2;
					}
					else if (sensor2 < S2_North)
					{
						S2_North = sensor2;
					}
					S1_absoluteneutral = sensor1;
					S1_Neutral[0] = S1_absoluteneutral - bound;
					S1_Neutral[1] = S1_absoluteneutral + bound;
				}
			}
		}

		if (use_extended_resolution)
		{
			sensor2 = (sensor2 * alpha) + (prevsmoothsensor2 * (1 - alpha));
			prevsmoothsensor2 = sensor2;
		}
		return sensor2;
	}
}

int MagRotaryEncoder::get_sensorValue(int sensornum, bool ret) {
	if (sensornum == 1) {
		return adcVal.Read(sensor1_pin);
	}
	else if (sensornum == 2) {
		return adcVal.Read(sensor2_pin);
	}
}


int MagRotaryEncoder::get_encodingState() {
	
	if (sensor2 >= S2_Neutral[0] && sensor2 <= S2_Neutral[1]) {  //sensor1 = north , sensor2 = neutral
		if (sensor1 < S1_Neutral[0])
		{
			return 1;
		}
		else if (sensor1 > S1_Neutral[1])
		{
			return 3;
		}
		
	}
	else if (sensor1 >= S1_Neutral[0] && sensor1 <= S1_Neutral[1]) { //sensor1 = neutral , sensor2 = south
		if (sensor2 > S2_Neutral[1])
		{
			return 2;
		}
		else if (sensor2 < S2_Neutral[0])
		{
			return 4;
		}
		
	}
	else {
		return startposition;
	}
		//if (sensor1 < Neutral[0] && sensor2 >= (Neutral[0]) && sensor2 <= (Neutral[1])) {  //sensor1 = north , sensor2 = neutral
		//	return 1;
		//}
		//else if (sensor1 >= (Neutral[0]) && sensor1 <= (Neutral[1]) && sensor2 > Neutral[1]) { //sensor1 = neutral , sensor2 = south
		//	return 2;
		//}
		//else if (sensor1 > Neutral[1] && sensor2 >= (Neutral[0]) && sensor2 <= (Neutral[1])) { //sensor1 = south , sensor2 = neutral
		//	return 3;
		//}
		//else if (sensor1 >= (Neutral[0]) && sensor1 <= (Neutral[1]) && sensor2 < Neutral[0]) { //sensor1 = neutral , sensor2 = north
		//	return 4;
		//}
	
		//return startposition;
	
}

int MagRotaryEncoder::get_encodingState(int incenter) {

	if (digitalRead(SensorINTpin[0]) != digitalRead(SensorINTpin[1]))
	{
		if (incenter == 1)
		{
			if (sensor2 > S2_Neutral[1]) { //sensor1 = neutral , sensor2 = south
				return 2;
			}

			else if (sensor2 < S2_Neutral[0]) { //sensor1 = neutral , sensor2 = north
				return 4;
			}
		}
		else if (incenter == 2) {
			if (sensor1 > S1_Neutral[1]) { //sensor2 = neutral , sensor1 = south
				return 3;
			}

			else if (sensor1 < S1_Neutral[0]) { //sensor2 = neutral , sensor1 = north
				return 1;
			}
		}
	}
		
		
		
	return startposition;
		
		
	
	
	
	
}

int MagRotaryEncoder::get_currentSensorValue(int sensornum) {
	if (sensornum == 1) {
		return sensor1;
	}
	else if (sensornum == 2) {
		return sensor2;
	}
}

int MagRotaryEncoder::get_encResCount(int retstep) {
	//keep counter value in the encoder's resolution range
	if (retstep < 0) {
		for (int i = 0; i > retstep; i--) {
			if ((encoderPosition - 1) >= 1) {
				encoderPosition = encoderPosition - 1;
			}
			else if ((encoderPosition - 1) < 1) {
				encoderPosition = encoderResolution;
			}
		}
	}
	else if (retstep > 0) {
		for (int i = 0; i < retstep; i++) {
			if ((encoderPosition + 1) <= encoderResolution) {
				encoderPosition = encoderPosition + 1;
			}
			else if ((encoderPosition + 1) > encoderResolution) {
				encoderPosition = 1;
			}
		}
	}
	//SerialUSB.println(retstep);
	//validate stored start point data
	//SerialUSB.print("\t");
	//SerialUSB.println(encoderPosition);
	if (encoderPosition == 1) {
		
		//get_sensorValue(1);
		//get_sensorValue(2);
		/*
		SerialUSB.print(storedstartposition);
		SerialUSB.print("\t");
		SerialUSB.print(startposition);
		SerialUSB.print("\t");
		SerialUSB.print(storedsensor1state);
		SerialUSB.print("\t");
		SerialUSB.print(sensor1);
		SerialUSB.print("\t");
		SerialUSB.print(storedsensor2state);
		SerialUSB.print("\t");
		SerialUSB.println(sensor2);
		*/
		int passmark = 0;
		inSync = false;
		if (storedstartposition == startposition)
		{

			passmark += 1;
			/*
			if (storedsensor1state > Neutral[1] && sensor1 > Neutral[1] || storedsensor1state < Neutral[0] && sensor1 < Neutral[0]
				|| storedsensor1state > Neutral[1] && sensor1 > Neutral[1] || storedsensor1state < Neutral[0] && sensor1 < Neutral[0]
				)
			{
				passmark += 1;
			}

			if (storedsensor2state > Neutral[1] && sensor2 > Neutral[1] || storedsensor2state < Neutral[0] && sensor2 < Neutral[0]
				|| storedsensor2state > Neutral[1] && sensor2 > Neutral[1] || storedsensor2state < Neutral[0] && sensor2 < Neutral[0]
				)
			{
				passmark += 1;
			}
			*/
		}
		if (passmark == 1)
		{
			inSync = true;
		}
	}

	if (inSync)
	{
		return encoderPosition;
	}
	else {
		encoderPosition = -1;
		return encoderPosition;
	}

}

void MagRotaryEncoder::set_encoderResolution(int res) {
	encoderResolution = res;
	encoderPosition = 0;
	storedstartposition = 0;
	storedsensor1state = 0;
	storedsensor2state = 0;
	inSync = false;
}

int MagRotaryEncoder::setToStart() {
	encoderPosition = 1;

	startposition = 0;
	get_sensorValue(1);
	get_sensorValue(2);
	recaliberate_startPosition();
	count = 0;

	if (startposition != 0)
	{
		storedstartposition = startposition;
		storedsensor1state = sensor1;
		storedsensor2state = sensor2;
		inSync = true;
		return encoderPosition;
	}
	else
	{
		return -1;
	}
}

int MagRotaryEncoder::detect_rotation() {  // openloop rotation encoding function

	if (!useInterrupt) { //interrupt detection is not used
		get_sensorValue(1);
		get_sensorValue(2);
		int newstate = get_encodingState();
		if (startposition == 1) {
			if (newstate == 2)
			{
				rotation_action(1);
				haptics(1);
			}
			else if (newstate == 4)
			{
				rotation_action(0);
				haptics(1);
			}
		}

		else if (startposition == 2) {
			if (newstate == 3)
			{
				rotation_action(1);
				haptics(1);
			}
			else if (newstate == 1)
			{
				rotation_action(0);
				haptics(1);
			}
		}

		else if (startposition == 3) {
			if (newstate == 4)
			{
				rotation_action(1);
				haptics(1);
			}
			else if (newstate == 2)
			{
				rotation_action(0);
				haptics(1);
			}
		}

		else if (startposition == 4) {
			if (newstate == 1)
			{
				rotation_action(1);
				haptics(1);
			}
			else if (newstate == 3)
			{
				rotation_action(0);
				haptics(1);
			}
		}

		if (count != 0) {
			if (invertcount)
			{
				count *= -1;
			}
			haptics(1);

			if (encoderResolution > 0)
			{
				if (inSync)
				{
					tempcount = count;
					recaliberate_startPosition();
					tempcount = get_encResCount(tempcount);
					//SerialUSB.print(tempcount);
				}
				else {
					tempcount = -1;
				}

			}
			else {
				tempcount = count;
				recaliberate_startPosition();
			}
			count = 0;
			return tempcount;
		}
		else {
			tempcount = count;
			recaliberate_startPosition();
			return 0;
		}

	}
	else {
		if (INTProcessed) { //processed in interrupt

			if (count != 0) {
				if (invertcount)
				{
					count *= -1;
				}
				haptics(1);

				if (encoderResolution > 0)
				{
					if (inSync)
					{
						tempcount = count;
						recaliberate_startPosition();
						tempcount = get_encResCount(tempcount);
						//SerialUSB.print(tempcount);
					}
					else {
						tempcount = -1;
					}
					
				}
				else {
					tempcount = count;
					recaliberate_startPosition();
				}
				count = 0;
				return tempcount;
			}
			else {
				tempcount = count;
				recaliberate_startPosition();
				return 0;
			}
		}
		else
		{
			if (!digitalRead(SensorINTpin[1]))//int1 active
			{
				get_sensorValue(2);
				startposition = get_encodingState(1);
				if (startposition != prevstartposition)
				{
					rotationrate = millis() - timetracker;  //rotation step rate
					if (startposition == 2) { //new startposition is not == 1 or 4
						if (prevstartposition == 1) { //clockwise
							count++;
						}
						else if (prevstartposition == 3) { //counterclockwise
							count--;
						}
						prevstartposition = startposition;
					}

					else if (startposition == 4) { // new startposition == 4
						if (prevstartposition == 3) { //clockwise
							count++;
						}
						else if (prevstartposition == 1) { //counterclockwise
							count--;
						}
						prevstartposition = startposition;
					}
					prevstartposition = startposition;
					timetracker = millis();
					INTProcessed = true;
				}
				
			}
			else if (!digitalRead(SensorINTpin[0]))//int2 active
			{
				get_sensorValue(1);
				startposition = get_encodingState(2);
				if (startposition != prevstartposition)
				{
					rotationrate = millis() - timetracker;  //rotation step rate
					if (startposition == 3) { //new startposition is not == 1 or 4
						if (prevstartposition == 2) { //clockwise
							count++;
						}
						else if (prevstartposition == 4) { //counterclockwise
							count--;
						}
						prevstartposition = startposition;
					}
					else if (startposition == 1) { // new startposition == 1
						if (prevstartposition == 4) { //clockwise
							count++;
						}
						else if (prevstartposition == 2) { //counterclockwise
							count--;
						}
						prevstartposition = startposition;
					}
					prevstartposition = startposition;
					timetracker = millis();
					INTProcessed = true;
				}
			}
			if (count != 0) {
				if (invertcount)
				{
					count *= -1;
				}
				haptics(1);

				if (encoderResolution > 0)
				{
					if (inSync)
					{
						tempcount = count;
						tempcount = get_encResCount(tempcount);
						//SerialUSB.print(tempcount);
					}
					else {
						tempcount = -1;
					}

				}
				else {
					tempcount = count;
					//recaliberate_startPosition();
				}
				count = 0;
				return tempcount;
			}
			else {
				tempcount = count;
				recaliberate_startPosition();
				return 0;
			}
		}
		tempcount = count;
		//recaliberate_startPosition();
		return 0;
	}
}

int MagRotaryEncoder::detect_rotationWithRate() {  // openloop rotation encoding function

	if (!useInterrupt) { //interrupt detection is not used
		get_sensorValue(1);
		get_sensorValue(2);
		int newstate = get_encodingState();
		if (startposition == 1) {
			if (newstate == 2)
			{
				rotationrate = millis() - timetracker;
				timetracker = millis();
				rotation_action(1);
				haptics(1);
			}
			else if (newstate == 4)
			{
				rotationrate = millis() - timetracker;
				timetracker = millis();
				rotation_action(0);
				haptics(1);
			}
		}

		else if (startposition == 2) {
			if (newstate == 3)
			{
				rotationrate = millis() - timetracker;
				timetracker = millis();
				rotation_action(1);
				haptics(1);
			}
			else if (newstate == 1)
			{
				rotationrate = millis() - timetracker;
				timetracker = millis();
				rotation_action(0);
				haptics(1);
			}
		}

		else if (startposition == 3) {
			if (newstate == 4)
			{
				rotationrate = millis() - timetracker;
				timetracker = millis();
				rotation_action(1);
				haptics(1);
			}
			else if (newstate == 2)
			{
				rotationrate = millis() - timetracker;
				timetracker = millis();
				rotation_action(0);
				haptics(1);
			}
		}

		else if (startposition == 4) {
			if (newstate == 1)
			{
				rotationrate = millis() - timetracker;
				timetracker = millis();
				rotation_action(1);
				haptics(1);
			}
			else if (newstate == 3)
			{
				rotationrate = millis() - timetracker;
				timetracker = millis();
				rotation_action(0);
				haptics(1);
			}
		}

		if (count != 0) {
			
			if (invertcount)
			{
				count *= -1;
			}
			haptics(1);
			if (rotationrate > 0) {  //avoid negative values
				if (rotationrate < timetomultiply) { //not fast anough for multiplier
					float calcmul = 1 - ((float)rotationrate / (float)timetomultiply);
					count = count * multiplier * calcmul;
					//timetracker = millis();
				}

			}
			tempcount = count;
			recaliberate_startPosition();
			
			count = 0;
			return tempcount;
		}
		else {
			tempcount = count;
			recaliberate_startPosition();
			return 0;
		}

	}
	else {
		if (INTProcessed) { //processed in interrupt

			if (count != 0) {
			
				if (invertcount)
				{
					count *= -1;
				}
				haptics(1);
				if (rotationrate > 0) {  //avoid negative values
					if (rotationrate < timetomultiply) { //not fast anough for multiplier
						float calcmul = 1 - ((float)rotationrate / (float)timetomultiply);
						count = count * multiplier * calcmul;
						//timetracker = millis();
					}

				}
				tempcount = count;
				recaliberate_startPosition();
			
				count = 0;
				return tempcount;
			}
			else {
				tempcount = count;
				recaliberate_startPosition();
				return 0;
			}
		}
		else
		{
			if (!digitalRead(SensorINTpin[1]))//int1 active
			{
				get_sensorValue(2);
				startposition = get_encodingState(1);
				if (startposition != prevstartposition)
				{
					rotationrate = millis() - timetracker;  //rotation step rate
					if (startposition == 2) { //new startposition is not == 1 or 4
						if (prevstartposition == 1) { //clockwise
							count++;
						}
						else if (prevstartposition == 3) { //counterclockwise
							count--;
						}
						prevstartposition = startposition;
					}

					else if (startposition == 4) { // new startposition == 4
						if (prevstartposition == 3) { //clockwise
							count++;
						}
						else if (prevstartposition == 1) { //counterclockwise
							count--;
						}
						prevstartposition = startposition;
					}
					prevstartposition = startposition;
					timetracker = millis();
					INTProcessed = true;
				}

			}
			else if (!digitalRead(SensorINTpin[0]))//int2 active
			{
				get_sensorValue(1);
				startposition = get_encodingState(2);
				if (startposition != prevstartposition)
				{
					rotationrate = millis() - timetracker;  //rotation step rate
					if (startposition == 3) { //new startposition is not == 1 or 4
						if (prevstartposition == 2) { //clockwise
							count++;
						}
						else if (prevstartposition == 4) { //counterclockwise
							count--;
						}
						prevstartposition = startposition;
					}
					else if (startposition == 1) { // new startposition == 1
						if (prevstartposition == 4) { //clockwise
							count++;
						}
						else if (prevstartposition == 2) { //counterclockwise
							count--;
						}
						prevstartposition = startposition;
					}
					prevstartposition = startposition;
					timetracker = millis();
					INTProcessed = true;
				}
			}
			if (count != 0) {

				if (invertcount)
				{
					count *= -1;
				}
				haptics(1);
				if (rotationrate > 0) {  //avoid negative values
					if (rotationrate < timetomultiply) { //not fast anough for multiplier
						float calcmul = 1 - ((float)rotationrate / (float)timetomultiply);
						count = count * multiplier * calcmul;
						//timetracker = millis();
					}

				}
				tempcount = count;
				recaliberate_startPosition();

				count = 0;
				return tempcount;
			}
			else {
				tempcount = count;
				recaliberate_startPosition();
				return 0;
			}
		}
		tempcount = count;
		recaliberate_startPosition();
		return 0;
	}
}


int MagRotaryEncoder::detect_rotationHR() {  // openloop rotation encoding function

	int sensvalrange = 0;

	if (!useInterrupt) { //interrupt detection is not used
		if (startposition == 1) {   //sensor2 is in neutral
			get_sensorValue(1);
			get_sensorValue(2);
			if (!(sensor2 < S2_Neutral[1] && sensor2 > S2_Neutral[0])) { //analog value is not in the neutral range
				// check for actual rotation
				if (prevsensor2 != Mid) {
					if (sensor2 > (prevsensor2 + setresolution) || sensor2 < (prevsensor2 - setresolution)) {
						//get distance from north and south pole magnets
						distance = prevsensor2 - sensor2;
						//check rotation direction with distance values
						if (distance > 0) {  //moved towards magnet northpole
							prevsensor2 = sensor2 + setresolution - (setresolution - 1);
							rotation_action(0);
							haptics(1);
						}
						else if (distance < 0) {  //moved towards magnet southpole
							prevsensor2 = sensor2 - setresolution + (setresolution - 1);
							rotation_action(1);
							haptics(1);
						}
					}
				}
				else {
					prevsensor2 = sensor2;
				}
			}

		}

		else if (startposition == 2) {  //sensor1 is in neutral
			get_sensorValue(1);
			get_sensorValue(2);
			if (!(sensor1 < S1_Neutral[1] && sensor1 > S1_Neutral[0])) { //analog value is not in the neutral range
				// check for actual rotation
				if (prevsensor1 != Mid) {
					// check for actual rotation
					if (sensor1 > (prevsensor1 + setresolution) || sensor1 < (prevsensor1 - setresolution)) {
						//get distance from north and south pole magnets
						distance = prevsensor1 - sensor1;
						//check rotation direction with distance values
						if (distance > 0) {  //moved towards magnet northpole
							prevsensor1 = sensor1 + setresolution - (setresolution - 1);
							rotation_action(0);
							haptics(1);
						}
						else if (distance < 0) {  //moved towards magnet southpole
							prevsensor1 = sensor1 - setresolution + (setresolution - 1);
							rotation_action(1);
							haptics(1);
						}
					}
				}
				else {
					prevsensor1 = sensor1;
				}
			}
		}

		else if (startposition == 3) {  //sensor2 is in neutral
			get_sensorValue(1);
			get_sensorValue(2);
			if (!(sensor2 < S2_Neutral[1] && sensor2 > S2_Neutral[0])) { //analog value is not in the neutral range
				// check for actual rotation
				if (prevsensor2 != Mid) {
					// check for actual rotation
					if (sensor2 > (prevsensor2 + setresolution) || sensor2 < (prevsensor2 - setresolution)) {
						distance = prevsensor2 - sensor2;
						//check rotation direction with distance values
						if (distance > 0) {  //moved towards magnet northpole
							prevsensor2 = sensor2 + setresolution - (setresolution - 1);
							rotation_action(1);
							haptics(1);
						}
						else if (distance < 0) {  //moved towards magnet southpole
							prevsensor2 = sensor2 - setresolution + (setresolution - 1);
							rotation_action(0);
							haptics(1);
						}
					}
				}
				else {
					prevsensor2 = sensor2;
				}
			}
		}

		else if (startposition == 4) {  //sensor1 is in neutral
			get_sensorValue(1);
			get_sensorValue(2);
			if (!(sensor1 < S1_Neutral[1] && sensor1 > S1_Neutral[0])) { //analog value is not in the neutral range
				// check for actual rotation
				if (prevsensor1 != Mid) {
					// check for actual rotation
					if (sensor1 > (prevsensor1 + setresolution) || sensor1 < (prevsensor1 - setresolution)) {
						//get distance from north and south pole magnets
						distance = prevsensor1 - sensor1;
						//check rotation direction with distance values
						if (distance > 0) {  //moved towards magnet northpole
							prevsensor1 = sensor1 + setresolution - (setresolution - 1);;
							rotation_action(1);
							haptics(1);
						}
						else if (distance < 0) {  //moved towards magnet southpole
							prevsensor1 = sensor1 - setresolution + (setresolution - 1);;
							rotation_action(0);
							haptics(1);
						}
					}
				}
				else {
					prevsensor1 = sensor1;
				}
			}
		}



		else {
			get_sensorValue(1);
			get_sensorValue(2);
		}
		tempcount = count;
		/*
		countt += tempcount;
		if (tempcount != 0) {
			SerialUSB.print(setresolution);
			SerialUSB.print("\t");
			SerialUSB.print(sensor1);
			SerialUSB.print("\t");
			SerialUSB.print(prevsensor1);
			SerialUSB.print("\t");
			SerialUSB.print(sensor2);
			SerialUSB.print("\t");
			SerialUSB.print(prevsensor2);
			SerialUSB.print("\t");
			SerialUSB.print(startposition);
			SerialUSB.print("\t");
			SerialUSB.print(tempcount);
			SerialUSB.print("\t");
			SerialUSB.println(countt);

		}
		*/

		recaliberate_startPosition();
		return tempcount;
	}
	else {  //interrupt assisted
		if (INT1fired) {
			get_sensorValue(2);
			startposition = get_encodingState(1);
		}
		else if (INT2fired) {
			get_sensorValue(1);
			startposition = get_encodingState(2);
		}

		if (startposition == 1) {   //sensor2 is in neutral
			//get_sensorValue(1);
			get_sensorValue(2);

			if (!(sensor2 < S2_Neutral[1] && sensor2 > S2_Neutral[0])) { //analog value is not in the neutral range
				// check for actual rotation
				if (prevsensor2 != Mid) {
					if (sensor2 > (prevsensor2 + setresolution) || sensor2 < (prevsensor2 - setresolution)) {
						//get distance from north and south pole magnets
						distance = prevsensor2 - sensor2;
						//check rotation direction with distance values
						if (distance > 0) {  //moved towards magnet northpole
							prevsensor2 = sensor2 + setresolution - (setresolution - 1);
							rotation_action(0);
							haptics(1);
						}
						else if (distance < 0) {  //moved towards magnet southpole
							prevsensor2 = sensor2 - setresolution + (setresolution - 1);
							rotation_action(1);
							haptics(1);
						}
					}
				}
				else {
					prevsensor2 = sensor2;
				}
			}

		}

		else if (startposition == 2) {  //sensor1 is in neutral

			get_sensorValue(1);
			//get_sensorValue(2);


			if (!(sensor1 < S1_Neutral[1] && sensor1 > S1_Neutral[0])) { //analog value is not in the neutral range
				// check for actual rotation
				if (prevsensor1 != Mid) {
					// check for actual rotation
					if (sensor1 > (prevsensor1 + setresolution) || sensor1 < (prevsensor1 - setresolution)) {
						//get distance from north and south pole magnets
						distance = prevsensor1 - sensor1;
						//check rotation direction with distance values
						if (distance > 0) {  //moved towards magnet northpole
							prevsensor1 = sensor1 + setresolution - (setresolution - 1);
							rotation_action(0);
							haptics(1);
						}
						else if (distance < 0) {  //moved towards magnet southpole
							prevsensor1 = sensor1 - setresolution + (setresolution - 1);
							rotation_action(1);
							haptics(1);
						}
					}
				}
				else {
					prevsensor1 = sensor1;
				}
			}
		}

		else if (startposition == 3) {  //sensor2 is in neutral


			//get_sensorValue(1);
			get_sensorValue(2);

			if (!(sensor2 < S2_Neutral[1] && sensor2 > S2_Neutral[0])) { //analog value is not in the neutral range
				// check for actual rotation
				if (prevsensor2 != Mid) {
					// check for actual rotation
					if (sensor2 > (prevsensor2 + setresolution) || sensor2 < (prevsensor2 - setresolution)) {
						distance = prevsensor2 - sensor2;
						//check rotation direction with distance values
						if (distance > 0) {  //moved towards magnet northpole
							prevsensor2 = sensor2 + setresolution - (setresolution - 1);
							rotation_action(1);
							haptics(1);
						}
						else if (distance < 0) {  //moved towards magnet southpole
							prevsensor2 = sensor2 - setresolution + (setresolution - 1);
							rotation_action(0);
							haptics(1);
						}
					}
				}
				else {
					prevsensor2 = sensor2;
				}
			}
		}

		else if (startposition == 4) {  //sensor1 is in neutral

			get_sensorValue(1);
			//get_sensorValue(2);


			if (!(sensor1 < S1_Neutral[1] && sensor1 > S1_Neutral[0])) { //analog value is not in the neutral range
				// check for actual rotation
				if (prevsensor1 != Mid) {
					// check for actual rotation
					if (sensor1 > (prevsensor1 + setresolution) || sensor1 < (prevsensor1 - setresolution)) {
						//get distance from north and south pole magnets
						distance = prevsensor1 - sensor1;
						//check rotation direction with distance values
						if (distance > 0) {  //moved towards magnet northpole
							prevsensor1 = sensor1 + setresolution - (setresolution - 1);;
							rotation_action(1);
							haptics(1);
						}
						else if (distance < 0) {  //moved towards magnet southpole
							prevsensor1 = sensor1 - setresolution + (setresolution - 1);;
							rotation_action(0);
							haptics(1);
						}
					}
				}
				else {
					prevsensor1 = sensor1;
				}
			}
		}

		if (invertcount)
		{
			count *= -1;
		}

		tempcount = count;
		recaliberate_startPosition();
		return tempcount;
	}


}






void MagRotaryEncoder::set_resolution(int percent) {
	if (percent == 100) {
		setresolution = 1;
		use_extended_resolution = true;
	}
	else if (percent == 0) {
		use_extended_resolution = false;
	}
	else {
		setresolution = stepres - ((percent * 0.01) * stepres);
		use_extended_resolution = true;
	}

}





void MagRotaryEncoder::rotation_action(int act) {  //sets action for clockwise and anticlockwise rotations
	if (act == 1) {
		count++;
		
	}
	else if (act == 0) {
		count--;
		
	}
}

void MagRotaryEncoder::set_bound(int b) {   //this value determines the upper and lower limit of the ADC values
	bound = b;
	initialize_encoder();
}
void MagRotaryEncoder::useinterruptdetection(bool act) {   
	useInterrupt = act;
}

void MagRotaryEncoder::set_haptics(int pin, int duration, int strength) {  //use to set haptics variables (arduino pwm pin, duration of haptics(ms), strength from 0-255)
	haptics_pin = pin;
	haptics_duration = duration;
	haptics_strength = strength;
}

void MagRotaryEncoder::haptics(int state) {   //viberation feedback function
	if (state == 1) {
		analogWrite(haptics_pin, haptics_strength);
		haptics_ontime = millis();
		haptics_state = 1;
	}
	else {
		haptics_offtime = millis();
		if (((haptics_offtime - haptics_ontime) >= haptics_duration) && haptics_state == 1) {
			analogWrite(haptics_pin, 0);
			haptics_state = 0;
		}

	}
	/*
	analogWrite(haptics_pin, haptics_strength);
	delay(haptics_duration);
	analogWrite(haptics_pin, 0);
	*/
}

void MagRotaryEncoder::setsleep(bool slpact) {
	sleepmode = slpact;
}

bool MagRotaryEncoder::readsleep() {
	return sleepmode;
}

