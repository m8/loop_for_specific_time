CC=g++
CC_FLAGS=-Wall -Wformat=0 -lrt

all: clean compile run
build: clean compile
tester: clean compile-tester run
cycler: clean compile-cycler run-cycler
generator: clean compile-generator run-generator

compile: compile-tester compile-cycler

compile-tester:
	$(CC) $(CC_FLAGS) main.cc -o tester.bin

compile-generator:
	$(CC) $(CC_FLAGS) loop_generator.cc -o generator.bin

compile-cycler:
	$(CC) $(CC_FLAGS) clock-cycler.cc -o cycler.bin	

run: 
	./tester.bin

run-cycler: 
	./cycler.bin

run-generator: 
	./generator.bin

clean:
	$(RM) tester.bin cycler.bin generator.bin