#!/bin/bash

ME=`whoami`

ipcs -m | grep $ME | perl -e 'while (<STDIN>) { @a=split(/\s+/); print `ipcrm -m $a[1]`}'
ipcs -q | grep $ME | perl -e 'while (<STDIN>) { @a=split(/\s+/); print `ipcrm -q $a[1]`}'
rm -f /tmp/server