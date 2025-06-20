#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "auth.h"
#include "subject.h"
#include "grade.h"

void init_grade_system(void);

int main(void) {
    uid_t uid = getuid();
    if (!is_root() && !is_professor(uid)) {
        fprintf(stderr, "Error: 권한 없음 (UID=%d)\n", uid);
        return EXIT_FAILURE;
    }

    init_grade_system();

    struct stat st;
    if (stat("Grade", &st)==-1) {
        printf("초기화가 필요합니다. 먼저 init을 실행하세요.\n");
        return EXIT_FAILURE;
    }

    char dept[64];
    get_department(dept, sizeof(dept));

    while (1) {
        char subjects[100][64];
        int n = list_subjects(subjects, 100);
        printf("\n=== 성적 관리 (%s) ===\n", dept);
        for (int i=0;i<n;i++) printf("%2d. %s\n", i+1, subjects[i]);
        printf("%2d. 과목 추가\n%2d. 과목 제거\n%2d. 종료\n", n+1, n+2, 0);
        printf("선택> ");
        int c; if (scanf("%d", &c)!=1) break;
        getchar();
        if (c==0) break;
        else if (c==n+1) {
            char name[64]; printf("신규 과목명: "); fgets(name,64,stdin);
            name[strcspn(name,"\n")]='\0'; add_subject(name);
        }
        else if (c==n+2) {
            char name[64]; printf("삭제 과목명: "); fgets(name,64,stdin);
            name[strcspn(name,"\n")]='\0'; remove_subject(name);
        }
        else if (c>=1 && c<=n) {
            printf("선택: %s (1:입력 2:수정)\n", subjects[c-1]);
            int m; scanf("%d", &m); getchar();
            if (m==1) input_grades(subjects[c-1]);
            else if (m==2) edit_grades(subjects[c-1]);
        }
    }
    return EXIT_SUCCESS;
}