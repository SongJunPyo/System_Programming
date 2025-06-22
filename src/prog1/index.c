// src/prog1/index.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "auth.h"
#include "path_utils.h"

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
    char index_path[PATH_MAX];
    get_student_path(index_path, sizeof(index_path), "Index.dat");

    FILE *fp = fopen(index_path, "r");
    if (!fp) {
        perror("Index.dat read");
        exit(EXIT_FAILURE);
    }
    char line[256];
    puts("---- Index.dat ----");
    while (fgets(line, sizeof(line), fp)) {
        fputs(line, stdout);
    }
    puts("----Complete----");
    fclose(fp);
}

// Index에 새 레코드 추가: next_id|student_id|timestamp
void append_index(const char *student_id) {
    char index_path[PATH_MAX];
    char temp_path[PATH_MAX];
    get_student_path(index_path, sizeof(index_path), "Index.dat");
    get_student_path(temp_path, sizeof(temp_path), "Index.tmp");

    FILE *rf = fopen(index_path, "r");
    FILE *wf = fopen(temp_path, "w");
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
    if (rename(temp_path, index_path) != 0) {
        perror("Index replace");
        exit(EXIT_FAILURE);
    }
}