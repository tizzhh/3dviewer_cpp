CFLAGS = -Wall -Werror -Wextra -std=c11 -pedantic
LFLAGS = -lcheck
CC = gcc

INSTALL_DIR := ../build

.PHONY: clean all rebuild install uninstall dvi dist

ALL_C_FILES := $(wildcard c_files/*.c)

SOURCES := $(filter-out c_files/3d_viewer_tests.c, $(ALL_C_FILES))
TESTS := c_files/3d_viewer_tests.c

OBJECTS := $(SOURCES:.c=.o)
TEST_OBJECTS := $(TESTS:.c=.o)

ifeq ($(shell uname), Linux)
	LFLAGS = -lpthread -lcheck -lsubunit -lm
endif

%.o: %.c 3d_viewer.h
	$(CC) $(CFLAGS) -c $< -o $@

all: tests

install: clean lib3d_viewer.a
	mkdir -p $(INSTALL_DIR)
	cd $(INSTALL_DIR) && cmake ../src/Frontend && make

uninstall:
	rm -rf $(INSTALL_DIR)

dvi:
	doxygen Doxyfile
	open html/index.html

dist: install
	mkdir -p ../3dviewer_dist
	cp -r ../src ../3dviewer_dist
	cp -r ../build ../3dviewer_dist
	tar --totals -czvf 3dviewer.tar.gz ../3dviewer_dist
	rm -rf ../3dviewer_dist

tests: $(TEST_OBJECTS) $(OBJECTS)
	$(CC) $(OBJECTS) c_files/3d_viewer_tests.c $(LFLAGS) -o test
	./test

gcov_report: clean add_gcov_flags tests
	./test
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory gcov_report
	open gcov_report/index.html

lib3d_viewer.a: $(OBJECTS)
	mkdir -p $(INSTALL_DIR)
	ar -rcs $@  $(OBJECTS)
	cp ./lib3d_viewer.a $(INSTALL_DIR)

add_gcov_flags:
	$(eval CFLAGS += --coverage)
	$(eval LFLAGS += -lgcov)

clean:
	rm -rf *.o c_files/*.o test gcov_report c_files/*.gcda c_files/*.gcno coverage.info *.a a.out *.tar latex html $(INSTALL_DIR) ../3dviewer_dist 3dviewer.tar.gz

rebuild: clean all
