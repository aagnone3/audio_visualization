#!/bin/bash
set -eou pipefail

bash clean.sh

aclocal
autoconf
automake --add-missing
./configure
make
