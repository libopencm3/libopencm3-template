#include "bits.h"

unsigned int bits::reverse(unsigned int n) {
	unsigned int r = 0;

	for (int i = 0; i < 32; ++i) {
		if (n & (1 << i)) {
			r |= (1 << (32 - 1 - i));
		}
	}

	return r;
}