unsigned u_sublim (unsigned minuend, unsigned subtrahend, unsigned limit) {
	unsigned x;
	if (minuend < limit) return limit;			// Minuend must already be > limit
	if (subtrahend > minuend) return limit;		// Avoid unsigned underflow (result < 0)
	x = minuend - subtrahend;
	if (x > limit) return x;
	else return limit;
}

unsigned u_addlim (unsigned augend, unsigned addend, unsigned limit) {
	unsigned x;
	if (augend > limit) return limit;			// Augend must be < limit
	if (addend > limit) return limit;			// Addend must be < limit
	x = augend + addend;
	if (x < addend) return limit;				// Overflow must have occurred (usually undefined behavior)
	if (x < limit) return x;
	else return limit;	
}