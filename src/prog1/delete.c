// src/prog1/delete.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "auth.h"

#define STUDENT_DIR "Student"
#define INDEX_PATH STUDENT_DIR "/Index.dat"
#define TEMP_PATH STUDENT_DIR "/Index.tmp"

void delete_student(void) {
    if (!is_root()) {
        fprintf(stderr, "Error: 관리자만 실행 가능합니다.\n");
        exit(EXIT_FAILURE);
    }

    char id[32], path[128];
    printf("삭제할 학생 학번: ");
    if (scanf("%31s", id) != 1) return;

    // 1) 학생 정보 파일 삭제
    snprintf(path, sizeof(path), "%s/%s.dat", STUDENT_DIR, id);
    if (remove(path) == 0) {
        printf("삭제됨: %s\n", path);
    } else {
        fprintf(stderr, "파일 삭제 실패: %s\n", path);
        return;
    }

    // 2) Index.dat 업데이트 (학번,시간 공백)
    FILE *rf = fopen(INDEX_PATH, "r");
    FILE *wf = fopen(TEMP_PATH, "w");
    if (!rf || !wf) { perror("Index 파일 열기"); exit(EXIT_FAILURE); }

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
    if (rename(TEMP_PATH, INDEX_PATH) != 0) {
        perror("Index 갱신 실패");
        exit(EXIT_FAILURE);
    }
    printf("Index.dat 에서도 삭제 처리되었습니다.\n");
}
