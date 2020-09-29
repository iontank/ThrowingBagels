SUBDIR-y += src/ledscape/firmware
SUBDIR-y += src/noise
SUBDIR-y += src/ledscape
SUBDIR-y += src/demos
all: 
	for dir in $(SUBDIR-y); do \
		$(MAKE) -C $$dir || exit 1; \
	done

clean:
	for dir in $(SUBDIR-y); do \
		$(MAKE) -C $$dir clean; \
	done

