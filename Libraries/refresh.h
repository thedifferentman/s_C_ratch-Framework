#include "head.h"

void enable_timing_refresh() {
	scr_func(2982520382, 0);
}

void disable_timing_refresh() {
	scr_func(1473611365, 0);
}

void flush_screen() {
	scr_func(3689068315, 0);
}

void set_refresh_time(const var time) {
	scr_func(3205029746, time);
}