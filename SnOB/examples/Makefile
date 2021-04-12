CFLAGS = -I ../src 
LIBS = -lstdc++ -lm

example1: example1.cpp ../src/Sn.o 
	gcc -o example1.o example1.cpp ../src/*.o $(CFLAGS) $(LIBS)

example2: example2.cpp ../src/SnElement.o 
	gcc -o example2.o example2.cpp ../src/*.o $(CFLAGS) $(LIBS)

example3: example3.cpp ../src/SnElement.o ../src/Cycles.o 
	gcc -o example3.o example3.cpp ../src/*.o $(CFLAGS) $(LIBS)

example4: example4.cpp ../src/Sn.o ../src/SnIrreducible.o
	gcc -o example4.o example4.cpp ../src/*.o $(CFLAGS) $(LIBS)

example5: example5.cpp ../src/Sn.o ../src/SnIrreducible.o
	gcc -o example5.o example5.cpp ../src/*.o $(CFLAGS) $(LIBS)

example6: example6.cpp ../src/Sn.o ../src/SnIrreducible.o
	gcc -o example6.o example6.cpp ../src/*.o $(CFLAGS) $(LIBS)

example7: example7.cpp ../src/Sn.o ../src/SnIrreducible.o
	gcc -o example7.o example7.cpp ../src/*.o $(CFLAGS) $(LIBS)

example7b: example7b.cpp ../src/Sn.o ../src/SnIrreducible.o
	gcc -o example7b.o example7b.cpp ../src/*.o $(CFLAGS) $(LIBS)

example8: example8.cpp ../src/SnFunction.o ../src/SnFourierTransform.o ../src/Sn.o
	gcc -o example8.o example8.cpp ../src/*.o $(CFLAGS) $(LIBS)

example8b: example8b.cpp ../src/SnFunction.o ../src/SnFourierTransform.o ../src/Sn.o
	gcc -o example8b.o example8b.cpp ../src/*.o $(CFLAGS) $(LIBS)

example9: example9.cpp ../src/SnFunction.o ../src/SnFourierTransform.o
	gcc -o example9.o example9.cpp ../src/*.o $(CFLAGS) $(LIBS)

example10: example10.cpp ../src/SnFunction.o ../src/SnFourierTransform.o
	gcc -o example10.o example10.cpp ../src/*.o $(CFLAGS) $(LIBS)

example11: example11.cpp ../src/SnFunction.o ../src/SnFourierTransform.o ../src/SnFtree.o 
	gcc -o example11.o example11.cpp ../src/*.o $(CFLAGS) $(LIBS)

example12: example12.cpp ../src/SnFunction.o ../src/SnFourierTransform.o ../src/SnFtree.o 
	gcc -o example12.o example12.cpp ../src/*.o $(CFLAGS) $(LIBS)

chartable: chartable.cpp ../src/SnFunction.o ../src/SnFourierTransform.o ../src/SnFtree.o 
	gcc -o chartable.o chartable.cpp ../src/*.o $(CFLAGS) $(LIBS)



all: example1 example2 example3 example4 example5 example6 example7 example7b example8 example8b example9 example10 \
	example11 example12 chartable  

