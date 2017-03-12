#!/bin/bash

program=$1
objcopy --dump-section .text=$1.bin $1