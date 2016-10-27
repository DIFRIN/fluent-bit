/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*  Fluent Bit
 *  ==========
 *  Copyright (C) 2015-2016 Treasure Data Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef FLB_MEM_H
#define FLB_MEM_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_macros.h>

#ifdef FLB_HAVE_JEMALLOC
#include <jemalloc/jemalloc.h>
#endif

#include <stdlib.h>

/*
 * The following memory handling wrappers, aims to simplify the way to use
 * the default memory allocator from the libc or an alternative one as Jemalloc.
 *
 * Here there is no error logging in case of failures, we defer that task to the
 * caller.
 */

#if ((__GNUC__ * 100 + __GNUC__MINOR__) > 430)  /* gcc version > 4.3 */
# define ALLOCSZ_ATTR(x,...) __attribute__ ((alloc_size(x, ##__VA_ARGS__)))
#else
# define ALLOCSZ_ATTR(x,...)
#endif

static inline ALLOCSZ_ATTR(1)
void *flb_malloc(const size_t size) {
    void *aux;

#ifdef FLB_HAVE_JEMALLOC
    aux = je_malloc(size);
#else
    aux = malloc(size);
#endif

    if (flb_unlikely(!aux && size)) {
        return NULL;
    }

    return aux;
}

static inline ALLOCSZ_ATTR(1)
void *flb_calloc(size_t n, const size_t size) {
    void *buf;

#ifdef FLB_HAVE_JEMALLOC
    buf = je_calloc(n, size);
#else
    buf = calloc(n, size);
#endif

    if (flb_unlikely(!buf)) {
        return NULL;
    }

    return buf;
}

static inline ALLOCSZ_ATTR(2)
void *flb_realloc(void *ptr, const size_t size)
{
    void *aux;

#ifdef FLB_HAVE_JEMALLOC
    aux = je_realloc(ptr, size);
#else
    aux = realloc(ptr, size);
#endif

    if (flb_unlikely(!aux && size)) {
        return NULL;
    }

    return aux;
}

static inline void flb_free(void *ptr) {
#ifdef FLB_HAVE_JEMALLOC
    je_free(ptr);
#else
    free(ptr);
#endif
}

#endif
