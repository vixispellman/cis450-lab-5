all: multiply shade detect

multiply: multiply.c multiplyOPT.c
	gcc -g -c -fopenmp -o multiplyOPT.o -O1 multiplyOPT.c
	gcc -g -o multiply -fopenmp multiply.c multiplyOPT.o

shade: bmp.c shade.c shadeOPT.c
	gcc -g -c -o shadeOPT.o -O1 shadeOPT.c
	gcc -g -o shade -fopenmp shade.c shadeOPT.o bmp.c

detect: bmp.c detect.c detectOPT.c
	gcc -g -c -o detectOPT.o -O1 detectOPT.c
	gcc -g -o detect -fopenmp detect.c detectOPT.o bmp.c

tests: multiplyTest shadeTest detectTest

multiplyTest: multiply
	#==================== Multiply Test ====================#
	./multiply 800 40

shadeTest: sunflower.bmp shade
	#====================  sharpen Test   ====================#
	./shade shade.txt sunflower.bmp sunflowerBASE.bmp sunflowerOPT.bmp
	diff sunflowerBASE.bmp sunflowerOPT.bmp

detectTest: board.bmp bunny.bmp
	#====================  Detect Test  ====================#
	./detect 40 board.bmp boardBASE.bmp boardOPT.bmp
	diff boardBASE.bmp boardOPT.bmp

clean:
	rm -f multiply shade detect multiplyOPT.o shadeOPT.o detectOPT.o
	rm -f sunflowerBASE.bmp sunflowerOPT.bmp
	rm -f boardBASE.bmp boardOPT.bmp
	rm -f bunnyBASE.bmp bunnyOPT.bmp
