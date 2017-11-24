CFLAGS += -g -Wno-deprecated-declarations -D__PROG_TYPES_COMPAT__
all: udp-server udp-client
TARGET = sparrow
UIP_CONF_IPV6 = 1
CFLAGS += -DUDP_CONNECTION_ADDR=fdfd::$(NODE)
CONTIKI = ../../../..
include $(CONTIKI)/Makefile.include
