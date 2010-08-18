SOURCES = $(wildcard *.c)
BINS = $(patsubst %.c, bin/%, $(SOURCES))

all: bin $(BINS) test
bin:
	mkdir -p bin

$(BINS): bin/% : %.c
	$(CC) $^ -o $@

test:
	@for b in bin/*; do echo $$b && ./$$b; done

clean:
	rm -rf bin

.PHONY: test
