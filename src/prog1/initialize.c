// src/prog1/initialize.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "auth.h"

void initialize_all(void) {
    if (!is_root()) {
        fprintf(stderr, "Error: 관리자만 실행 가능합니다.\n");
        exit(EXIT_FAILURE);
    }

    // Student/와 Grade/ 폴더 및 내부 파일 전체 삭제
    if (system("rm -rf Student Grade") != 0) {
        perror("초기화 실패");
        exit(EXIT_FAILURE);
    }
    printf("모든 데이터가 초기화되었습니다. (Student/, Grade/ 삭제됨)\n");
}