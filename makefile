TARGET = bin/main
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

default: $(TARGET)
	$(TARGET) -n -f ./newFile.db

clean:
	rm -f obj/*.o
	rm -f src/*

$(TARGET): $(OBJ)
	gcc -o $@ $?

obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude