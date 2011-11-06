#! /bin/bash

mkdir -p "$HOME/tmp/livecd-cache/yum-cache"
sudo livecd-creator --config=livecd-fedora-minimal-cui.ks --cache="$HOME/tmp/livecd-cache"
# --skip-compression
