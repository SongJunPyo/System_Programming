#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "auth.h"
#include "subject.h"
#include "grade.h"
#include "path_utils.h"

// 외부 함수 선언
void init_grade_system(void);

// main 함수만 남기고 flush_stdin 함수는 이제 필요 없으므로 제거합니다.

int main(int argc, char *argv[]) {
    set_project_root(argv[0]);

    uid_t uid = getuid();
    if (!is_root() && !is_professor(uid)) {
        fprintf(stderr, "Error: 권한 없음 (UID=%d)\n", uid);
        return EXIT_FAILURE;
    }

    char conf_path[PATH_MAX];
    get_grade_path(conf_path, sizeof(conf_path), "department.conf");

    if (access(conf_path, F_OK) == -1) {
        printf("초기 설정 파일(department.conf)이 없습니다. 초기화를 시작합니다.\n");
        init_grade_system();
    }

    char dept[64];
    get_department(dept, sizeof(dept));

    while (1) { // 메인 메뉴 루프
        char subjects[100][64];
        int n = list_subjects(subjects, 100);
        printf("\n=== 성적 관리 (%s) ===\n", dept);
        for (int i=0; i<n; i++) printf("%2d. %s\n", i+1, subjects[i]);
        printf("--------------------------\n");
        printf("%2d. 과목 추가\n", n + 1);
        printf("%2d. 과목 제거\n", n + 2);
        printf("%2d. 종료\n", 0);
        printf("선택> ");
        
        // --- scanf를 fgets와 atoi로 대체 ---
        char input_buffer[16];
        if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
            break; // EOF (Ctrl+D) 처리
        }
        int c = atoi(input_buffer);

        if (c == 0) {
            break;
        } else if (c == n + 1) {
            char name[64]; printf("신규 과목명: "); fgets(name, 64, stdin);
            name[strcspn(name, "\n")] = '\0'; add_subject(name);
        } else if (c == n + 2) {
            char name[64]; printf("삭제 과목명: "); fgets(name, 64, stdin);
            name[strcspn(name, "\n")] = '\0'; remove_subject(name);
        } else if (c >= 1 && c <= n) {
            char selected_subject[64];
            strcpy(selected_subject, subjects[c-1]);

            // 하위 메뉴 루프
            while (1) {
                printf("\n--- [%s] 과목 작업 ---\n", selected_subject);
                printf("1. 교과목 삭제\n");
                printf("2. 성적 입력\n");
                printf("3. 성적 수정\n");
                printf("0. 이전 메뉴로 돌아가기\n");
                printf("선택> ");

                // --- 여기도 scanf를 fgets와 atoi로 대체 ---
                if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
                    break; // EOF (Ctrl+D) 처리
                }
                int m = atoi(input_buffer);

                if(m == 1){
                    remove_subject(selected_subject);
                    printf("과목 [%s] 가 삭제되었습니다.\n", selected_subject);
                    break; 
                } else if (m == 2) {
                    input_grades(selected_subject);
                } else if (m == 3) {
                    edit_grades(selected_subject);
                } else if (m == 0) {
                    break;
                } else {
                    printf("잘못된 선택입니다.\n");
                }
            }
        }
    }
    return EXIT_SUCCESS;
}