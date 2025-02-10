#include "head.h"

void* memcpy(void* dest, const void* src, var n) {
	return scr_func(1649996047, &dest);
}

void* memmove(void* dest, const void* src, var n) {
	return scr_func(1613856462, &dest);
}

void* memset(void* ptr, var value, var n) {
	return scr_func(198331081, &ptr);
}

var memcmp(const void* ptr1, const void* ptr2, var n) {
	return scr_func(1649956045, &ptr1);
}

void* malloc(var size) {
	return scr_func(1445361288, size);
}

void* calloc(var num, var size) {
	return memset(malloc(num * size), 0, num * size);
}

void free(void* ptr) {
	scr_func(1907765640, ptr);
}

void* realloc(void* ptr, var new_size) {
	var size = ((var*)ptr)[-2];
	free(ptr);
	return memmove(malloc(new_size), ptr, size);
}

