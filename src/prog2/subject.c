//src/prog2/subject.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include "subject.h"
#include "path_utils.h"


// 디렉토리 내 .dat 파일만 골라 과목 목록을 subjects 배열에 채우고 개수를 반환
int list_subjects(char subjects[][64], int max) {
    char grade_dir_path[PATH_MAX];
    snprintf(grade_dir_path, sizeof(grade_dir_path), "%s/Grade", g_project_root);

    DIR *d = opendir(grade_dir_path);
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
        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", grade_dir_path, name);
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
    char path[PATH_MAX];
    char filename[128];
    snprintf(filename, sizeof(filename), "%s.dat", name);
    get_grade_path(path, sizeof(path), filename);
    
    FILE *f = fopen(path, "w");
    if (!f) { perror("add_subject"); exit(EXIT_FAILURE); }
    fclose(f);
}

void remove_subject(const char *name) {
    char path[PATH_MAX];
    char filename[128];
    snprintf(filename, sizeof(filename), "%s.dat", name);
    get_grade_path(path, sizeof(path), filename);

    if (remove(path) != 0) { perror("remove_subject"); }
}

void get_department(char *buf, size_t len) {
    char conf_path[PATH_MAX];
    get_grade_path(conf_path, sizeof(conf_path), "department.conf");

    FILE *f = fopen(conf_path, "r");
    if (!f) { perror("open conf"); exit(EXIT_FAILURE); }
    if (!fgets(buf, len, f)) buf[0] = '\0';
    buf[strcspn(buf, "\n")] = '\0';
    fclose(f);
}