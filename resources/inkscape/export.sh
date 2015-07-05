#!/bin/bash

if [ $# -lt 1 ]
then
  echo "Usage: $0 <svg>"
  echo "or"
  echo "for i in *.svg; do ./export.sh \"\$i\"; done"
  exit 1
fi

SVG="$1"
PNG="${SVG%svg}png"

inkscape --export-png="$PNG" \
         --export-area-page \
         --export-width=128 \
         --export-height=128 \
         --without-gui \
         "$SVG"
