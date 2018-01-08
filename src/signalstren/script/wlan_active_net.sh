#!/bin/sh
iwconfig wlan0 | awk '/Access Point:/{ i = match($0,/Access Point: ([[:xdigit:]][[:xdigit:]]:)+[[:xdigit:]][[:xdigit:]]/);
  if (i != 0) {
    printf "%s", substr($0, i + length("Access Point: "), RLENGTH - length("Access Point: "));
  }
  }' -
