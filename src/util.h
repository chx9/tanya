/*
 * Copyright (C) 2019  Giuseppe Fabio Nicotra <artix2 at gmail dot com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __REDIS_CLUSTER_PROXY_UTIL_H__
#define __REDIS_CLUSTER_PROXY_UTIL_H__
#include <hiredis.h>

#define LLONG_MAX  __LONG_LONG_MAX__
#define LLONG_MIN  (-__LONG_LONG_MAX__-1LL)

void consumeRedisReaderBuffer(redisContext *ctx);
void bytesToHuman(char *s, unsigned long long n);
uint32_t digits10(uint64_t v);
int ll2string(char *s, size_t len, long long value);
int ull2string(char *s, size_t len, unsigned long long value);
redisReply *dupRedisReply(redisReply *src);
int stringmatchlen_(const char *pattern, int patternLen, const char *string, int stringLen, int nocase);
#endif /* __REDIS_CLUSTER_PROXY_UTIL_H__ */
