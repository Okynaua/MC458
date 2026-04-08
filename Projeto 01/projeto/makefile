CC = gcc
CFLAGS = -Wall -Wextra

TARGETS = test teste teste_ilustrativo

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGETS)