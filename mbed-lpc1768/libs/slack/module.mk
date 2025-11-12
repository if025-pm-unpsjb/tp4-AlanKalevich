ifeq ($(USE_SLACK),y)

SLACK_BASE=./libs/slack

INCLUDE_PATHS += -I$(SLACK_BASE)/

WRAP = -Wl,--wrap=xTaskDelayUntil -Wl,--wrap=xTaskIncrementTick

endif
