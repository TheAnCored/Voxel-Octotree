GPP = g++
CFLAG = -c
OFLAG = -o
WALLFLAG = -Wall

alt: my_prog

main.o: main.cpp
	$(GPP) $(CFLAG) $(WALLFLAG) main.cpp

octotree.o: octotree.cpp
	$(GPP) $(CFLAG) $(WALLFLAG) octotree.cpp

my_prog: main.o octotree.o
	$(GPP) main.o octotree.o $(OFLAG) my_prog

clear:
	rm -rf *.o my_prog