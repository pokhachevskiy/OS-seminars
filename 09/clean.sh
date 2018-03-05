#!/bin/bash

ME=`whoami`

ipcs -s | grep daniil | perl -e 'while (<STDIN>) { @a=split(/\s+/); print `ipcrm -s $a[1]`}'
