#include "head.h"

//输出
void putchar(const var ch) {//打印一个字符到当前行
	scr_func(1937642001, ch);
}

void print_str(const var *str) {//打印一个字符串到当前行
	scr_func(1170697315, str);
}

void print_num(const var num) {//打印一个数字到当前行
	scr_func(282132836, num);
}

//询问输入
void input_str() {//用“询问”模块将字符串读入缓冲区
	scr_func(1905016116, 0);
}

void clear_buffer() {//清空缓冲区
	scr_func(2589927310, 0);
}

var getchar() {//从缓冲区中读取一个字符
	return scr_func(2617352480, 0);
}

var getnum() {//从缓冲区中读取一个数字
	return scr_func(1004227932, 0);
}

void scan_str(const var* str) {//从缓冲区中读取一个字符串并写入变量
	scr_func(1331999947, str);
}

void scan_str_getline(const var* str) {//从缓冲区读取一整行字符串
	scr_func(1740578662, str);
}

void scan_num(const var *num_p) {//从缓冲区中读取一个数字并写入变量
	scr_func(443435468, num_p);
}

//键盘输入
var get_key() {//获取当前键盘按下的键，若没有则返回0
	return scr_func(1343832706, 0);
}

void start_key_detection_service() {//启动按键检测服务
	scr_func(112511034, 0);
}

void stop_key_detection_service() {//停止按键检测服务
	scr_func(1624815706, 0);
}

var get_last_key() {//获取上一次键盘按下的键，若没有则返回0
	return scr_func(427327727, 0);
}

var key_pressed(const var key) {//判断一个键是否按下
	return scr_func(204560538, key);
}

//鼠标输入
var get_mouse_x() {//获取鼠标x坐标
	return scr_func(1073433617, 0);
}

var get_mouse_y() {//获取鼠标的y坐标
	return scr_func(1073433618, 0);
}

var mouse_pressed() {//判断鼠标键是否按下
	return scr_func(4004631084, 0);
}

var wheel_count() {//获取鼠标滚轮当前累计滚动量（向下为正）并清除该量
	return scr_func(4100929295, 0);
}