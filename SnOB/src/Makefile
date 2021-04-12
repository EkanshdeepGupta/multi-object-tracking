CFLAGS = #-I /usr/include/custom/matrix
LIBS = -lstdc++ -lm



# Sn 

Cycles.o: Cycles.hpp Cycles.cpp  
	gcc -c Cycles.cpp $(CFLAGS)

Partition.o: Partition.hpp Partition.cpp StandardTableau.hpp  Matrix.hpp 
	gcc -c Partition.cpp $(CFLAGS)

StandardTableau.o: StandardTableau.hpp StandardTableau.cpp Partition.o 
	gcc -c StandardTableau.cpp $(CFLAGS)

SnElement.o: SnElement.cpp SnElement.hpp Cycles.o Sn.hpp  
	gcc -c SnElement.cpp $(CFLAGS)

SnIrreducible.o: SnIrreducible.hpp SnIrreducible.cpp Partition.o StandardTableau.o SnElement.o  Sn.o
	gcc -c SnIrreducible.cpp $(CFLAGS)
 
SnFunction.o: SnFunction.cpp SnFunction.hpp SnElement.o FiniteGroup.hpp Sn.hpp 
	gcc -c SnFunction.cpp $(CFLAGS)

SnFourierTransform.o: SnFourierTransform.cpp SnFourierTransform.hpp Sn.hpp SnFunction.hpp SnElement.o  Matrix.hpp
	gcc -c SnFourierTransform.cpp $(CFLAGS)

SnFtree.o: SnFtree.cpp SnFtree.hpp Sn.hpp SnFunction.hpp SnElement.o  Matrix.hpp SnIrreducible.o
	gcc -c SnFtree.cpp $(CFLAGS)


Sn.o: Sn.cpp Sn.hpp SnElement.hpp StandardTableau.hpp Partition.hpp FiniteGroup.hpp Group.hpp 
	gcc -c Sn.cpp $(CFLAGS)

testSnFtree: testSnFtree.cpp Sn.o SnFtree.o SnFourierTransform.o SnFunction.o SnIrreducible.o StandardTableau.o Partition.o SnElement.o 
	gcc -o testSnFtree testSnFtree.cpp *.o $(CFLAGS) $(LIBS) 



SnFiles: Partition.o StandardTableau.o Cycles.o SnElement.o SnIrreducible.o \
	SnFunction.o SnFourierTransform.o SnFtree.o Sn.o 


all: SnFiles 

clean: 
	rm *.o
