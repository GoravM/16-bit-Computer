# default compile
warnings_BAD: *.c
	gcc -Werror -Wall -g -std=gnu99 -o read Assembler.c -lrt
	
# compile without caring about errors
warnings_OK: *.c
	gcc -Wall -g -std=gnu99 -o read Assembler.c -lrt

# defaults runs executable with input.txt
run:
	./read input.txt

# clean
clean:
	rm read 
	> output.txt
