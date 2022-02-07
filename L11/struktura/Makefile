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

all: MPI_simple run 

run: MPI_simple
	$(MPI_run) -np 3 MPI_simple

# zaleznosci i komendy
MPI_simple : MPI_simple.o
	$(LOADER) $(OPT) MPI_simple.o -o MPI_simple 

# jak uzyskac plik moj_program.o ?
MPI_simple.o: MPI_simple.c 
	$(CCOMP) -c $(OPT) MPI_simple.c 

clean:
	rm -f *.o MPI_simple
