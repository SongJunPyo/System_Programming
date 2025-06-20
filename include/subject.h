#ifndef SUBJECT_H
#define SUBJECT_H

#include <stddef.h>

// list 모든 과목명을 subjects 배열에 담고 개수 반환
int list_subjects(char subjects[][64], int max);

// 새로운 과목 파일 생성
void add_subject(const char *name);

// 해당 과목 파일 삭제
void remove_subject(const char *name);

// 저장된 학과명 읽기
void get_department(char *buf, size_t len);

#endif // SUBJECT_H