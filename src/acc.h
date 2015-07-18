#ifndef ACC_H
#define ACC_H
//Calibratie voor bord 3
void AccInit(void);		//Initialize accelerometer module
unsigned int AccGetXAxisRaw(void); //Get X axis value (uncalibrated)
unsigned int AccGetYAxisRaw(void); //Get Y axis value (uncalibrated)
unsigned int AccGetZAxisRaw(void); //Get Z axis value (uncalibrated)
int AccGetXAxis(unsigned int AccRaw); //Calculate X axis value (in mG). Input: raw Acc value 
int AccGetYAxis(unsigned int AccRaw); //Calculate Y axis value (in mG). Input: raw Acc value 
int AccGetZAxis(unsigned int AccRaw); //Calculate Z axis value (in mG). Input: raw Acc value 

#endif 