#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include "subject.h"

#define GRADE_DIR  "Grade"
#define CONF_FILE  GRADE_DIR "/department.conf"

// 디렉토리 내 .dat 파일만 골라 과목 목록을 subjects 배열에 채우고 개수를 반환
int list_subjects(char subjects[][64], int max) {
    DIR *d = opendir(GRADE_DIR);
    if (!d) {
        perror("opendir Grade");
        return 0;
    }
    struct dirent *e;
    int cnt = 0;
    while ((e = readdir(d)) && cnt < max) {
        const char *name = e->d_name;
        // ".." 및 "." 제거
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) continue;
        // .dat 확장자 검사
        const char *ext = strrchr(name, '.');
        if (!ext || strcmp(ext, ".dat") != 0) continue;
        // 등록 파일인지 stat으로 확인
        char path[128];
        snprintf(path, sizeof(path), "%s/%s", GRADE_DIR, name);
        struct stat st;
        if (stat(path, &st) == -1) continue;
        if (!S_ISREG(st.st_mode)) continue;
        // 확장자 제거 후 이름 복사
        char base[64];
        strncpy(base, name, ext - name);
        base[ext - name] = '\0';
        strncpy(subjects[cnt++], base, 64);
    }
    closedir(d);
    return cnt;
}

void add_subject(const char *name) {
    char path[128];
    snprintf(path, sizeof(path), "%s/%s.dat", GRADE_DIR, name);
    FILE *f = fopen(path, "w");
    if (!f) { perror("add_subject"); exit(EXIT_FAILURE); }
    fclose(f);
}

void remove_subject(const char *name) {
    char path[128];
    snprintf(path, sizeof(path), "%s/%s.dat", GRADE_DIR, name);
    if (remove(path) != 0) { perror("remove_subject"); }
}

void get_department(char *buf, size_t len) {
    FILE *f = fopen(CONF_FILE, "r");
    if (!f) { perror("open conf"); exit(EXIT_FAILURE); }
    if (!fgets(buf, len, f)) buf[0] = '\0';
    buf[strcspn(buf, "\n")] = '\0';
    fclose(f);
}