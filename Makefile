SOURCES = $(wildcard *.c)
BINS = $(patsubst %.c, bin/%, $(SOURCES))

all: bin $(BINS) test
bin:
	mkdir -p bin

bin/avl-tree: bst.c
$(BINS): perf.h
$(BINS): bin/% : %.c
	$(CC) $< -o $@ -g -O0

test:
	@for b in bin/*; do \
		printf "%20s" "$$b "; \
		for i in `seq 1 20`; do \
			echo -n .; \
			./$$b > /dev/null || break; \
		done; \
		echo; \
	done

clean:
	rm -rf bin *.data

.PHONY: test
