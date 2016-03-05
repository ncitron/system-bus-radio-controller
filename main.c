// SYSTEM BUS RADIO
// https://github.com/fulldecent/system-bus-radio
// Copyright 2016 William Entriken

//system-bus-radio-controller
//using code from above author along with noah7545 and jczimm

#include <stdio.h>
#include <emmintrin.h>
#include <math.h>
#include<stdint.h>
#include<time.h>
__m128i reg;
__m128i reg_zero;
__m128i reg_one;

#define NSEC_PER_SEC	1000000000	/* nanoseconds per second */

uint64_t mach_absolute_time(){
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start); /* mark start time */

	return (uint64_t)start.tv_sec * NSEC_PER_SEC + start.tv_nsec;
}

static inline void square_am_signal(float time, float frequency) {
	printf("Playing / %0.3f seconds / %4.0f Hz\n", time, frequency);
	uint64_t period = NSEC_PER_SEC / frequency;

	uint64_t start = mach_absolute_time();
	uint64_t end = start + time * NSEC_PER_SEC;

	printf("%u\n",start);	

	while (mach_absolute_time() < end) {
		uint64_t mid = start + period / 2;
		uint64_t reset = start + period;

   		while (mach_absolute_time() < mid) {
      			_mm_stream_si128(&reg, reg_one);
      			_mm_stream_si128(&reg, reg_zero);
  		}


		while (mach_absolute_time() < reset){

		}

		start = reset;
	}
}

int main()
{


	uint64_t start = mach_absolute_time();
	uint64_t end = mach_absolute_time();
	printf("TESTING TIME TO EXECUTE mach_absolute_time()\n  Result: %lld nanoseconds\n\n", end - start);

	
	reg_zero = _mm_set_epi32(0, 0, 0, 0);
	reg_one = _mm_set_epi32(-1, -1, -1, -1);
	
	FILE *pipe_fp;
	char note[10];	
	float val;
	pipe_fp = popen("node out.js", "r");
	while (1) {
		fgets(note, 10, pipe_fp);
		square_am_signal(.05, val = atof(note));
	}
}
