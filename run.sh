#!/bin/bash

scons
qemu-system-i386 -cdrom circuit.iso
