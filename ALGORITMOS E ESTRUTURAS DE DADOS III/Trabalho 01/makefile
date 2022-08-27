compilador = gcc
flags = -Wall -Wextra -lm
arquivo = christofides.c
bibliotecas = grafo.c vetorInt.c
objeto_bibliotecas = grafo.o vetorInt.o
binario = christofides.o

make: $(objeto_bibliotecas)
	make bibliotecas
	$(compilador) $(flags) -o $(binario) $(arquivo) $(objeto_bibliotecas)
debug:
	$(compilador) $(flags) -g -o $(binario) $(arquivo) $(objeto_bibliotecas)
bibliotecas:
	$(compilador) $(flags) -c $(bibliotecas) 
mtest:
	valgrind --leak-check=full ./$(binario)
otimizado:
	$(compilador) -c $(bibliotecas) 
	$(compilador) -o $(binario) $(arquivo) $(objeto_bibliotecas)
run:
	./$(binario)
test:
	./$(binario) <teste.txt
time:
	time ./$(binario) <teste.txt
clean:
	rm *.o