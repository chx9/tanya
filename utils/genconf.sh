#!/bin/bash

START_PORT=7000
END_PORT=7005

for port in $(seq $START_PORT $END_PORT); do
  CONFIG_DIR="./$port"  
  
  mkdir -p testcluster/$CONFIG_DIR

  cat <<EOF > testcluster/$CONFIG_DIR/redis.conf
port $port
cluster-enabled yes
cluster-config-file nodes-$port.conf
cluster-node-timeout 5000
save "" 
daemonize yes  
protected-mode no
dir ./testcluster/$CONFIG_DIR
EOF

  echo "Created configuration for Redis instance on port $port in $CONFIG_DIR."
done

echo "All configuration files have been created in the current directory."

