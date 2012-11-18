#ifdef __cplusplus
extern "C" {
#endif

#define NFETS (8)

#define FET0 5 //Arduino pin 5 = FET 0
#define FET1 2 //Arduino pin 2 = FET 1
#define FET2 3 //Arduino pin 3 = FET 2
#define FET3 6 //Arduino pin 6 = FET 3
#define FET4 7 //Arduino pin 7 = FET 4
#define FET5 8 //Arduino pin 8 = FET 5
#define FET6 46 //Arduino pin 46 = FET 6
#define FET7 45 //Arduino pin 45 = FET 7

extern int Fet_Data[NFETS];
void Fet_Init();
void Fet_Reset();
void Fet_WriteAll();



#ifdef __cplusplus
}
#endif
