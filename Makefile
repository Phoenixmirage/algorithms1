OBJS	= main.o functions.o structs.o structs_cos.o structs_ma.o structs_ham.o 
SOURCE	= main.c functions.c structs.c structs_cos.c structs_ma.c structs_ham.c 
HEADER	= struct.h functions.h
OUT	= LSH
CC	= gcc
FLAGS	= -g -c
all:$(OBJS)
	$(CC) $(OBJS) -o $(OUT) -lm
main.o: main.c
	$(CC) $(FLAGS) main.c

functions.o: functions.c
	$(CC) $(FLAGS) functions.c
	
structs.o: structs.c
	$(CC) $(FLAGS) structs.c
	
structs_ham.o: structs_ham.c
	$(CC) $(FLAGS) structs_ham.c

structs_cos.o: structs_cos.c
	$(CC) $(FLAGS) structs_cos.c

structs_ma.o: structs_ma.c
	$(CC) $(FLAGS) structs_ma.c


clean:
	rm $(OUT)
	rm $(OBJS)


count:
	wc $(SOURCE) $(HEADER)
