//src/prog2/grade.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "auth.h"
#include "grade.h"
#include "subject.h"
#include "student.h"
#include "path_utils.h"

static void get_timestamp(char *buf, size_t len) {
    time_t t; struct tm *tm;
    time(&t);
    tm = localtime(&t);
    strftime(buf, len, "%Y-%m-%d %H:%M:%S", tm);
}

void input_grades(const char *subject) {
    uid_t uid = getuid();
    if (!is_root() && !is_professor(uid)) {
        fprintf(stderr, "권한 없음\n"); exit(EXIT_FAILURE);
    }
    char dept[64]; get_department(dept, sizeof(dept));
    char ids[100][32];
    int n = get_students_by_major(dept, ids, 100);
    
    // --- 수정된 부분 ---
    char path[PATH_MAX];
    char grade_filename[128];
    snprintf(grade_filename, sizeof(grade_filename), "%s.dat", subject);
    get_grade_path(path, sizeof(path), grade_filename);

    FILE *f = fopen(path, "a+");
    if (!f) { perror("open grade"); exit(EXIT_FAILURE); }
    char ts[32], buf[16];
    for (int i=0; i<n; i++) {
        printf("(%s) 점수 입력: ", ids[i]);
        if (!fgets(buf, sizeof(buf), stdin)) break;
        if (buf[0]=='\n') continue;
        int score = atoi(buf);
        get_timestamp(ts, sizeof(ts));
        fprintf(f, "%s|%d|%s\n", ids[i], score, ts);
    }
    fclose(f);
}

void edit_grades(const char *subject) {
    uid_t uid = getuid();
    if (!is_root() && !is_professor(uid)) {
        fprintf(stderr, "권한 없음\n"); exit(EXIT_FAILURE);
    }
    char path[PATH_MAX], tmp_path[PATH_MAX];
    char grade_filename[128], tmp_filename[128];
    snprintf(grade_filename, sizeof(grade_filename), "%s.dat", subject);
    snprintf(tmp_filename, sizeof(tmp_filename), "%s.tmp", subject);
    get_grade_path(path, sizeof(path), grade_filename);
    get_grade_path(tmp_path, sizeof(tmp_path), tmp_filename);

    FILE *rf = fopen(path, "r");
    FILE *wf = fopen(tmp_path, "w");
    if (!rf || !wf) { perror("open"); exit(EXIT_FAILURE); }
    char line[256];
    while (fgets(line, sizeof(line), rf)) {
        char sid[32], oldts[32]; int score;
        if (sscanf(line, "%31[^|]|%d|%31[^\"\n]", sid, &score, oldts)==3) {
            char buf[16];
            printf("수정 %s (현재 %d, Enter 유지): ", sid, score);
            fgets(buf, sizeof(buf), stdin);
            if (buf[0]=='\n') {
                fputs(line, wf);
            } else {
                int ns = atoi(buf);
                char ts2[32]; get_timestamp(ts2, sizeof(ts2));
                fprintf(wf, "%s|%d|%s\n", sid, ns, ts2);
            }
        } else {
            fputs(line, wf);
        }
    }
    fclose(rf);
    fclose(wf);
    rename(tmp_path, path);
}