#!/bin/bash
shopt -s nullglob
for f in ./captures/*.yuv; do
    ffmpeg -y -loglevel error -f rawvideo -pix_fmt yuyv422 -s 640x360 -i "$f" ${f%.*}.png
done