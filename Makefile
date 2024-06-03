cc= gcc
flags= -Wextra -Werror -Wall -std=c11
all: s21_grep

s21_cat:clean
	$(cc) $(flags) s21_grep.c -o s21_grep

clean:
	rm -rf s21_grep



