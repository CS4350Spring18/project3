project=mysh
objects=mysh.o mycd.o
cc=gcc
cflags=-Wall -std=c99 -g -pedantic

exec: $(project)
	./$< $(file)

all: $(objects)

$(project): $(objects)
	$(cc) -o $(project) $(objects) $(links)

$(objects): %.o: %.c
	$(cc) -c $(cflags) $< -o $@

.PHONY: clean
clean:
	rm -f $(project) $(objects) *.out *.log *.txt
