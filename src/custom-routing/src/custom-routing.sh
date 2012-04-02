#!/bin/bash

###############################################################################
### Routes all ICMP packets through wlan interface; everything else through
### 3g.
###
### WARNING! This script is a quick hack, it's nowhere near production quality!
###
### Uses:
### * iproute2
### * iptables
###
### Run as root
###############################################################################

### Network parameters
# IP address
WLAN_IP="192.168.1.80"
# Gateway IP
WLAN_GATEWAY="192.168.1.1"
# Subnet in CIDR notation
WLAN_SUBNET="192.168.1.0/24"
# Interface
WLAN_IFACE="wlan0"
# Routing table name
WLAN_TABLE="wlan"
# Firewall mark for packets sent or received thru this net (a number)
WLAN_MARK=1

UMTS_IP="10.40.115.53"
UMTS_GATEWAY="10.40.115.5"
UMTS_SUBNET="10.40.115.0/24"
UMTS_IFACE="eth0"
UMTS_TABLE="3g"
UMTS_MARK=2

### Paths to executables
IP=/sbin/ip
FW=/sbin/iptables

### Configure routing
# add tables to iproute2 configuration
if ! grep "balance-tables" /etc/iproute2/rt_tables > /dev/null; then 
  echo "Adding routing tables $WLAN_TABLE and $UMTS_TABLE"
cat >>/etc/iproute2/rt_tables <<EOF
### BEGIN balance-tables
1 $WLAN_TABLE
2 $UMTS_TABLE
### END balance-tables
EOF
fi

# enable forwarding
echo 1 > /proc/sys/net/ipv4/ip_forward

# disable reverse path filter
for i in /proc/sys/net/ipv4/conf/*/rp_filter ; do
  echo 0 > $i 
done

# remove any existing routes for our tables
$IP route delete table $WLAN_TABLE
$IP route delete table $UMTS_TABLE

# add routes for WLAN
$IP route add $WLAN_SUBNET dev wlan0 src $WLAN_IP table $WLAN_TABLE
$IP route add default via $WLAN_GATEWAY dev $WLAN_IFACE table $WLAN_TABLE

# add routes for 3G
$IP route add $UMTS_SUBNET dev $UMTS_IFACE src $UMTS_IP table $UMTS_TABLE
$IP route add default via $UMTS_GATEWAY dev $UMTS_IFACE table $UMTS_TABLE

# remove any existing rules for the tables
$IP rule delete table $WLAN_TABLE
$IP rule delete table $UMTS_TABLE

# add rules: routing table is selected according to the firewall mark
$IP rule add from all fwmark $WLAN_MARK table $WLAN_TABLE
$IP rule add from all fwmark $UMTS_MARK table $UMTS_TABLE

### Set up firewall to mark packets for routing
# clear all rules in tables we're going to change
$FW --flush -vt nat
$FW --flush -vt mangle

# add marking rules
for chain in PREROUTING OUTPUT FORWARD
do
  # if packet is targeted to 3g subnet, send it through 3g.
  $FW -I $chain -t mangle -p icmp -d $UMTS_SUBNET -j MARK --set-mark $UMTS_MARK
  # else, send it through wlan  
  $FW -I $chain -t mangle -p icmp ! -d $UMTS_SUBNET -j MARK --set-mark $WLAN_MARK
  
  # all other packets through 3g.
  $FW -I $chain -t mangle ! -p icmp -j MARK --set-mark $UMTS_MARK
done

# because 3g is default route, source address for the ICMP packets
# forced through wlan will be incorrect.
# we solve this problem by adding a rule for masquerading:
$FW -t nat -A POSTROUTING -p icmp -o $WLAN_IFACE -j MASQUERADE
