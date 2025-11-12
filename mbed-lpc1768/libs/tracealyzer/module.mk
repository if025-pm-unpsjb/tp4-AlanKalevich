ifeq ($(USE_TRACEALYZER),y)

COMMON_FLAGS += -DUSE_TRACEALIZER

TRACE_BASE=./libs/tracealyzer

SRC+=$(wildcard $(TRACE_BASE)/*.c)
INCLUDE_PATHS += -I$(TRACE_BASE)/include
INCLUDE_PATHS += -I$(TRACE_BASE)/config

endif
