/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/times.h>

#define TEST_LEN    1000000

static void time_dump(int ticks, clock_t start, clock_t stop, struct tms *start_tms, struct tms *stop_tms)
{
    printf("  real time: %lf\n", (stop - start) / (double)ticks);
    printf("  user time: %lf\n", (stop_tms->tms_utime - start_tms->tms_utime) / (double)ticks);
    printf("  kern time: %lf\n", (stop_tms->tms_stime - start_tms->tms_stime) / (double)ticks);
}

int main(void)
{
    ngx_rbtree_t bench_root;
    ngx_rbtree_node_t sentinel;
    ngx_rbtree_node_t *node;
    struct tms start_tms, stop_tms;
    clock_t start, stop;
    unsigned int count, ticks;

    ticks = sysconf(_SC_CLK_TCK);
    ngx_rbtree_init(&bench_root, &sentinel, ngx_rbtree_insert_value);

    printf("Generate %d Node:\n", TEST_LEN);
    start = times(&start_tms);
    for (count = 0; count < TEST_LEN; ++count) {
        node = malloc(sizeof(*node));
        if (!node) {
            printf("Insufficient Memory!\n");
            return -1;
        }

        node->key = ((unsigned long)rand() << 32) | rand();
        ngx_rbtree_insert(&bench_root, node);
    }
    stop = times(&stop_tms);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    return 0;
}
