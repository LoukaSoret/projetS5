#!/bin/bash

./configure CFLAGS='-Wall -Werror -g'
make distclean
autoreconf -vfi
make
