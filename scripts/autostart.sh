#!/bin/bash

# Startup Programs
if [ "$(uname -n)" = "notebook" ]; then
    picom --config "$HOME"/.config/picom/mobile.conf &
else
    picom --config "$HOME"/.config/picom/box.conf &
fi
autorandr -c &
feh --no-fehbg --bg-scale "$XDG_CACHE_HOME"/wallpaper/current.jpg &
sxhkd &
"$HOME"/.config/polybar/launch.sh &

