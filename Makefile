CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -rdynamic `pkg-config --cflags gtk+-3.0 2>/dev/null || echo ""`
LIBS = `pkg-config --libs gtk+-3.0 2>/dev/null || echo ""`

SRC = src/main.c src/membres.c src/entraineurs.c src/cours.c src/equipements.c src/centres.c src/evenements.c src/utils.c
OBJ = $(SRC:.c=.o)
TARGET = app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo "✓ Compilation réussie: $@"

crud-test: src/crud_test.c src/membres.c src/centres.c src/utils.c
	$(CC) -Wall -Wextra -std=c99 -o crud_test src/crud_test.c src/membres.c src/centres.c src/utils.c
	@echo "✓ Binaire de test CRUD: crud_test"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) app app-test crud_test *.o src/*.o

run: clean $(TARGET)
	./$(TARGET)

.PHONY: all clean run


