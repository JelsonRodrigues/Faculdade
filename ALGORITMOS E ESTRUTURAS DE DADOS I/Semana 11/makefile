compilador = gcc
flags = -g -Wall -Wextra -lm
arquivo = exercicio.c
binario = exercicio

make:
	$(compilador) $(arquivo) -o $(binario) $(flags)
mtest:
	valgrind ./$(binario)
test:
	./$(binario)
clean:
	rm $(binario)