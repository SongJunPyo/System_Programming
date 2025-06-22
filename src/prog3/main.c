#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include "auth.h"
#include "path_utils.h"
#include "view.h"
#include "subject.h"
#include "student.h"

int main(int argc, char *argv[]) {
    set_project_root(argv[0]);

    uid_t uid = getuid();

    // 1. 관리자(root)일 경우 -> 반복 조회 메뉴 제공
    if (is_root()) {
        while (1) {
            char student_id[32];
            printf("\n정보를 조회할 학생의 학번을 입력하세요 (종료: exit): ");
            if (scanf("%31s", student_id) != 1) break;

            // "exit"를 입력하면 루프 종료
            if (strcmp(student_id, "exit") == 0) {
                break;
            }

            printf("\n'%s'님의 정보를 조회합니다. (관리자 권한)\n", student_id);
            print_student_info(student_id);
            print_student_grades(student_id);
        }
    }
    // 2. 교수일 경우 -> 반복 조회 메뉴 제공
    else if (is_professor(uid)) {
        while (1) {
            char dept[64];
            get_department(dept, sizeof(dept));

            char ids[100][32];
            int n = get_students_by_major(dept, ids, 100);
            if (n == 0) {
                printf("'%s' 학과에 소속된 학생이 없습니다.\n", dept);
                break;
            }

            printf("\n=== [%s] 학과 학생 목록 ===\n", dept);
            for(int i=0; i<n; i++) {
                printf("%d. %s\n", i + 1, ids[i]);
            }
            printf("---------------------------------\n");
            printf("정보를 조회할 학생의 번호를 입력하세요 (종료: 0): ");
            
            int choice;
            if (scanf("%d", &choice) != 1) {
                // 숫자가 아닌 입력 처리
                while(getchar()!='\n'); // 버퍼 비우기
                choice = -1; // 잘못된 선택으로 유도
            }
            
            if (choice == 0) {
                break; // 0을 입력하면 루프 종료
            } else if (choice < 1 || choice > n) {
                printf("잘못된 선택입니다.\n");
                continue; // 루프의 처음으로 돌아가 학생 목록 다시 출력
            }
            
            const char *student_id = ids[choice - 1];
            printf("\n'%s'님의 정보를 조회합니다. (교수 권한)\n", student_id);
            print_student_grades(student_id);
        }
    }
    // 3. 학생일 경우 -> 정보 한 번만 출력하고 종료
    else if (is_student(uid)) {
        struct passwd *pw = getpwuid(uid);
        if (!pw) {
            fprintf(stderr, "Error: 사용자 정보를 가져올 수 없습니다.\n");
            return EXIT_FAILURE;
        }
        const char *student_id = pw->pw_name;

        printf("\n'%s'님의 정보를 조회합니다.\n", student_id);
        print_student_info(student_id);
        print_student_grades(student_id);
    }
    // 4. 등록되지 않은 사용자일 경우
    else {
        fprintf(stderr, "Error: 시스템에 등록된 사용자가 아닙니다.\n");
        return EXIT_FAILURE;
    }

    printf("\n프로그램을 종료합니다.\n");
    return EXIT_SUCCESS;
}