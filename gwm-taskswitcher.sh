#!/bin/sh
echo -n "$@" | dmenu | xargs -r xsetroot -name
