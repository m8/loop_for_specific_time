CC_FLAGS=-Wall -Wformat=0

all: clear compile run
build: clear compile

compile:
	g++ $(CC_FLAGS) main.cc -o tester.bin -lrt
	g++ $(CC_FLAGS) clock-cycler.cc -o cycler.bin -lrt	

run:
	./tester

clear:
	$(RM) tester.bin cycler.bin