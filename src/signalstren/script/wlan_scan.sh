#!/bin/sh
iwlist wlan0 scanning last 2>/dev/null >.wlanmeasure
cat .wlanmeasure | awk 'BEGIN{FS="- Address: "}
/Cell [[:digit:]]+ - Address:/ {
  print $2
}
/ESSID:/ {
  split($0, a, /\"/);
  print a[2]
}
' 
