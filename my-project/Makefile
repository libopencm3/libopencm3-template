PROJECT = STM32F030F4P6
BUILD_DIR = build

SHARED_DIR = include #rtc
CFILES = my-project.c

# TODO - you will need to edit these two lines!
DEVICE=stm32f030f4p6
OOCD_INTERFACE=stlink-v2
OOCD_TARGET=stm32f0x

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk

############# For testing coding syntax #############
TESTING_CFILES = rtc/rtc_lib.c
TESTING_OBJS = $(TESTING_CFILES:%.c=$(BUILD_DIR)/%.o)
test: $(TESTING_OBJS)
#	@echo $(TESTING_OBJS)
############# End For testing coding syntax #########