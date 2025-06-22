// src/prog1/init.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "path_utils.h"

void ensure_student_dir_and_index() {
    struct stat st;

    char student_dir_path[PATH_MAX];
    char index_path[PATH_MAX];

    snprintf(student_dir_path, sizeof(student_dir_path), "%s/Student", g_project_root);
    get_student_path(index_path, sizeof(index_path), "Index.dat");


    // 1) Student 디렉토리 존재 확인 → 없으면 생성
    if (stat(student_dir_path, &st) == -1) {
        if (errno != ENOENT) {
            perror("stat");
            exit(EXIT_FAILURE);
        }
        if (mkdir(student_dir_path, 0755) == -1) {
            perror("mkdir Student");
            exit(EXIT_FAILURE);
        }
        printf("Created directory: %s/\n", student_dir_path);
    }

    // 2) Index.dat 파일 존재 확인 → 없으면 생성
    FILE *fp = fopen(index_path, "a+");
    if (!fp) {
        perror("fopen Index.dat");
        exit(EXIT_FAILURE);
    }
    fclose(fp);
    printf("Index file ready: %s\n", index_path);
}