CXXFLAGS = -Wall -Werror -Wextra -std=c++17
LFLAGS = -lgtest -lgtest_main
CXX = g++

INSTALL_DIR := ../build

.PHONY: clean all rebuild install uninstall dvi dist

ALL_CPP_FILES := $(wildcard cpp_files/*.cpp)
HEADERS := $(wildcard cpp_files/*.hpp)

SOURCES := $(filter-out cpp_files/3d_viewer_tests.cpp, $(ALL_CPP_FILES))
TESTS := cpp_files/3d_viewer_tests.cpp

OBJECTS := $(SOURCES:.cpp=.o)
TEST_OBJECTS := $(TESTS:.cpp=.o)

ifeq ($(shell uname), Linux)
	LFLAGS = -lpthread -lgtest -lgtest_main -lsubunit -lm
endif

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

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
	$(CXX) $(OBJECTS) cpp_files/3d_viewer_tests.cpp $(LFLAGS) -o test
	./test

gcov_report: clean add_gcov_flags tests
	./test
	lcov --capture --directory . --output-file coverage.info --no-external
	genhtml coverage.info --output-directory gcov_report
	open gcov_report/index.html

lib3d_viewer.a: $(OBJECTS)
	mkdir -p $(INSTALL_DIR)
	ar -rcs $@  $(OBJECTS)
	cp ./lib3d_viewer.a $(INSTALL_DIR)

add_gcov_flags:
	$(eval CXXFLAGS += --coverage)
	$(eval LFLAGS += -lgcov)

clean:
	rm -rf *.o cpp_files/*.o test gcov_report cpp_files/*.gcda cpp_files/*.gcno coverage.info *.a a.out *.tar latex html $(INSTALL_DIR) ../3dviewer_dist 3dviewer.tar.gz

rebuild: clean all
