#ifdef __cplusplus
extern "C" {
#endif


void Disp_Init();
unsigned char Disp_ReadSR();
void Disp_WriteIR(unsigned char d);
unsigned char Disp_ReadDR();
void Disp_WriteDR(unsigned char d);
void Disp_Reset();
void Disp_Wait();
void Disp_RC(unsigned char, unsigned char);
void Disp_PutChar(char);
void Disp_PutStr(char*);
void Disp_CursOn();
void Disp_CursOff();




#ifdef __cplusplus
}
#endif
