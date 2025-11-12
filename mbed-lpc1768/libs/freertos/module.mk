ifeq ($(USE_FREERTOS),y)

FREERTOS_BASE=./libs/freertos

SRC+=$(wildcard $(FREERTOS_BASE)/*.c)
SRC+=$(wildcard $(FREERTOS_BASE)/portable/*.c)
SRC+=$(FREERTOS_BASE)/MemMang/heap_$(FREERTOS_HEAP_TYPE).c

INCLUDE_PATHS += -I$(FREERTOS_BASE)/include
INCLUDE_PATHS += -I$(FREERTOS_BASE)/portable

endif
