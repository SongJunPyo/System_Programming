// include/auth.h
#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>
#include <sys/types.h>

bool is_root(void);
bool is_student(uid_t uid);
bool is_professor(uid_t uid);

#endif // AUTH_H