#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "path_utils.h"

// 학생 개인 정보 출력 함수
void print_student_info(const char *student_id) {
    char path[PATH_MAX];
    char filename[64];
    snprintf(filename, sizeof(filename), "%s.dat", student_id);
    get_student_path(path, sizeof(path), filename);

    FILE *fp = fopen(path, "r");
    if (!fp) {
        printf("개인 정보를 찾을 수 없습니다: %s\n", path);
        return;
    }

    printf("\n=== 개인 정보 ===\n");
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}

// 학생 성적 정보 출력 함수
void print_student_grades(const char *student_id) {
    char grade_dir_path[PATH_MAX];
    snprintf(grade_dir_path, sizeof(grade_dir_path), "%s/Grade", g_project_root);

    DIR *d = opendir(grade_dir_path);
    if (!d) return;

    printf("\n=== 성적 정보 ===\n");
    struct dirent *e;
    // Grade 디렉토리의 모든 파일을 순회
    while ((e = readdir(d))) {
        if (strstr(e->d_name, ".dat")) { // .dat 파일만 대상으로 함
            char subject_path[PATH_MAX];
            get_grade_path(subject_path, sizeof(subject_path), e->d_name);
            
            FILE *fp = fopen(subject_path, "r");
            if (!fp) continue;

            // 과목 파일의 각 줄을 읽어 학번 비교
            char line[256];
            while (fgets(line, sizeof(line), fp)) {
                char sid[32];
                int score;
                // "학번|점수|..." 형식에서 학번 파싱
                if (sscanf(line, "%31[^|]|%d", sid, &score) == 2) {
                    if (strcmp(sid, student_id) == 0) {
                        // 과목명 추출 (예: "알고리즘.dat" -> "알고리즘")
                        char subject_name[64];
                        strncpy(subject_name, e->d_name, sizeof(subject_name));
                        *strrchr(subject_name, '.') = '\0';
                        
                        printf("%-20s: %d점\n", subject_name, score);
                    }
                }
            }
            fclose(fp);
        }
    }
    closedir(d);
}