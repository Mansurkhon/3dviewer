CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -g
OS = $(shell uname)
MODULES = $(wildcard modules/*.c)
OBJECTS = $(patsubst %.c, %.o, $(MODULES))
TARGET = 3dViewer
TEST_MODULES = test_modules.o

ifeq ($(OS), Linux)
	LDLIBS = -lcheck_pic -pthread -lm -lrt -lsubunit
else
	LDLIBS = -lcheck -lpthread -pthread
endif

all: clean install

%.o: %.c
	mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $(patsubst modules%, obj%, $@)

install:
	@mkdir -p ../build
	@cd 3dViewer && qmake && make &> /dev/null && make clean &> /dev/null && rm -rf Makefile && mv 3dViewer.app ../../build/$(TARGET).app
	@cp -R ../build/$(TARGET).app ~/Desktop/$(TARGET).app

uninstall:
	@rm -rf ~/Desktop/$(TARGET).app ../build

dvi:
	@open ../README_RUS.md

dist:
	@mkdir -p ../dist
	@cp -R ../build/$(TARGET).app ../dist
	@cp ../README_RUS.md ../dist && cp ../README.md ../dist
	@cd ../dist && tar cvzf $(TARGET)_v1.0.tgz *
	@cd ../dist && rm -rf $(TARGET).app README.md README_RUS.md

test: $(OBJECTS)
	@$(CC) $(CFLAGS) ./test_lib/unit_tests.c $(LDLIBS) ./obj/*.o -o $(TEST_MODULES)
	@./$(TEST_MODULES)

gcov_report: $(OBJECTS)
	@$(CC) --coverage $(CFLAGS) ./test_lib/unit_tests.c $(LDLIBS) ./modules/*.c -o ./$(TEST_MODULES)
	@./$(TEST_MODULES)
	@mkdir -p modules_gcov
	@mv *.gcno ./modules_gcov/ && mv *.gcda ./modules_gcov/
	@lcov -t "test_coverage" -o test_coverage.info -c -d .
	@genhtml -o report test_coverage.info
	@open report/index.html
	@rm -rf *.gcda *.gcno report/test_coverage.info modules_gcov

valgrind_check: test
	@CK_FORK=no valgrind --vgdb=no --leak-check=full \
	--show-leak-kinds=all --track-origins=yes --log-file="unit_tests_vg.log" -v --verbose -q --quiet -s $(TEST_MODUlES)

clean:
	@rm -rf *.o obj *.dSYM **/*.dSYM report test_coverage.info

fclean: clean
	@rm -rf ../build ../dist ~/Desktop/$(TARGET).app

format_style:
	@cp ../materials/linters/.clang-format .
	@clang-format -i **/*.c **/*.h 3dViewer/*.h 3dViewer/*.cpp
	@rm ./.clang-format

style_check:
	@cp ../materials/linters/.clang-format .
	@clang-format -n **/*.c **/*.h 3dViewer/*.h 3dViewer/*.cpp
	@rm ./.clang-format
