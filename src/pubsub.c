#include "pubsub.h"
#include "adlist.h"
#include "dict.h"
#include "proxy.h"
extern redisClusterProxy proxy;

void pubsubSubscribeChannel(client *c, sds channel){
    dictEntry *de = NULL;
    proxyThread *thread =  proxy.threads[getCurrentThreadID()]; 
    dict *pubsub_channels = thread->pubsub_channels;
    de = dictFind(pubsub_channels, channel);
    list *clients = NULL;
    if(de == NULL){
        clients = listCreate();
        dictAdd(pubsub_channels, channel, clients);
    }else{
        clients = dictGetVal(de);
    }
    listAddNodeTail(clients, c);
}