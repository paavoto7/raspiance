#!/bin/bash

# Simply converts the images in captures directory to png's
# If you use this, pay attention to the size given to ffmpeg

shopt -s nullglob
for f in ./captures/*.yuv; do
    ffmpeg -y -loglevel error -f rawvideo -pix_fmt yuyv422 -s 640x360 -i "$f" ${f%.*}.png
done