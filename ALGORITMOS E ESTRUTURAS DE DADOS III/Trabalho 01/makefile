compilador = gcc
flags = -Wall -Wextra -lm
arquivo = tsp.c
bibliotecas = grafo.c vetorInt.c
objeto_bibliotecas = grafo.o vetorInt.o
binario = tsp.o

make: $(objeto_bibliotecas)
	make bibliotecas
	$(compilador) $(flags) -g -o $(binario) $(arquivo) $(objeto_bibliotecas)
bibliotecas:
	$(compilador) $(flags) -g -c $(bibliotecas) 
mtest:
	valgrind --leak-check=full ./$(binario)
mtest_auto:
	valgrind --leak-check=full ./$(binario) <teste.txt
otimizado:
	$(compilador) -c $(bibliotecas) 
	$(compilador) -o $(binario) $(arquivo) $(objeto_bibliotecas)
run:
	./$(binario)
test:
	./$(binario) <teste.txt
time:
	time ./$(binario) <teste.txt
time2:
	time ./$(binario) --path=tsp5_27603.csv --delim=';' --alg=a
clean:
	rm *.o