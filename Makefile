CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -Wextra -std=c99
LIBS = `pkg-config --libs gtk+-3.0`

# Try to detect if GTK3 is available
GTK_AVAILABLE := $(shell pkg-config gtk+-3.0 --modversion 2>/dev/null)

ifeq ($(GTK_AVAILABLE),)
  # GTK3 not available, use test mode
  TEST_MODE = 1
  CFLAGS = -Wall -Wextra -std=c99
  LIBS = 
  SRC = src/test_main.c src/utils.c
  TARGET = app-test
else
  # GTK3 available, normal mode
  SRC = src/main.c src/membres.c src/entraineurs.c src/cours.c src/equipements.c src/centres.c src/evenements.c src/utils.c
  TARGET = app
endif

OBJ = $(SRC:.c=.o)

all: check-gtk $(TARGET)

check-gtk:
	@echo "Vérification des dépendances..."
	@if [ -z "$(GTK_AVAILABLE)" ]; then \
		echo "⚠ GTK3 non trouvé - Mode test activé"; \
		echo "Pour installer: apt-get install libgtk-3-dev pkg-config"; \
	else \
		echo "✓ GTK3 $(GTK_AVAILABLE) trouvé"; \
	fi

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo "✓ Compilation réussie: $@"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) app app-test

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run check-gtk


