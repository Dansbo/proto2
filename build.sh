#!/bin/bash
rm -rf *.PRG
rm -rf *.prg
acme -f cbm -o DRAWING.PRG drawing.asm
acme -f cbm -o LEVELS.BIN levels.asm
acme -f cbm -o PROTO2.PRG proto2.asm
acme -f cbm -o LVLEDIT.PRG lvledit.asm
