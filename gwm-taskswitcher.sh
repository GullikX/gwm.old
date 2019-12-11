#!/bin/sh
task="$(echo "$@" | dmenu)"
[ -n "$task" ] && xsetroot -name "$task"
