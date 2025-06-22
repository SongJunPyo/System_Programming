//src/prog2/init.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "auth.h"
#include "subject.h"
#include "path_utils.h"

int init_grade_system(void) {
    uid_t uid = getuid();
    if (!is_root() && !is_professor(uid)) {
        fprintf(stderr, "Error: 교수 또는 관리자만 실행할 수 있습니다 (UID=%d)\n", uid);
        return EXIT_FAILURE;
    }

    char grade_dir_path[PATH_MAX];
    snprintf(grade_dir_path, sizeof(grade_dir_path), "%s/Grade", g_project_root);

    struct stat st;
    if (stat(grade_dir_path, &st) == -1) {
        if (errno != ENOENT) { perror("stat Grade"); return EXIT_FAILURE; }
        if (mkdir(grade_dir_path, 0755) == -1) { perror("mkdir Grade"); return EXIT_FAILURE; }
        printf("Created: %s/\n", grade_dir_path);
    }

    char dept[64];
    printf("학과를 입력하세요: ");
    if (!fgets(dept, sizeof(dept), stdin)) return EXIT_FAILURE;
    dept[strcspn(dept, "\n")] = '\0';

    char conf_path[PATH_MAX];
    get_grade_path(conf_path, sizeof(conf_path), "department.conf");

    FILE *cf = fopen(conf_path, "w");
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
        printf("Created: %s/%s.dat\n", grade_dir_path, name);
    }
    printf("초기화 완료: %d 과목 생성됨.\n", n);
    return EXIT_SUCCESS;
}