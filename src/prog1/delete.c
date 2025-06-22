// src/prog1/delete.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "auth.h"
#include "path_utils.h" // 경로 유틸리티 헤더 추가

// #define 들은 이제 필요 없으므로 삭제합니다.

void delete_student(void) {
    if (!is_root()) {
        fprintf(stderr, "Error: 관리자만 실행 가능합니다.\n");
        exit(EXIT_FAILURE);
    }

    char id[32];
    printf("삭제할 학생 학번: ");
    if (scanf("%31s", id) != 1) return;

    // --- 수정된 부분 ---
    char student_dat_path[PATH_MAX];
    char student_filename[64];
    snprintf(student_filename, sizeof(student_filename), "%s.dat", id);
    get_student_path(student_dat_path, sizeof(student_dat_path), student_filename);

    // 1) 학생 정보 파일 삭제
    if (remove(student_dat_path) == 0) {
        printf("삭제됨: %s\n", student_dat_path);
    } else {
        fprintf(stderr, "파일 삭제 실패: %s\n", student_dat_path);
        return;
    }

    // --- 수정된 부분 ---
    char index_path[PATH_MAX];
    char temp_path[PATH_MAX];
    get_student_path(index_path, sizeof(index_path), "Index.dat");
    get_student_path(temp_path, sizeof(temp_path), "Index.tmp");

    // 2) Index.dat 업데이트 (학번,시간 공백)
    FILE *rf = fopen(index_path, "r");
    FILE *wf = fopen(temp_path, "w");
    if (!rf || !wf) { 
        perror("Index 파일 열기"); 
        exit(EXIT_FAILURE); 
    }

    char line[256];
    while (fgets(line, sizeof(line), rf)) {
        int num;
        char sid[32], ts[32];
        if (sscanf(line, "%d|%31[^|]|%31[^\n]", &num, sid, ts) == 3
            && strcmp(sid, id) == 0) {
            // 삭제된 레코드는 학번·시간 공백으로 유지
            fprintf(wf, "%d||\n", num);
        } else {
            fputs(line, wf);
        }
    }
    fclose(rf);
    fclose(wf);

    // 원본 덮어쓰기
    if (rename(temp_path, index_path) != 0) {
        perror("Index 갱신 실패");
        exit(EXIT_FAILURE);
    }
    printf("Index.dat 에서도 삭제 처리되었습니다.\n");
}