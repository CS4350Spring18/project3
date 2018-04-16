project=project3
objects=mysh.o
cc=gcc
cflags=-Wall -std=c99 -g -pedantic

exec: $(project)
	./$<

all: $(objects)

$(project): $(objects)
	$(cc) -o $(project) $(objects) $(links)

$(objects): %.o: %.c
	$(cc) -c $(cflags) $< -o $@

.PHONY: clean
clean:
	rm -f $(project) $(objects) *.out *.log *.txt
