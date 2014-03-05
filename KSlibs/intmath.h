/*
	intmath.h - integer math convenience functions
*/

#ifdef __cplusplus
extern "C" {
#endif

// Subtract and limit the result
unsigned u_sublim (unsigned minuend, unsigned subtrahend, unsigned limit);

// Add and limit the result
unsigned u_addlim (unsigned augend, unsigned addend, unsigned limit);

#ifdef __cplusplus
}
#endif