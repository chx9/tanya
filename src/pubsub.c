#include "pubsub.h"
#include "adlist.h"
#include "dict.h"
#include "logger.h"
#include "proxy.h"
#include "sds.h"
#include <assert.h>
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

int pubsubUnsubscribeAll(client *c){
    proxyThread *thread = proxy.threads[getCurrentThreadID()];
    int cnt = 0;
    listIter li;
    listNode *ln;
    listRewind(c->subscribed_channels, &li);
    dictEntry *de;
    while((ln = listNext(&li)) != NULL){
        sds channels = ln->value;
        de = dictFind(thread->pubsub_channels, channels);
        list *clients = dictGetVal(de);
        assert(clients!=NULL);
        listNode *client_node = listSearchKey(clients, c);
        if(client_node){
            listDelNode(clients, client_node);
        }
        listDelNode(c->subscribed_channels, ln);
        cnt++;
    }
    listRewind(c->subscribed_patterns, &li);
    while((ln = listNext(&li))!=NULL){
        sds channels = ln->value;
        de = dictFind(thread->pubsub_patterns, channels);
        list *clients = dictGetVal(de);
        assert(clients!=NULL);
        listNode *client_node = listSearchKey(clients, c);
        if(client_node){
            listDelNode(clients, client_node);
        }
        listDelNode(c->subscribed_channels, ln);
        cnt++;
    }
    c->flags &= ~(CLIENT_PUBSUB);
    return cnt;
}

int pubsubUnsubscribe(client *c, sds channel_or_pattern){
    proxyThread * thread = proxy.threads[getCurrentThreadID()];
    int cnt = 0;
    listNode *ln;
    dictEntry *de;
    {
        listIter li;
        listRewind(c->subscribed_channels, &li);
        while((ln = listNext(&li))){
            sds channel = ln->value;
            proxyLogInfo("iter list: %s", channel);
        }
    }
    ln = listSearchKey(c->subscribed_channels, channel_or_pattern);
    if(ln != NULL){
        de = dictFind(thread->pubsub_channels, channel_or_pattern);
        list *clients = dictGetVal(de);
        listNode *client_node = listSearchKey(clients, c);
        if(client_node){
            listDelNode(clients, client_node);
        }
        listDelNode(c->subscribed_channels, ln);
        cnt++;
    }
    ln = listSearchKey(c->subscribed_patterns, channel_or_pattern);
    if(ln != NULL){
        de = dictFind(thread->pubsub_patterns, channel_or_pattern);
        list *clients = dictGetVal(de);
        listNode *client_node = listSearchKey(clients, c);
        if(client_node){
            listDelNode(clients, client_node);
        }
        listDelNode(c->subscribed_patterns, ln);
        cnt++;
    }
    if (listLength(c->subscribed_channels) == 0 && listLength(c->subscribed_patterns) == 0){
        c->flags &= ~(CLIENT_PUBSUB);
    }
    return 0;
}