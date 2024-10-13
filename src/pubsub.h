#ifndef __REDIS_CLUSTER_PROXY_PUBSUB_H__
#define __REDIS_CLUSTER_PROXY_PUBSUB_H__

#include "proxy.h"
void pubsubSubscribeChannel(client *c, sds channel);
void pubsubPsubscribePattern(client *c, sds pattern);
#endif