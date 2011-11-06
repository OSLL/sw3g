lang en_US.UTF-8
keyboard us
timezone Europe/Moscow --utc
auth --useshadow --enablemd5
selinux --disabled
#firewall --disabled
part / --size 1024
bootloader --append single

repo --name=e11 --baseurl=http://download.fedoraproject.org/pub/fedora/linux/releases/11/Everything/i386/os/
repo --name=u11 --baseurl=http://download.fedoraproject.org/pub/fedora/linux/updates/11/i386/

#%packages --excludedocs --nobase --nodefault
%packages
glibc
initscripts
chkconfig
system-config-firewall-tui

bash
kernel
passwd
authconfig

# cyrillic console
kbd

#debugging
rpm
yum
strace
vim-minimal

#tools
screen
nc
iperf

#networking
iptables
net-tools
bind-utils
dhcp
wireless-tools
tcpdump
wireshark
iw
ppp

vconfig
tunctl
bridge-utils
iproute
ipsec-tools
openssl

tree
openssh-clients
openssh-server

%end


%post
cat > /etc/rc.d/init.d/fedora-live << EOF
#!/bin/bash
#
# live: Init script for live image
#
# chkconfig: 345 00 99
# description: Init script for live image.

. /etc/init.d/functions

if ! strstr "\`cat /proc/cmdline\`" liveimg || [ "\$1" != "start" ] || [ -e /.liveimg-configured ] ; then
    exit 0
fi

exists() {
    which \$1 >/dev/null 2>&1 || return
    \$*
}

touch /.liveimg-configured

# mount live image
if [ -b /dev/live ]; then
   mkdir -p /mnt/live
   mount -o ro /dev/live /mnt/live
fi

# read some variables out of /proc/cmdline
for o in \`cat /proc/cmdline\` ; do
    case \$o in
    ks=*)
        ks="\${o#ks=}"
        ;;
    xdriver=*)
        xdriver="--set-driver=\${o#xdriver=}"
        ;;
    esac
done


# if liveinst or textinst is given, start anaconda
if strstr "\`cat /proc/cmdline\`" liveinst ; then
   /usr/sbin/liveinst \$ks
fi
if strstr "\`cat /proc/cmdline\`" textinst ; then
   /usr/sbin/liveinst --text \$ks
fi

# enable swaps unless requested otherwise
swaps=\`blkid -t TYPE=swap -o device\`
if ! strstr "\`cat /proc/cmdline\`" noswap -a [ -n "\$swaps" ] ; then
  for s in \$swaps ; do
    action "Enabling swap partition \$s" swapon \$s
  done
fi

# configure X, allowing user to override xdriver
exists system-config-display --noui --reconfig --set-depth=24 \$xdriver

EOF

# turn off firstboot for livecd boots
echo "RUN_FIRSTBOOT=NO" > /etc/sysconfig/firstboot

chkconfig --level 345 yum-updatesd off 2>/dev/null
chkconfig --level 345 crond off 2>/dev/null
chkconfig --level 345 atd off 2>/dev/null
chkconfig --level 345 anacron off 2>/dev/null
chkconfig --level 345 readahead_early off 2>/dev/null
chkconfig --level 345 readahead_later off 2>/dev/null
chkconfig sshd on
chkconfig network on
chkconfig dhcpd off
chkconfig exim off

# Stopgap fix for RH #217966; should be fixed in HAL instead
touch /media/.hal-mtab

# workaround clock syncing on shutdown that we don't want (#297421)
sed -i -e 's/hwclock/no-such-hwclock/g' /etc/rc.d/init.d/halt

mkdir -p /root/.ssh
cat > /root/.ssh/authorized_keys <<EOF
ssh-rsa AAAAB3NzaC1yc2EAAAABIwAAAQEA2nQFWiJHja2r/F3G3nIakItlZpCcL2ny/rgXvV1+/KNWGVVqR+e4PzUg4EI9jiH5RGGui2ZF8wsHtpgg9ajDPoa4RdcYKdPIoXeFvJBCEB05yZwvpyBooINdIpARJu87vyiasDTXsHptIPe1EJO5PcBWL9tyQ7sj78HpmRmQH/6NfaVM2EYkjMUTTYNxfWbbhBQfKrTS9ZI5KSAgQ3vMWF4WIIsurEhMz9UOioX4PnOjvwFt7TO2331v2oDYrwz/yuq6kqwmYee3XilYmtC3vzaa8NKb61+IXKlAWZkDNtxBpCFFUrFT7yWi70ANtWadPQhfQhuNR55ITpKqIA5vXw== dumb@stripy
EOF
chmod -R go= /root/.ssh

cat >> /etc/hosts << EOF
192.168.1.1 guest1
192.168.1.2 guest2
EOF

cat > /etc/resolv.conf << EOF
nameserver 192.168.248.21
EOF

cat > /etc/sysconfig/network << EOF
NETWORKING=yes
NETWORKING_IPV6=no
HOSTNAME=porky.metropolis
EOF

cat > /etc/sysconfig/network-scripts/ifcfg-eth0 << EOF
DEVICE=eth0
BOOTPROTO=none
IPADDR=192.168.0.1
IPV6INIT=no
NETMASK=255.255.255.0
ONBOOT=yes
EOF

cat > /etc/sysconfig/network-scripts/ifcfg-eth1 << EOF
DEVICE=eth1
BOOTPROTO=none
IPADDR=192.168.1.1
IPV6INIT=no
NETMASK=255.255.255.0
ONBOOT=yes
EOF

sed -i /etc/sysctl.conf -e 's/^net\.ipv4\.ip_forward.*$/net.ipv4.ip_forward = 1/'


/sbin/restorecon /etc/resolv.conf

chmod 755 /etc/rc.d/init.d/fedora-live
/sbin/restorecon /etc/rc.d/init.d/fedora-live
/sbin/chkconfig --add fedora-live

# save a little bit of space at least...
rm -f /boot/initrd*
# make sure there aren't core files lying around
rm -f /core*

cat > /etc/setkey.conf << EOF
#!/usr/sbin/setkey -f

# Configuration for 192.168.5.2

# Flush the SAD and SPD
flush;
spdflush;

# Attention: Use this keys only for testing purposes!
# Generate your own keys!

# AH SAs using 128 bit long keys
add 192.168.5.1 192.168.5.2 ah 0x200 -A hmac-md5 
0xc0291ff014dccdd03874d9e8e4cdf3e6;
add 192.168.5.2 192.168.5.1 ah 0x300 -A hmac-md5 
0x96358c90783bbfa3d7b196ceabe0536b;

# ESP SAs using 192 bit long keys (168 + 24 parity)
add 192.168.5.1 192.168.5.2 esp 0x201 -E 3des-cbc 
0x7aeaca3f87d060a12f4a4487d5a5c3355920fae69a96c831;
add 192.168.5.2 192.168.5.1 esp 0x301 -E 3des-cbc 
0xf6ddb555acfd9d77b03ea3843f2653255afe8eb5573965df;

# Security policies
spdadd 192.168.5.1 192.168.5.2 any -P in ipsec
           esp/transport//require
           ah/transport//require;

spdadd 192.168.5.2 192.168.5.1 any -P out ipsec
           esp/transport//require
           ah/transport//require;
EOF

%end
