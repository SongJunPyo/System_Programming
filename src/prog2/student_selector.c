// src/prog2/student_selector.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "student.h"

#define STUDENT_DIR "Student"

int get_students_by_major(const char *major, char ids[][32], int max) {
    DIR *d = opendir(STUDENT_DIR);
    if (!d) return 0;
    struct dirent *e;
    int cnt = 0;

    while ((e = readdir(d)) && cnt < max) {
        const char *name = e->d_name;
        // "." 및 ".." 건너뛰기
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) 
            continue;
        // .dat 확장자 검사
        const char *ext = strrchr(name, '.');
        if (!ext || strcmp(ext, ".dat") != 0) 
            continue;

        // 실제 파일인지 stat으로 확인
        char path[256];
        snprintf(path, sizeof(path), "%s/%s", STUDENT_DIR, name);
        struct stat st;
        if (stat(path, &st) == -1) 
            continue;
        if (!S_ISREG(st.st_mode)) 
            continue;

        // 파일 열어서 학과 검사
        FILE *f = fopen(path, "r");
        if (!f) 
            continue;

        char buf[256], sid[32], maj[64];
        // 첫 줄: 학번. 세 줄을 건너뛰고 네 번째 줄에서 학과 읽기
        fgets(buf, sizeof(buf), f);
        sscanf(buf, "학번: %31s", sid);
        for (int i = 0; i < 3; i++) 
            fgets(buf, sizeof(buf), f);
        sscanf(buf, "학과: %63[^\n]", maj);
        fclose(f);

        if (strcmp(maj, major) == 0) {
            strncpy(ids[cnt++], sid, 32);
        }
    }

    closedir(d);
    return cnt;
}