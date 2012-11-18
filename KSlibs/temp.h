#ifdef __cplusplus
extern "C" {
#endif



#define NTEMP (16)
extern int n_temp;


extern unsigned long Temp_Data[NTEMP];
extern int Temp_Available[NTEMP];


void Temp_Init() ;
void Temp_Reset();
void Temp_ReadAll();



#ifdef __cplusplus
}
#endif
