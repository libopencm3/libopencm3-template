PROJECT = STM32F030F4P6
BUILD_DIR = build

SHARED_DIR = rtc include
CFILES = my-project.c

# TODO - you will need to edit these two lines!
DEVICE=stm32f030f4p6
OOCD_FILE = board/stm32f0discovery.cfg

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
