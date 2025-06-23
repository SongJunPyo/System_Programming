//src/common/auth.c
#include "auth.h"
#include <unistd.h>

const uid_t STUDENT_UIDS[]   = { 1016, 1017, 1018, 1019, 1020, 1021, 1022, 1023, 1024, 1025 };
const uid_t PROFESSOR_UIDS[] = { 1012, 1013, 1014, 1015 };

bool is_root(void) {
    return getuid() == 0;
}

bool is_student(uid_t uid) {
    for (size_t i = 0; i < sizeof(STUDENT_UIDS)/sizeof(uid_t); i++) {
        if (STUDENT_UIDS[i] == uid) return true;
    }
    return false;
}

bool is_professor(uid_t uid) {
    for (size_t i = 0; i < sizeof(PROFESSOR_UIDS)/sizeof(uid_t); i++) {
        if (PROFESSOR_UIDS[i] == uid) return true;
    }
    return false;
}