// src/prog1/init.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define STUDENT_DIR "Student"
#define INDEX_PATH STUDENT_DIR "/Index.dat"

void ensure_student_dir_and_index() {
    struct stat st;

    // 1) Student 디렉토리 존재 확인 → 없으면 생성
    if (stat(STUDENT_DIR, &st) == -1) {
        if (errno != ENOENT) {
            perror("stat");
            exit(EXIT_FAILURE);
        }
        if (mkdir(STUDENT_DIR, 0755) == -1) {
            perror("mkdir Student");
            exit(EXIT_FAILURE);
        }
        printf("Created directory: %s/\n", STUDENT_DIR);
    }

    // 2) Index.dat 파일 존재 확인 → 없으면 생성
    FILE *fp = fopen(INDEX_PATH, "a+");
    if (!fp) {
        perror("fopen Index.dat");
        exit(EXIT_FAILURE);
    }
    fclose(fp);
    printf("Index file ready: %s\n", INDEX_PATH);
}
