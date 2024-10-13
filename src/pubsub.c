#include "pubsub.h"
#include "adlist.h"
#include "dict.h"
#include "logger.h"
#include "proxy.h"
#include "sds.h"
extern redisClusterProxy proxy;

void pubsubSubscribeChannel(client *c, sds channel){
    dictEntry *de = NULL;
    proxyThread *thread =  proxy.threads[getCurrentThreadID()]; 
    dict *pubsub_channels = thread->pubsub_channels;
    de = dictFind(pubsub_channels, channel);
    list *clients = NULL;
    if(de == NULL){
        clients = listCreate();
        dictAdd(pubsub_channels, sdsdup(channel), clients);
    }else{
        clients = dictGetVal(de);
    }
    listAddNodeTail(clients, c);
}
void pubsubPsubscribePattern(client *c, sds pattern){
    dictEntry *de = NULL;    
    proxyThread * thread = proxy.threads[getCurrentThreadID()];
    dict *pubsub_patterns = thread->pubsub_patterns;
    de = dictFind(pubsub_patterns, pattern);
    list *clients = NULL;
    if(de == NULL){
        clients = listCreate();
        dictAdd(pubsub_patterns, sdsdup(pattern), clients);
    }else{
        clients = dictGetVal(de);
    }
    listAddNodeTail(clients, c);
}