SOURCES = $(wildcard *.c)
BINS = $(patsubst %.c, bin/%, $(SOURCES))

all: bin $(BINS) test
bin:
	mkdir -p bin

$(BINS): bin/% : %.c
	$(CC) $^ -o $@ -g -O0

test:
	@for b in bin/*; do echo $$b && for i in `seq 1 100`; do ./$$b || break; done ; done

clean:
	rm -rf bin

.PHONY: test
