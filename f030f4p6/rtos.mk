# /*
# * Copyright (C) 2019 ChinhPC <chinhphancong@outlook.com>
# * 
# * Author: ChinhPC
# *
# * This file is free software: you can redistribute it and/or modify it
# * under the terms of the GNU General Public License as published by the
# * Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# * 
# * This file is distributed in the hope that it will be useful, but
# * WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# * See the GNU General Public License for more details.
# * 
# * You should have received a copy of the GNU General Public License along
# * with this program.  If not, see <http://www.gnu.org/licenses/>.
# */

OS_SHARED_DIR := \
$(FreeRTOS_DIR)/FreeRTOS/Source/include \
$(FreeRTOS_DIR)/FreeRTOS/Source/portable/GCC/ARM_CM0

CFILES += \
$(FreeRTOS_DIR)/FreeRTOS/Source/tasks.c \
$(FreeRTOS_DIR)/FreeRTOS/Source/queue.c \
$(FreeRTOS_DIR)/FreeRTOS/Source/list.c \
$(FreeRTOS_DIR)/FreeRTOS/Source/portable/GCC/ARM_CM0/port.c \
$(FreeRTOS_DIR)/FreeRTOS/Source/portable/MemMang/heap_3.c \
$(FreeRTOS_DIR)/FreeRTOS/Source/timers.c

VPATH += $(OS_SHARED_DIR)
INCLUDES += $(patsubst %,-I%,$(OS_SHARED_DIR))
