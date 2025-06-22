// src/prog1/student.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "path_utils.h" // 경로 유틸리티 헤더 추가

// Student/<id>.dat 생성
void create_student_file(const char *id,
                         const char *name,
                         const char *dob,
                         const char *major,
                         const char *status) {
    char path[PATH_MAX];
    char filename[64];
    snprintf(filename, sizeof(filename), "%s.dat", id);

    // 수정된 경로 생성 함수 사용
    get_student_path(path, sizeof(path), filename);

    FILE *fp = fopen(path, "w");
    if (!fp) {
        perror("student file");
        exit(EXIT_FAILURE);
    }

    fprintf(fp,
        "학번: %s\n이름: %s\n생년월일: %s\n학과: %s\n상태: %s\n",
        id, name, dob, major, status);

    fclose(fp);
}

// create_student_file이랑 append_index 같이 쓰는 게 좋지 않나?