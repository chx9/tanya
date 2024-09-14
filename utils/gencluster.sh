#!/bin/bash

START_PORT=7000
END_PORT=7005
HOST=127.0.0.1  

for port in $(seq $START_PORT $END_PORT); do
  redis-server ./$port/redis.conf &
  echo "Started Redis instance on port $port."
done

sleep 3

for port in $(seq $START_PORT $END_PORT); do
  if ! redis-cli -p $port ping > /dev/null; then
    echo "Redis instance on port $port failed to start. Aborting cluster creation."
    exit 1
  fi
done

REDIS_CLI="redis-cli"
NODES=""

for port in $(seq $START_PORT $END_PORT); do
  NODES="$NODES $HOST:$port"
done

$REDIS_CLI --cluster create $NODES --cluster-replicas 1 --cluster-yes

echo "Redis cluster created with nodes: $NODES"
