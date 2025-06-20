// src/prog1/student.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

// Student/<id>.dat 생성
void create_student_file(const char *id,
                         const char *name,
                         const char *dob,
                         const char *major,
                         const char *status) {
    char path[128];
    snprintf(path, sizeof(path), "Student/%s.dat", id);
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