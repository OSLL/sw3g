#!/bin/sh
# sw3g project, OSLL
# adding ipsec route on the application server

# !!! NOT TESTED !!!

ppp_ip   = $1 # PPP IP address
ipsec_ip = $2 # IPsec IP address

# if run script without any parameters
if [ $# -eq 0 ]; then
		echo "usage : $0 [PPP IP] [IPsec IP]";	# $0 - name of script
	else
		route add "$ppp_ip" gw "$ipsec_ip"
		echo "ipsec route successfuly add!"
fi
