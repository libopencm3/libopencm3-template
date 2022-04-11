![CI](https://github.com/chinhpc/libopencm3-template/workflows/CI/badge.svg)

Easy "clone and go" repository for a libopencm3 based project.

# Instructions
 1. git clone --recurse-submodules git clone https://github.com/chinhpc/libopencm3-template.git your-project
 2. cd your-project
 3. make

If you have an older git, or got ahead of yourself and skipped the ```--recurse-submodules```
you can fix things by running ```git submodule update --init``` (This is only needed once)

# Directories
* f030f4p6 contains your application
* FreeRTOS contains your OS
* libopencm3 contains your library

# As a template
You should replace this with your _own_ README if you are using this
as a template.
