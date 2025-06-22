// src/prog1/main.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "auth.h"
#include "student.h"
#include "path_utils.h"

// 외부 모듈 함수 선언
void ensure_student_dir_and_index(void);  // init.c
void print_index(void);                   // index.c
void append_index(const char *student_id);// index.c

void edit_student(void);                  // edit.c
void delete_student(void);                // delete.c
void initialize_all(void);                // initialize.c

int main(int argc, char *argv[]) {
    set_project_root(argv[0]);

    if (!is_root()) {
        fprintf(stderr, "Error: 관리자만 실행 가능합니다 (UID=%d)\n", getuid());
        return EXIT_FAILURE;
    }

    // 0. 최초 실행: Student 디렉토리 및 Index.dat 준비
    ensure_student_dir_and_index();
    print_index();

    while (1) {
        int choice;
        printf("\n=== 학생 정보 관리 ===\n");
        printf("1. 학생 정보 입력/삭제 모드\n");   // ← 삭제를 포함
        printf("2. 학생 정보 수정 모드\n");
        printf("3. 초기화 (Student/Grade 폴더 삭제)\n");
        printf("0. 종료\n");
        printf("선택> ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: {
            int sub;
            printf("\n-- 입력/삭제 모드 --\n");
            printf(" 1. 학생 정보 입력\n");
            printf(" 2. 학생 정보 삭제\n");
            printf(" 0. 이전\n");
            printf(" 선택> ");
            scanf("%d", &sub);
            switch (sub) {
            case 1: {
                char id[32], name[64], dob[16], major[64], status[16];
                printf(" 학번: ");     scanf("%31s", id);
                printf(" 이름: ");     scanf("%63s", name);
                printf(" 생년월일: "); scanf("%15s", dob);
                printf(" 학과: ");     scanf("%63s", major);
                printf(" 학적상태: "); scanf("%15s", status);
                create_student_file(id, name, dob, major, status);
                append_index(id);
                printf(" → 학생 정보가 추가되었습니다.\n");
                break;
            }
            case 2:
                delete_student();
                break;
            case 0:
                break;
            default:
                printf(" 1,2,0 중 하나만 선택 가능합니다.\n");
            }
            break;
        }
        case 2:
            edit_student();
            break;
        case 3:
            printf("정말 초기화하시겠습니까? (y/n): ");
            getchar(); // 앞 scanf 개행 제거
            if (getchar() == 'y') { 
                initialize_all(); 
                printf("초기화 완료.\n"); 
            }
            else {
                printf("초기화 취소.\n");
            }
            break;
        case 0:
            printf("프로그램을 종료합니다.\n");
            return EXIT_SUCCESS;
        default:
            printf("1~3 또는 0만 선택 가능합니다.\n");
        }
    }

}
