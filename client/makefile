CC = gcc

CFLAGS = -g -Iinclude -I../shared/include
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net

SRC = src
EXE = exe
APP = client

C_FILES = $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*/*.c)
O_FILES = $(C_FILES:$(SRC)/%.c=$(EXE)/%.o)

main: $(O_FILES)
	$(CC) $(O_FILES) -o $(APP) $(LDFLAGS)

$(EXE)/%.o: $(SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(EXE) $(APP)






