#include "head.h"

//���
void putchar(const var ch) {//��ӡһ���ַ�����ǰ��
	scr_func(1937642001, ch);
}

void print_str(const var *str) {//��ӡһ���ַ�������ǰ��
	scr_func(1170697315, str);
}

void print_num(const var num) {//��ӡһ�����ֵ���ǰ��
	scr_func(282132836, num);
}

//ѯ������
void input_str() {//�á�ѯ�ʡ�ģ�齫�ַ������뻺����
	scr_func(1905016116, 0);
}

void clear_buffer() {//��ջ�����
	scr_func(2589927310, 0);
}

var getchar() {//�ӻ������ж�ȡһ���ַ�
	return scr_func(2617352480, 0);
}

var getnum() {//�ӻ������ж�ȡһ������
	return scr_func(1004227932, 0);
}

void scan_str(const var* str) {//�ӻ������ж�ȡһ���ַ�����д�����
	scr_func(1331999947, str);
}

void scan_str_getline(const var* str) {//�ӻ�������ȡһ�����ַ���
	scr_func(1740578662, str);
}

void scan_num(const var *num_p) {//�ӻ������ж�ȡһ�����ֲ�д�����
	scr_func(443435468, num_p);
}

//��������
var get_key() {//��ȡ��ǰ���̰��µļ�����û���򷵻�0
	return scr_func(1343832706, 0);
}

void start_key_detection_service() {//��������������
	scr_func(112511034, 0);
}

void stop_key_detection_service() {//ֹͣ����������
	scr_func(1624815706, 0);
}

var get_last_key() {//��ȡ��һ�μ��̰��µļ�����û���򷵻�0
	return scr_func(427327727, 0);
}

var key_pressed(const var key) {//�ж�һ�����Ƿ���
	return scr_func(204560538, key);
}

//�������
var get_mouse_x() {//��ȡ���x����
	return scr_func(1073433617, 0);
}

var get_mouse_y() {//��ȡ����y����
	return scr_func(1073433618, 0);
}

var mouse_pressed() {//�ж������Ƿ���
	return scr_func(4004631084, 0);
}

var wheel_count() {//��ȡ�����ֵ�ǰ�ۼƹ�����������Ϊ�������������
	return scr_func(4100929295, 0);
}