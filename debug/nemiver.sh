#!/bin/bash
xterm -e st-util &

# save the PID of the background process
XTERM_PID=$!

# start the gdb gui
nemiver --remote=localhost:4242 --gdb-binary="$(which arm-none-eabi-gdb)" build/$1

# close xterm when the user has exited nemiver
kill $XTERM_PID
