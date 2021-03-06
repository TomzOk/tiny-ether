// Copyright 2017 Altronix Corp.
// This file is part of the tiny-ether library
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

/**
 * @author Thomas Chiantia <thomas@altronix>
 * @date 2017
 */

/**
 * @file usys_timers.h
 *
 * @brief Simple wrapper around OS primitives to better support non standard
 * enviorments.
 */
#ifndef USYS_TIMERS_H_
#define USYS_TIMERS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "usys_config.h"
#include "usys_time.h"

#include "khash.h"

/**
 * @brief Typedefs to provide commin interface and hide kh_ details
 */
typedef khint64_t usys_timer_key;
typedef khiter_t usys_timer_iter;

/**
 * @brief Timer context
 */
typedef struct usys_timer
{
    uint32_t ms, fire, flags;
    usys_timer_key key;
    void* ctx;
    int (*fn)(usys_timer_key, void*, uint32_t);
} usys_timer;

/**
 * @brief klib context (klib hash table)
 */
KHASH_MAP_INIT_INT64(usys_timers, usys_timer);

/**
 * @brief Our wrapper around kh_* to provided bounds
 */
typedef struct
{
    uint32_t max;
    kh_usys_timers_t* timers;
} usys_timers_context;

/**
 * @brief Callers on expirey function
 */
typedef int (*usys_timer_fn)(usys_timer_key, void*, uint32_t);

/**
 * @brief Allocate a main timers context
 *
 * @param c Number of timers to allocate
 *
 * @return Returns the main api handle
 */
static inline int
usys_timers_init(usys_timers_context* context, uint32_t c)
{
    context->timers = kh_init(usys_timers);
    if (context->timers) {
        context->max = c;
        kh_resize(usys_timers, context->timers, c);
        return 0;
    }
    return -1;
}

/**
 * @brief Call when no longer need timers or else leak
 *
 * @param self_p
 */
static inline void
usys_timers_deinit(usys_timers_context* self)
{
    kh_destroy_usys_timers(self->timers);
    memset(self, 0, sizeof(usys_timers_context));
}

/**
 * @brief Get number of timers inside the hash table
 *
 * @param context
 *
 * @return
 */
static inline uint32_t
usys_timers_size(usys_timers_context* context)
{
    return kh_size(context->timers);
}

/**
 * @brief Grab a timer context from a key (key returned from insert)
 *
 * @param context
 * @param key
 *
 * @return
 */
static inline usys_timer*
usys_timers_get(usys_timers_context* context, usys_timer_key key)
{
    khiter_t k = kh_get(usys_timers, context->timers, key);
    return k == kh_end(context->timers) ? NULL : &kh_val(context->timers, k);
}

/**
 * @brief Start a timer will fire after ms requires caller to call
 * usys_timers_poll() on the context where the timer is placed
 *
 * @param timer
 */
static inline int
usys_timers_start(usys_timers_context* context, usys_timer_key key, uint32_t ms)
{
    usys_timer* t = usys_timers_get(context, key);
    uint32_t tick = usys_tick();
    if (t) {
        if (ms) t->ms = ms;
        t->flags |= 1;
        t->fire = tick + t->ms;
        return 0;
    }
    return -1;
}

static inline int
usys_timers_cancel(usys_timers_context* context, usys_timer_key key)
{
    usys_timer* t = usys_timers_get(context, key);
    if (t) {
        t->flags &= (~(1));
        t->fire = 0;
    }
    return -1;
}

/**
 * @brief Add a timer to manage
 *
 * @param context main api handle
 * @param key key returned from insert
 * @param fn called when timer times out
 * @param ctx provided to callback
 * @param ms current tick
 *
 * @return
 */
static inline usys_timer_key
usys_timers_insert(
    usys_timers_context* context,
    usys_timer_key key,
    usys_timer_fn fn,
    void* ctx,
    uint32_t ms)
{
    int absent;
    usys_timer* t = NULL;
    khiter_t k;

    if (usys_timers_size(context) < context->max) {
        k = kh_put(usys_timers, context->timers, key, &absent);
        t = &kh_val(context->timers, k);
        t->fn = fn;
        t->ctx = ctx;
        t->ms = ms;
        t->key = key;
        t->flags = t->fire = 0;
        return k;
    }
    return 0;
}

/**
 * @brief Remove a timer from the hash table and free space for new timers.
 *
 * @param context
 * @param key
 */
static inline void
usys_timers_remove(usys_timers_context* self, usys_timer_key key)
{
    usys_timer_iter k = kh_get(usys_timers, self->timers, key);
    if (!(k == kh_end(self->timers))) kh_del(usys_timers, self->timers, k);
}

/**
 * @brief Loop active timers in hash table, fire any time out callbacks
 *
 * @param ctx
 */
static inline void
usys_timers_poll(usys_timers_context* ctx)
{
    usys_timer* t;
    khiter_t iter;
    uint32_t tick = usys_tick();
    for (iter = kh_begin(ctx->timers); iter != kh_end(ctx->timers); iter++) {
        t = &kh_val(ctx->timers, iter);
        if (kh_exist(ctx->timers, iter)) {
            if (t && t->flags && t->fire && tick >= t->fire) {
                t->flags &= (~(1));
                t->fire = 0;
                t->fn(t->key, t->ctx, tick);
            }
        }
    }
}

#ifdef __cplusplus
}
#endif
#endif
