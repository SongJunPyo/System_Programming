//src/common/path_utils.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h> // dirname()을 위해
#include <unistd.h>
#include "path_utils.h"

// 프로젝트 루트 경로를 저장할 전역 변수 정의
char g_project_root[PATH_MAX];

void set_project_root(const char *executable_path) {
    char actual_path[PATH_MAX];
    // 실행 파일의 실제 경로를 가져옴 (심볼릭 링크 등 해석)
    if (realpath(executable_path, actual_path) == NULL) {
        perror("realpath");
        exit(EXIT_FAILURE);
    }

    // 실행파일이 있는 디렉토리(예: /.../project/bin)를 얻어옴
    char *exec_dir = dirname(actual_path);
    
    // 거기서 한 단계 상위 디렉토리(프로젝트 루트)를 얻어옴
    strcpy(g_project_root, dirname(exec_dir));
}

void get_student_path(char *buffer, size_t size, const char *filename) {
    snprintf(buffer, size, "%s/Student/%s", g_project_root, filename);
}

void get_grade_path(char *buffer, size_t size, const char *filename) {
    snprintf(buffer, size, "%s/Grade/%s", g_project_root, filename);
}