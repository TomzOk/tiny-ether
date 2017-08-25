/**
 * @file urlp.h
 *
 * @brief api and types
 */
#ifndef URLP_H_
#define URLP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "urlp_config.h"

/**
 * @brief urlp context
 * Doubly linked list with tail pointers and extra padding for node rlp.
 */
typedef struct urlp {
    struct urlp* next;
    uint32_t sz, spot;  // TODO get rid of spot since sz always exact
    uint8_t b[];
} urlp;

typedef void (*urlp_walk_fn)(urlp*, void*);

urlp* urlp_item(const uint8_t*, uint32_t);
urlp* urlp_list(int n, ...);
urlp* urlp_push(urlp*, urlp*);
uint32_t urlp_size(urlp* rlp);
const uint8_t* urlp_data(urlp* rlp);
void urlp_walk(urlp*, urlp_walk_fn, void*);
uint32_t urlp_print(urlp* rlp, uint8_t* b, uint32_t sz);
void urlp_free(urlp**);

/*
 * Things that are not an rlp list (where[NULL]is rlp->sz==0)
 * [NULL]->
 * [NULL]->[dog]
 * [dog]->
 * #define urlp_is_list(rlp) \
 *     ((rlp->sz && rlp->next) || (!rlp->sz && rlp->next && rlp->next))
 */

#define urlp_is_list(rlp) ((rlp->sz && rlp->next) || (rlp->next && rlp->next))

#ifdef __cplusplus
}
#endif
#endif
