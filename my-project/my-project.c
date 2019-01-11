#include "api.h"
#include "api-asm.h"

int main(void) {
	/* add your own code */
	uint32_t rev = 0xaabbccdd;
	rev = rev_bytes(rev);
	return my_func(rev);
}
