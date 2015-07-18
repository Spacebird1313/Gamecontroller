#ifndef SWITCH_H_
#define SWITCH_H_

void SwitchInit(void);	//Initialize switches
char SwitchGet(void);	//Get switch status. Switches are returned as a bitmask (B0 --> B4)


#endif 