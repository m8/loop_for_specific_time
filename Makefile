CC_FLAGS=-Wall -Wformat=0

all: clear compile run
build: clear compile
cycler: clear compile-cycler run-cycler

compile: compile-tester compile-cycler

compile-tester:
	g++ $(CC_FLAGS) main.cc -o tester.bin -lrt

compile-cycler:
	g++ $(CC_FLAGS) clock-cycler.cc -o cycler.bin -lrt	

run:
	./tester.bin

run-cycler:
	./cycler.bin

clear:
	$(RM) tester.bin cycler.bin