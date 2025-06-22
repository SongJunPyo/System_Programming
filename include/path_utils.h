//include/path_utils.h
#ifndef PATH_UTILS_H
#define PATH_UTILS_H

#include <linux/limits.h> // PATH_MAX를 위해

// 프로젝트 루트 경로를 저장할 전역 변수 선언
extern char g_project_root[PATH_MAX];

// 프로그램 실행 시 최초 한 번 호출하여 루트 경로 설정
void set_project_root(const char *executable_path);

// Student 폴더 내의 파일 경로를 만들어주는 함수
void get_student_path(char *buffer, size_t size, const char *filename);

// Grade 폴더 내의 파일 경로를 만들어주는 함수
void get_grade_path(char *buffer, size_t size, const char *filename);

#endif // PATH_UTILS_H