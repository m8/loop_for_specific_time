CC_FLAGS=-Wall -Wformat=0

all: clean compile run
build: clean compile
cycler: clean compile-cycler run-cycler

compile: compile-tester compile-cycler

compile-tester:
	g++ $(CC_FLAGS) main.cc -o tester.bin -lrt

compile-cycler:
	g++ $(CC_FLAGS) clock-cycler.cc -o cycler.bin -lrt	

run:
	./tester.bin

run-cycler:
	./cycler.bin

clean:
	$(RM) tester.bin cycler.bin