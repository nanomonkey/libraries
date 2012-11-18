#ifdef __cplusplus
extern "C" {
#endif

#define V1 1
#define V2 2
#define V3 3
#define HALFFILL 1
#define FULLFILL 2
#define LITEFILL 3
#define P7722 0
#define P777222 1
#define P222777 2
#define P777722 3
#define NULL -9999

extern int GCU_version; 
extern int GCU_fill; 
extern int GCU_psequence; 

void GCU_Setup(int version, int fill, int psequence);
void DelayMS(unsigned int count);

#ifdef __cplusplus
}
#endif
