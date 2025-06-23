# Makefile (수정 완료된 최종본)

CC      := gcc
# CFLAGS에 -Iinclude 를 정의하여 컴파일 시 항상 include 디렉토리를 참조하도록 함
CFLAGS  := -g -Iinclude

# 소스 디렉터리
SRCDIR_COMMON := src/common
SRCDIR_PROG1  := src/prog1
SRCDIR_PROG2  := src/prog2
SRCDIR_PROG3  := src/prog3

# 빌드 산출물 디렉터리
BINDIR := bin
OBJDIR := obj

# 공통 소스/오브젝트
COMMON_SRCS := $(SRCDIR_COMMON)/auth.c $(SRCDIR_COMMON)/path_utils.c
COMMON_OBJS := $(patsubst $(SRCDIR_COMMON)/%.c,$(OBJDIR)/common/%.o,$(COMMON_SRCS))

# 프로그램1
PROG1_SRCS := \
    $(SRCDIR_PROG1)/init.c \
    $(SRCDIR_PROG1)/index.c \
    $(SRCDIR_PROG1)/student.c \
    $(SRCDIR_PROG1)/edit.c \
    $(SRCDIR_PROG1)/delete.c \
    $(SRCDIR_PROG1)/initialize.c \
    $(SRCDIR_PROG1)/main.c

# 프로그램2 
PROG2_SRCS := \
    $(SRCDIR_PROG2)/init.c \
    $(SRCDIR_PROG2)/subject.c \
    $(SRCDIR_PROG2)/student_selector.c \
    $(SRCDIR_PROG2)/grade.c \
    $(SRCDIR_PROG2)/main.c

# 프로그램3 
PROG3_SRCS := \
    $(SRCDIR_PROG3)/view.c \
    $(SRCDIR_PROG3)/main.c

PROG1_OBJS := $(patsubst $(SRCDIR_PROG1)/%.c,$(OBJDIR)/prog1/%.o,$(PROG1_SRCS))
PROG2_OBJS := $(patsubst $(SRCDIR_PROG2)/%.c,$(OBJDIR)/prog2/%.o,$(PROG2_SRCS))
PROG3_OBJS := $(patsubst $(SRCDIR_PROG3)/%.c,$(OBJDIR)/prog3/%.o,$(PROG3_SRCS))


.PHONY: all clean dirs

all: dirs $(BINDIR)/prog1 $(BINDIR)/prog2 $(BINDIR)/prog3

# 필수 디렉터리 생성
dirs:
	mkdir -p $(BINDIR) $(OBJDIR)/common $(OBJDIR)/prog1 $(OBJDIR)/prog2 $(OBJDIR)/prog3 Student Grade 

# 공통 모듈 컴파일 규칙 (각 .c 파일이 여기서 컴파일됨)
$(OBJDIR)/common/%.o: $(SRCDIR_COMMON)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# prog1 모듈 컴파일 규칙
$(OBJDIR)/prog1/%.o: $(SRCDIR_PROG1)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# prog2 모듈 컴파일 규칙
$(OBJDIR)/prog2/%.o: $(SRCDIR_PROG2)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# prog3 모듈 컴파일 규칙 추가
$(OBJDIR)/prog3/%.o: $(SRCDIR_PROG3)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# prog1 링크 규칙 (컴파일된 .o 파일들을 링크함)
$(BINDIR)/prog1: $(COMMON_OBJS) $(PROG1_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# prog2 링크 규칙
$(BINDIR)/prog2: $(COMMON_OBJS) $(PROG2_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# prog3 링크 규칙 추가
$(BINDIR)/prog3: $(COMMON_OBJS) $(PROG3_OBJS) $(OBJDIR)/prog2/subject.o $(OBJDIR)/prog2/student_selector.o
	$(CC) $(CFLAGS) $^ -o $@


clean:
	rm -rf $(BINDIR) $(OBJDIR) Student Grade

install: all
	sudo chown root:root $(BINDIR)/prog1 $(BINDIR)/prog2 $(BINDIR)/prog3
	sudo chmod 4755 $(BINDIR)/prog1 $(BINDIR)/prog2  $(BINDIR)/prog3
