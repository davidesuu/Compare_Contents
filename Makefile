ifdef OS
	EXT = .exe
	else
		ifeq ($(shell uname), Linux)
			EXT =
		endif
		ifeq ($(shell uname), Darwin)		#Para os usuarios de mac :)
			EXT =
		endif
	endif

CC = gcc

CFLAGS = -I./headers -Wall         # Flag pra procurar dentro da pasta headers sem precisar especificar o caminho completo

TARGET = compare$(EXT)

SRC = main.c src/content.c src/csv.c src/name.c src/directory.c
OUTPUT = output/result_cont.csv output/result_name.csv
all: $(TARGET) 

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)


clean:
	rm -f $(TARGET)
	rm -f $(OUTPUT)