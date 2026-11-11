#ifndef INPUT_UTILS_H
#define INPUT_UTILS_H

#include <stddef.h>

// Đọc một số nguyên từ bàn phím, in ra prompt trước
int read_int(const char* prompt);

// Đọc một chuỗi từ bàn phím, loại bỏ ký tự xuống dòng
void read_string(const char* prompt, char* buffer, size_t size);

// Dừng màn hình chờ người dùng nhấn Enter
void wait_for_enter();

#endif
