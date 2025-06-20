// include/student.h
#ifndef STUDENT_H
#define STUDENT_H
#include <stddef.h>

// Student/<id>.dat 생성
void create_student_file(
    const char *id,
    const char *name,
    const char *dob,
    const char *major,
    const char *status
);

// department와 일치하는 학생ID 목록을 ids[][]에 담고 개수 반환
int get_students_by_major(const char *major, char ids[][32], int max);

#endif // STUDENT_H