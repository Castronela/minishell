#!/bin/bash

# Define the configuration file path
config_file="app_config.conf"

# Use heredoc to create the configuration file
cat <<EOF > $config_file
# Application Configuration File
# Generated on $(date)

[database]
host = localhost
port = 5432
username = admin
password = secret

[server]
port = 8080
max_clients = 100

[logging]
level = info
file = /var/log/app.log
EOF

echo "Configuration file '$config_file' created successfully."