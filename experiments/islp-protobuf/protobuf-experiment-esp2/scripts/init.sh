#!/usr/bin/env bash
platformio lib install "Nanopb@0.4.2"


platformio init       \
--board  esp32doit-devkit-v1        \
--ide    clion
