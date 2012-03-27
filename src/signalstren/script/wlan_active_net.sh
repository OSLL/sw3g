#!/bin/sh
iwconfig wlan0 | awk '/Access Point:/{ i = match($0,/Access Point: /);
  if (i != 0) {
    print substr($0, i + length("Access Point: "));
  }
  }' -
