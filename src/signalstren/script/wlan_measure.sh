#!/bin/sh
if ( grep "No scan results" /tmp/.wlanmeasure >/dev/null )
then
  exit 1;
fi

cat /tmp/.wlanmeasure  | awk 'BEGIN{FS="- Address: "}
/Cell [[:digit:]]+ - Address:/ {
  print $2
}
/Quality=.*/ {
  print $0
}
' | awk 'BEGIN{FS=" ";RS="\n"} /Quality=.*/{ 
  i = match($1,/Quality=/);
  if (i != 0) {
    print substr($1, i + length("Quality="));
  }
} !/Quality=.*/{print}' - | awk 'BEGIN{FS="/"} /.*\/.*/{print $1 * 100/$2} !/.*\/.*/{print}'

rm -f /tmp/.wlanmeasure
