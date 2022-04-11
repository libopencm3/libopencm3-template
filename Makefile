# Be silent per default, but 'make V=1' will show all compiler calls.
# If you're insane, V=99 will print out all sorts of things.
V?=0
ifeq ($(V),0)
Q       := @
NULL    := 2>/dev/null

# Do not print "Entering directory ...".
MAKEFLAGS += --no-print-directory
endif

PARALLEL_MAKE := $(shell nproc)
MAKEFLAGS += -j$(PARALLEL_MAKE)
$(info 	)
$(info [*****	MAKEFLAGS = ${MAKEFLAGS}	*****])
$(info 	)

TARGETS ?= f030f4p6

OPENCM3_DIR := $(realpath libopencm3)
FreeRTOS := $(realpath FreeRTOS)

PREFIX := $(dir $(wildcard /opt/gcc-arm-none-eabi*/bin/arm-none-eabi-gcc))arm-none-eabi-

TARGETS_DIRS:=$(sort $(dir $(addsuffix /Makefile,$(TARGETS))))

all: demos

demos: $(TARGETS_DIRS)
	$(Q)true

$(TARGETS_DIRS): lib
	@printf "  BUILD   $@\n";
	$(Q)$(MAKE) --directory=$@ PREFIX=$(PREFIX)

submodule:
	$(Q)if [ ! "`ls -A $(FreeRTOS)`" ] || [ ! "`ls -A $(OPENCM3_DIR)`" ] ; then \
		git submodule update --init --recursive ; \
		fi

lib: submodule
	$(Q)$(MAKE) -C $(OPENCM3_DIR) PREFIX=$(PREFIX)

clean: $(TARGETS_DIRS:=.clean)
	$(Q)true

%.clean:
	$(Q)if [ -d $* ]; then \
		printf "  CLEAN   $*\n"; \
		$(MAKE) -C $* clean PREFIX=$(PREFIX) || exit $?; \
		fi;

%.flash: %/
	$(Q)$(MAKE) -C $* PREFIX=$(PREFIX) flash

%.debug: %/
	$(Q)$(MAKE) -C $* PREFIX=$(PREFIX) debug

.PHONY: all clean flash debug
