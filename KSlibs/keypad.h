#ifdef __cplusplus
extern "C" {
#endif



void Kpd_Init();
void Kpd_Reset();
int Kpd_Scan();
int Kpd_GetKeyAsync();
int Kpd_GetKeySync();
void Kpd_WaitKeyUp();



#ifdef __cplusplus
}
#endif
