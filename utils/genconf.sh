#!/bin/bash

START_PORT=7000
END_PORT=7005

for port in $(seq $START_PORT $END_PORT); do
  CONFIG_DIR="./$port"  
  
  mkdir -p $CONFIG_DIR

  cat <<EOF > $CONFIG_DIR/redis.conf
port $port
cluster-enabled yes
cluster-config-file nodes-$port.conf
cluster-node-timeout 5000
appendonly no  # 禁用磁盘上的 AOF 日志
save ""  # 禁用从磁盘恢复 RDB 数据
daemonize yes  # 后台运行
protected-mode no
dir $CONFIG_DIR
EOF

  echo "Created configuration for Redis instance on port $port in $CONFIG_DIR."
done

echo "All configuration files have been created in the current directory."

