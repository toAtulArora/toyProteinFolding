SRC =$(addsuffix .o, main)

PROGRAM = main

all: $(PROGRAM)

$(PROGRAM): $(SRC)
	g++ -std=c++14 -o $@  $^

%.o : %.C
	g++ -std=c++14 -o $@ -c $<

clean:
	rm -f *.o

