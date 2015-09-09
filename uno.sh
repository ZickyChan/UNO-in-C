#!/bin/bash
export XDG_CONFIG_HOME=$PWD/conf/
xfce4-terminal --fullscreen --disable-server --command="./bin/uno $*"