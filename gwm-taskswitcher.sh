#!/bin/sh
echo -n "$@" | dmenu | tr '\n' '\0' | xargs -0 -r xsetroot -name
