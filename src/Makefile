PROJECT = awesomesauce
BUILD_DIR = bin

SHARED_DIR = ./include
CPPFILES += main.cpp
CFILES += api.c
# AFILES += api-asm.S

# TODO - you will need to edit these lines!
DEVICE = stm32f103c8
OOCD_INTERFACE ?= stlink-v2
OOCD_TARGET ?= stm32f1x

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR = ../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
