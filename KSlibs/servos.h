#ifdef __cplusplus
extern "C" {
#endif

#define NSERVO (3)

#define SERVO0 11 //Arduino pin 11 = SERVO 0
#define SERVO1 12 //Arduino pin 12 = SERVO 1
#define SERVO2 13 //Arduino pin 13 = SERVO 2

extern int Servo_Data[NSERVO];


void Servo_Init() ;
void Servo_Reset();
void Servo_WriteAll();



#ifdef __cplusplus
}
#endif
