# Copyright © 2017 Adam Pyka <cavetronic@cavetronic.eu>
# This program is free software. It comes without any warranty, to the extent
# permitted by applicable law. You can redistribute it and/or modify it under
# the terms of the GNU GENERAL PUBLIC LICENSE, Version 3, as
# published by Free Software Foundation. See the LICENSE file for more details.

CFLAGS = -g -Wall -Wextra -Werror -std=c99
CFLAGS += -D_POSIX_C_SOURCE=199309L -D_BSD_SOURCE -D_DEFAULT_SOURCE -D_DARWIN_C_SOURCE

all: clean convert
	@echo "+++ All good."""

clean:
	$(RM) convert *.o

convert: convert.c convert.h

.PHONY: all clean
