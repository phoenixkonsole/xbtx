 #!/usr/bin/env bash

 # Execute this file to install the BitcoinSubsidium cli tools into your path on OS X

 CURRENT_LOC="$( cd "$(dirname "$0")" ; pwd -P )"
 LOCATION=${CURRENT_LOC%BitcoinSubsidium-Qt.app*}

 # Ensure that the directory to symlink to exists
 sudo mkdir -p /usr/local/bin

 # Create symlinks to the cli tools
 sudo ln -s ${LOCATION}/BitcoinSubsidium-Qt.app/Contents/MacOS/BitcoinSubsidiumd /usr/local/bin/BitcoinSubsidiumd
 sudo ln -s ${LOCATION}/BitcoinSubsidium-Qt.app/Contents/MacOS/BitcoinSubsidium-cli /usr/local/bin/BitcoinSubsidium-cli
