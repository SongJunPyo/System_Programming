// src/prog1/edit.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "auth.h"

#define STUDENT_DIR   "Student"
#define INDEX_PATH    STUDENT_DIR "/Index.dat"

// index.c에서 제공하는 함수
void init_index(void);
void print_index(void);
void append_index(const char *);
void get_timestamp(char *buf, size_t len);

// index.c 내부에서만 쓰던 get_timestamp 재선언
static void update_index_timestamp(const char *student_id) {
    FILE *f = fopen(INDEX_PATH, "r+");
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
            // 새 타임스탬프 생성
            get_timestamp(ts, sizeof(ts));
            // 덮어쓸 문자열 준비
            snprintf(out, sizeof(out), "%d|%s|%s\n", num, sid, ts);
            // 파일 포인터를 해당 라인 시작으로 되돌림
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

    // 데이터 파일 경로
    char path[128];
    snprintf(path, sizeof(path), "%s/%s.dat", STUDENT_DIR, id);

    // 원본 파일 열기
    FILE *fp = fopen(path, "r+");
    if (!fp) {
        fprintf(stderr, "학생 정보 (%s) 가 없습니다.\n", path);
        return;
    }
    fclose(fp);

    // 새 정보 입력
    char name[64], dob[16], major[64], status[16];
    printf("새 이름: ");      scanf("%63s", name);
    printf("새 생년월일: "); scanf("%15s", dob);
    printf("새 학과: ");      scanf("%63s", major);
    printf("새 상태: ");      scanf("%15s", status);

    // 덮어쓰기
    fp = fopen(path, "w");
    if (!fp) { perror("fopen"); exit(EXIT_FAILURE); }
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

