# kompilator c
CCOMP = mpicc

# konsolidator
LOADER = mpicc

# program uruchomieniowy MPI
MPI_run = mpirun -oversubscribe

# opcje optymalizacji:
# wersja do debugowania
OPT = -g -DDEBUG
# wersja zoptymalizowana do mierzenia czasu
# OPT = -O3

# pliki naglowkowe
# INC = -I/usr/lib/x86_64-linux-gnu/openmpi/include/openmpi -I/usr/lib/x86_64-linux-gnu/openmpi/include -pthread

# biblioteki
# LIB = -L/usr/lib/x86_64-linux-gnu/openmpi/lib -lmpi

all: oblicz_PI run 

run: oblicz_PI
	$(MPI_run) -np 4 oblicz_PI

# zaleznosci i komendy
oblicz_PI : oblicz_PI.o
	$(LOADER) $(OPT) oblicz_PI.o -o oblicz_PI -lm

# jak uzyskac plik moj_program.o ?
oblicz_PI.o: oblicz_PI.c 
	$(CCOMP) -c $(OPT) oblicz_PI.c 

clean:
	rm -f *.o oblicz_PI
