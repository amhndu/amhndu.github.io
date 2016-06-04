#!/bin/sh
# Disclaimer: This modified script comes with no warranty, run at your own risk.

HAMACHI_DST=/opt/logmein-hamachi

#if [ ! -x /usr/lib/lsb/install_initd ] ; then
#	echo LogMeIn Hamachi needs lsb package;
#	exit 1;
#fi

if [ -x /opt/logmein-hamachi/uninstall.sh ] ; then
	echo Removing previous version ..
	/opt/logmein-hamachi/uninstall.sh
fi

echo Copying files into $HAMACHI_DST ..
mkdir -p "$HAMACHI_DST/bin"
install -m 755 hamachid "$HAMACHI_DST/bin"
install -m 755 dnsup "$HAMACHI_DST/bin"
install -m 755 dnsdown "$HAMACHI_DST/bin"
install -m 755 uninstall.sh "$HAMACHI_DST"
install -m 444 README "$HAMACHI_DST"
install -m 444 LICENSE "$HAMACHI_DST"
install -m 444 CHANGES "$HAMACHI_DST"

echo Creating LogMeIn Hamachi symlink ..
ln -sf "$HAMACHI_DST/bin/hamachid" /usr/bin/hamachi

if [ ! -x /dev/net/tun ] ; then
	echo Creating TunTap ..
	mkdir -p /dev/net
	mknod /dev/net/tun c 10 200
	chmod 0666 /dev/net/tun
fi

echo Installing LogMeIn Hamachi service ..
mkdir -p /usr/lib/systemd/system
install -m 644 systemd /usr/lib/systemd/system/hamachi.service
#install -m 755 hamachi-init /etc/init.d/logmein-hamachi
#/usr/lib/lsb/install_initd /etc/init.d/logmein-hamachi

echo Starting LogMeIn Hamachi service ..
#/etc/init.d/logmein-hamachi start
systemctl start hamachi

echo LogMeIn Hamachi is installed. See README for what to do next.
