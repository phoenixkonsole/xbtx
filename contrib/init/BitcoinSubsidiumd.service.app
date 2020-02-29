[Unit]
Description=BitcoinSubsidium's distributed currency daemon
After=network.target

[Service]
User=BitcoinSubsidium
Group=BitcoinSubsidium

Type=forking
PIDFile=/var/lib/BitcoinSubsidiumd/BitcoinSubsidiumd.pid
ExecStart=/usr/bin/BitcoinSubsidiumd -daemon -pid=/var/lib/BitcoinSubsidiumd/BitcoinSubsidiumd.pid \
-conf=/etc/BitcoinSubsidium/BitcoinSubsidium.conf -datadir=/var/lib/BitcoinSubsidiumd -disablewallet

Restart=always
PrivateTmp=true
TimeoutStopSec=60s
TimeoutStartSec=2s
StartLimitInterval=120s
StartLimitBurst=5

[Install]
WantedBy=multi-user.target
