CC    = gcc
CPP   = g++
FLAGS = -Wall
LIBS  = -lpthread -lm
TARGETS = par_nepar par_nepar_1 par_nepar_2 par_nepar_3 par_nepar_4 par_nepar_5

default	: par_nepar
all     : $(TARGETS)

par_nepar: par_nepar.o
	$(CC) $(CFLAGS) par_nepar.o $(LIBS) -o par_nepar

par_nepar_1: par_nepar_1.o
	$(CC) $(CFLAGS) par_nepar_1.o $(LIBS) -o par_nepar_1

par_nepar_2: par_nepar_2.o
	$(CC) $(CFLAGS) par_nepar_2.o $(LIBS) -o par_nepar_2

par_nepar_3: par_nepar_3.o
	$(CC) $(CFLAGS) par_nepar_3.o $(LIBS) -o par_nepar_3

par_nepar_4: par_nepar_4.o
	$(CC) $(CFLAGS) par_nepar_4.o $(LIBS) -o par_nepar_4

par_nepar_5: par_nepar_5.o
	$(CC) $(CFLAGS) par_nepar_5.o $(LIBS) -o par_nepar_5


clean	:
	rm -f *.o *~ $(TARGETS)

.c.o:
	$(CC) $(FLAGS) -c $<








