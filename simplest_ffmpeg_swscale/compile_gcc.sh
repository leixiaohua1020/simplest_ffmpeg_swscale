#! /bin/sh
gcc simplest_ffmpeg_swscale.cpp -g -o simplest_ffmpeg_swscale.out  -I /usr/local/include -L /usr/local/lib \
-lswscale -lavutil
