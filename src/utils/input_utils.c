#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input_utils.h"

int read_int(const char* prompt) {
    char buffer[64];
    int value = 0;
    
    while (1) {
        printf("%s", prompt);
        fflush(stdout);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Thử chuyển đổi chuỗi sang số
            char* endptr;
            value = strtol(buffer, &endptr, 10);
            
            // Kiểm tra xem có phải nhập toàn khoảng trắng hoặc không phải số không
            if (endptr != buffer && (*endptr == '\n' || *endptr == '\0')) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number.\n");
    }
}

void read_string(const char* prompt, char* buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) != NULL) {
        // Xóa ký tự xuống dòng ở cuối nếu có
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}

void wait_for_enter() {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n');
}
