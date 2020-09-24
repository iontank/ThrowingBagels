SUBDIR-y += src/ledscape/firmware
SUBDIR-y += src/ledscape
all: 
	for dir in $(SUBDIR-y); do \
		$(MAKE) -C $$dir || exit 1; \
	done

clean:
	for dir in $(SUBDIR-y); do \
		$(MAKE) -C $$dir clean; \
	done

