# default compile
warnings_BAD: Translator.c
	gcc -Werror -Wall -g -std=gnu99 -o read Translator.c -lrt
	
# compile without caring about errors
warnings_OK: Translator.c
	gcc -Wall -g -std=gnu99 -o read Translator.c -lrt

# defaults runs executable with input.txt
run:
	gcc -Werror -Wall -g -std=gnu99 -o read Translator.c -lrt
	./read input.txt 
	rm read

clean:
	rm read
	> output.txt

full:
	gcc -Werror -Wall -g -std=gnu99 -o read Translator.c -lrt
	./read VMinput.txt
	rm read

	mv output.txt ASinput.txt

	gcc -Werror -Wall -g -std=gnu99 -o read ~/Workspace/16-bit-Computer/Assembler/Assembler.c -lrt
	./read ASinput.txt
	rm read
