# Makefile for 프로그램1 (객체-링크 방식)

CC      := gcc
CFLAGS  := -Wall -g -Iinclude

# 소스 디렉터리
SRCDIR_COMMON := src/common
SRCDIR_PROG1  := src/prog1
SRCDIR_PROG2  := src/prog2

# 빌드 산출물 디렉터리
BINDIR := bin
OBJDIR := obj

# 공통 소스/오브젝트
COMMON_SRC := $(SRCDIR_COMMON)/auth.c
COMMON_OBJ := $(OBJDIR)/common/auth.o

# 프로그램1 소스/오브젝트 목록
PROG1_SRCS := \
    $(SRCDIR_PROG1)/init.c \
    $(SRCDIR_PROG1)/index.c \
    $(SRCDIR_PROG1)/student.c \
    $(SRCDIR_PROG1)/edit.c \
    $(SRCDIR_PROG1)/delete.c \
    $(SRCDIR_PROG1)/initialize.c \
    $(SRCDIR_PROG1)/main.c

PROG2_SRCS := \
    $(SRCDIR_PROG2)/init.c \
    $(SRCDIR_PROG2)/subject.c \
    $(SRCDIR_PROG2)/student_selector.c \
    $(SRCDIR_PROG2)/grade.c \
    $(SRCDIR_PROG2)/main.c

PROG1_OBJS := $(patsubst $(SRCDIR_PROG1)/%.c,$(OBJDIR)/prog1/%.o,$(PROG1_SRCS))
PROG2_OBJS := $(patsubst $(SRCDIR_PROG2)/%.c,$(OBJDIR)/prog2/%.o,$(PROG2_SRCS))

# 전체 오브젝트
OBJS := $(COMMON_OBJ) $(PROG1_OBJS) $(PROG2_OBJS)

.PHONY: all clean dirs

all: dirs $(BINDIR)/prog1 $(BINDIR)/prog2

# 필수 디렉터리 생성
dirs:
	mkdir -p $(BINDIR) $(OBJDIR)/common $(OBJDIR)/prog1 $(OBJDIR)/prog2 Student Grade

# 공통 모듈
$(COMMON_OBJ): $(COMMON_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

# prog1 모듈 컴파일
$(OBJDIR)/prog1/%.o: $(SRCDIR_PROG1)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# prog2 모듈 컴파일
$(OBJDIR)/prog2/%.o: $(SRCDIR_PROG2)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# prog1 링크
$(BINDIR)/prog1: $(COMMON_OBJ) $(PROG1_OBJS)
	$(CC) $^ -o $@

# prog2 링크
$(BINDIR)/prog2: $(COMMON_OBJ) $(PROG2_OBJS)
	$(CC) $^ -o $@


clean:
	rm -rf $(BINDIR) $(OBJDIR) Student Grade