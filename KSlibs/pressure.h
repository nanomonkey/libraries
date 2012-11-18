#ifdef __cplusplus
extern "C" {
#endif

extern int GCU_version;
extern int GCU_fill;
extern int GCU_psequence;
extern int n_pressure;

#define NPRESS (6)

extern long Press_Data[NPRESS];
extern int Press_Available[NPRESS];


void Press_Init() ;
void Press_Reset();
void Press_ReadAll();



#ifdef __cplusplus
}
#endif
