// src/prog1/index.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "auth.h"

#define INDEX_PATH "Student/Index.dat"
#define TMP_PATH    "Student/Index.tmp"

// 현재 타임스탬프 문자열 얻기
void get_timestamp(char *buf, size_t len) {
    struct tm *tm;
    time_t t;

    time(&t);
    tm = localtime(&t);
    strftime(buf, len, "%Y-%m-%d %H:%M:%S", tm);
}

// Index.dat의 모든 라인 출력
void print_index() {
    FILE *fp = fopen(INDEX_PATH, "r");
    if (!fp) {
        perror("Index.dat read");
        exit(EXIT_FAILURE);
    }
    char line[256];
    puts("---- Index.dat ----");
    while (fgets(line, sizeof(line), fp)) {
        fputs(line, stdout);
    }
    puts("-------------------");
    fclose(fp);
}

// Index에 새 레코드 추가: next_id|student_id|timestamp
void append_index(const char *student_id) {
    FILE *rf = fopen(INDEX_PATH, "r");
    FILE *wf = fopen(TMP_PATH, "w");
    if (!rf || !wf) {
        perror("Index open");
        exit(EXIT_FAILURE);
    }

    char line[256];
    int max_num = 0;
    int filled  = 0;
    // 1) 한 줄씩 보면서
    while (fgets(line, sizeof(line), rf)) {
        int num;
        // 빈 슬롯은 “숫자|” 다음에 바로 “|” 가 오는 줄
        char *p1 = strchr(line, '|');
        if (p1 && p1[1]=='|') {
            // 빈 슬롯 발견
            filled = 1;
            // 번호 파싱
            num = atoi(line);
            // 새 타임스탬프
            char ts[32];
            get_timestamp(ts, sizeof(ts));
            fprintf(wf, "%d|%s|%s\n", num, student_id, ts);
        } else {
            // 그대로 복사
            fputs(line, wf);
            // 번호 갱신
            num = atoi(line);
        }
        if (num > max_num) max_num = num;
    }

    // 2) 빈 슬롯이 없으면 맨 뒤에 새로 추가
    if (!filled) {
        char ts[32];
        get_timestamp(ts, sizeof(ts));
        fprintf(wf, "%d|%s|%s\n", max_num + 1, student_id, ts);
    }

    fclose(rf);
    fclose(wf);

    // 3) 원본 교체
    if (rename(TMP_PATH, INDEX_PATH) != 0) {
        perror("Index replace");
        exit(EXIT_FAILURE);
    }
}