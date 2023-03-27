CC = g++

CFLAGS =-c -g -Wall -Wextra -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE

SRC_DIR = ./src/

all : Diff

Diff : $(SRC_DIR)differentiator.o  $(SRC_DIR)main.o  $(SRC_DIR)file_utils.o
	$(CC) $^  -o $@

differentiator.o : $(SRC_DIR)differentiator.cpp
	$(CC) $(CFLAGS) $^
	
main.o : $(SRC_DIR)main.cpp
	$(CC) $(CFLAGS) $^

file_utils.o : $(SRC_DIR) file_utils.cpp
	$(CC) $(CFLAGS) $^

clean:
	rm -rf *.o Diff

rebuild: clean all