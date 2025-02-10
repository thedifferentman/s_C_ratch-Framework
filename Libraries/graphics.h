#include "head.h"

struct Position {
	var x, y;
};

//“∆∂Ø
void move_to(const var x, const var y) {
	struct Position pos = { x,y };
	scr_func(992057857, &pos);
}

void set_x(const var x) {
	scr_func(427086803, x);
}

void set_y(const var y) {
	scr_func(427086804, y);
}

void add_x(const var x) {
	scr_func(2857401158, x);
}

void add_y(const var y) {
	scr_func(2857401159, y);
}

void set_direction(const var dir) {
	scr_func(2971055278, dir);
}

void add_direction(const var dir) {
	scr_func(1633127297, dir);
}

var get_x() {
	return scr_func(2812393223, 0);
}

var get_y() {
	return scr_func(2812393224, 0);
}

var get_direction() {
	return scr_func(1075444322, 0);
}

//Õ‚π€
void change_to_costume(const var* name) {
	scr_func(426981027, name);
}

void change_to_costume_id(const var id) {
	scr_func(1874165607, id);
}

void set_size(const var size) {
	scr_func(3222504926, size);
}

void add_size(const var size) {
	scr_func(178860047, size);
}

void set_color_effect(const var val) {
	scr_func(1544451746, val);
}

void add_color_effect(const var val) {
	scr_func(2859659763, val);
}

void set_brightness_effect(const var val) {
	scr_func(2600697030, val);
}

void add_brightness_effect(const var val) {
	scr_func(3633483513, val);
}

void set_ghost_effect(const var val) {
	scr_func(4175052898, val);
}

void add_ghost_effect(const var val) {
	scr_func(1195293619, val);
}

void show() {
	scr_func(3855099021, 0);
}

void hide() {
	scr_func(1211672154, 0);
}

var get_costume_id() {
	return scr_func(4200083763, 0);
}

var get_size() {
	return scr_func(354560026, 0);
}

var get_color_effect() {
	return scr_func(2509517918, 0);
}

var get_brightness_effect() {
	return scr_func(956101882, 0);
}

var get_ghost_effect() {
	return scr_func(845151774, 0);
}

//ª≠± 
void erase_all() {
	scr_func(657238400, 0);
}

void stamp() {
	scr_func(1678393327, 0);
}

void pen_down() {
	scr_func(382552892, 0);
}

void pen_up() {
	scr_func(1745374037, 0);
}

void _set_color(const var col) {
	scr_func(2791086155, col);
}

var rgb_color(const var r, const var g, const var b) {
	if (r < 0 || r>255 || g < 0 || g>255 || b < 0 || b>255) scr_func(0, "invalid RGB value");
	return r * 65536 + g * 256 + b;
}

void set_color(const var r, const var g, const var b) {
	_set_color(rgb_color(r, g, b));
}

void set_transparency(const var transparency) {
	scr_func(1226076853, transparency);
}

void set_pen_size(const var size) {
	scr_func(1915221908, size);
}