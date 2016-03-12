#! /bin/sh
g++ simplest_ffmpeg_swscale.cpp -g -o simplest_ffmpeg_swscale.exe \
-I /usr/local/include -L /usr/local/lib -lswscale -lavutil
