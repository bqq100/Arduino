#ifndef Utilities_h
#define Utilities_h

extern void(* resetFunc) (void);//declare reset function at address 0

String boolToString( bool input );
int freeRam();

#endif

