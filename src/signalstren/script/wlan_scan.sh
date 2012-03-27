#!/bin/sh
iwlist wlan0 scanning 2>/dev/null >/tmp/.wlanmeasure
cat /tmp/.wlanmeasure | awk 'BEGIN{FS="- Address: "}
/Cell [[:digit:]]+ - Address:/ {
  print $2
}
/ESSID:/ {
  split($0, a, /\"/);
  print a[2]
}
' 
