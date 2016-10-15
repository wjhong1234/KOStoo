#include <stdio.h>
#include "syscalls.h"

/*Infinite loop to occupy core 3.*/
int main() {
	#define MASK 0x4
	cpu_set_t affinityMask = MASK;
	int bleh = sched_setaffinity( 2, sizeof(cpu_set_t), &affinityMask );
	for (;;) {
		//does nothing productive
	}
}
