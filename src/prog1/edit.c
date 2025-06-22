// src/prog1/edit.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "auth.h"
#include "path_utils.h" // 경로 유틸리티 헤더 추가

// index.c에서 제공하는 함수 선언
void get_timestamp(char *buf, size_t len);

// index.c 내부에서만 쓰던 get_timestamp 재선언
static void update_index_timestamp(const char *student_id) {
    // --- 수정된 부분 ---
    char index_path[PATH_MAX];
    get_student_path(index_path, sizeof(index_path), "Index.dat");

    FILE *f = fopen(index_path, "r+");
    if (!f) { perror("Index.dat open"); exit(EXIT_FAILURE); }

    char line[256], out[256];
    char ts[32];
    while (fgets(line, sizeof(line), f)) {
        int num;
        char sid[64], oldts[32];
        if (sscanf(line, "%d|%63[^|]|%31[^\n]", &num, sid, oldts) != 3) {
            continue;
        }
        if (strcmp(sid, student_id) == 0) {
            get_timestamp(ts, sizeof(ts));
            snprintf(out, sizeof(out), "%d|%s|%s\n", num, sid, ts);
            fseek(f, -strlen(line), SEEK_CUR);
            fputs(out, f);
            break;
        }
    }
    fclose(f);
}

void edit_student(void) {
    if (!is_root()) {
        fprintf(stderr, "Error: 관리자만 실행 가능합니다.\n");
        exit(EXIT_FAILURE);
    }

    char id[32];
    printf("수정할 학생 학번: ");
    if (scanf("%31s", id) != 1) return;

    // --- 수정된 부분 ---
    char path[PATH_MAX];
    char filename[64];
    snprintf(filename, sizeof(filename), "%s.dat", id);
    get_student_path(path, sizeof(path), filename);

    // 원본 파일 존재 확인
    if (access(path, F_OK) != 0) {
        fprintf(stderr, "학생 정보 (%s) 가 없습니다.\n", path);
        return;
    }

    // 새 정보 입력
    char name[64], dob[16], major[64], status[16];
    printf("새 이름: ");       
    scanf("%63s", name);
    printf("새 생년월일: "); 
    scanf("%15s", dob);
    printf("새 학과: ");       
    scanf("%63s", major);
    printf("새 상태: ");       
    scanf("%15s", status);

    // 덮어쓰기
    FILE *fp = fopen(path, "w");
    if (!fp) { 
        perror("fopen"); 
        exit(EXIT_FAILURE); 
    }
    fprintf(fp,
        "학번: %s\n"
        "이름: %s\n"
        "생년월일: %s\n"
        "학과: %s\n"
        "상태: %s\n",
        id, name, dob, major, status);
    fclose(fp);

    // Index.dat 타임스탬프 갱신
    update_index_timestamp(id);
    printf("학생 정보가 수정되었습니다.\n");
}

// 모든 정보를 수정하지 말고 원하는 정보만 수정할 수 있도록 변경