a6p2: Assign06P2.o
	g++ Assign06P2.o -o a6p2
Assign06P2.o: Assign06P2.cpp 
	g++ -Wall -ansi -pedantic -c Assign06P2.cpp

t:
	./a6p2 > a6p2test.out
nt:
	@rm -rf Assign06P2.o Assign06P2.cpp a6p2 a6p2test.out

clean:
	@rm -rf Assign06P2.o
cleanall:
	@rm -rf Assign06P2.o a6p2
