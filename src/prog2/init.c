#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "auth.h"
#include "subject.h"

#define GRADE_DIR   "Grade"
#define CONF_FILE   GRADE_DIR "/department.conf"

int init_grade_system(void) {
    uid_t uid = getuid();
    if (!is_root() && !is_professor(uid)) {
        fprintf(stderr, "Error: 교수 또는 관리자만 실행할 수 있습니다 (UID=%d)\n", uid);
        return EXIT_FAILURE;
    }

    struct stat st;
    if (stat(GRADE_DIR, &st) == -1) {
        if (errno != ENOENT) { perror("stat Grade"); return EXIT_FAILURE; }
        if (mkdir(GRADE_DIR, 0755) == -1) { perror("mkdir Grade"); return EXIT_FAILURE; }
        printf("Created: %s/\n", GRADE_DIR);
    }

    char dept[64];
    printf("학과를 입력하세요: ");
    if (!fgets(dept, sizeof(dept), stdin)) return EXIT_FAILURE;
    dept[strcspn(dept, "\n")] = '\0';

    FILE *cf = fopen(CONF_FILE, "w");
    if (!cf) { perror("fopen department.conf"); return EXIT_FAILURE; }
    fprintf(cf, "%s\n", dept);
    fclose(cf);
    printf("Saved department: %s\n", dept);

    int n;
    printf("담당 교과목 수를 입력하세요: ");
    if (scanf("%d", &n) != 1 || n <= 0) { fprintf(stderr,"Invalid count\n"); return EXIT_FAILURE; }
    getchar();

    for (int i = 1; i <= n; i++) {
        char name[64];
        printf("%d번째 교과목 이름: ", i);
        if (!fgets(name, sizeof(name), stdin)) return EXIT_FAILURE;
        name[strcspn(name, "\n")] = '\0';
        add_subject(name);
        printf("Created: %s/%s.dat\n", GRADE_DIR, name);
    }
    printf("초기화 완료: %d 과목 생성됨.\n", n);
    return EXIT_SUCCESS;
}