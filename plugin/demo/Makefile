#   Copyright (C) 2025 John Törnblom
#
# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.

ifdef PS5_PAYLOAD_SDK
    include $(PS5_PAYLOAD_SDK)/toolchain/prospero.mk
    MHD_CFLAGS := `$(PS5_PAYLOAD_SDK)/bin/prospero-pkg-config libmicrohttpd --cflags`
    MHD_LIBS := `$(PS5_PAYLOAD_SDK)/bin/prospero-pkg-config libmicrohttpd --libs`
else
    CC ?= gcc
    MHD_CFLAGS := `pkg-config --cflags libmicrohttpd`
    MHD_LIBS := `pkg-config --libs libmicrohttpd`
endif

WEBSRV_SRC := ../../src
CFLAGS += -I$(WEBSRV_SRC) $(MHD_CFLAGS) -fPIC -shared -Wall -Werror
PLUGIN := demo.so

all: $(PLUGIN)

$(PLUGIN): demo.c
	$(CC) $(CFLAGS) -o $@ $^ $(MHD_LIBS)

clean:
	rm -f $(PLUGIN)
