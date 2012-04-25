#!/bin/sh

# trick to get current script directory
# from: http://ubuntuforums.org/showpost.php?p=7876770&postcount=7
currentscriptpath()
{
  fullpath=`echo "$(readlink -f $0)"`
  fullpath_length=`echo ${#fullpath}`
  scriptname="$(basename $0)"
  scriptname_length=`echo ${#scriptname}`
  result_length=`echo $fullpath_length - $scriptname_length - 1 | bc`
  scriptdir=`echo $fullpath | head -c $result_length`
}

if ( grep "No scan results" /tmp/.wlanmeasure >/dev/null )
then
  exit 1;
fi

currentscriptpath

active_net=`$scriptdir/wlan_active_net.sh`
snr=`awk '/wlan0/{print $4-$5}' /proc/net/wireless`

cat /tmp/.wlanmeasure  | awk -v active_net="$active_net" -v snr="$snr" 'BEGIN{FS="- Address: "}
/Cell [[:digit:]]+ - Address:/ {
  print $2
  if ($2 == active_net) {
    print snr;
  } else {
    print "---";
  }
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
