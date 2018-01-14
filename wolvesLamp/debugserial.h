#ifndef DEBUGSERIAL_H_
#define DEBUGSERIAL_H_

//comment out the line below to disable DEBUG Messages
#define DEBUGMODE

#ifdef DEBUGMODE
#define debugPrint(x)	Serial.print(x)
#else
#define debugPrint(x)
#endif


extern void debugserial_init();


#endif /* DEBUGSERIAL_H_ */
