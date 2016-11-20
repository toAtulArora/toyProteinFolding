SRC =$(addsuffix .o, main)

PROGRAM = main

all: $(PROGRAM)

$(PROGRAM): $(SRC)
	g++ -std=c++11 -o $@  $^

%.o : %.C
	g++ -std=c++11 -o $@ -c $<

clean:
	rm -f *.o

