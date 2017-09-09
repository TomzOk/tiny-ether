#ifndef DH_H_
#define DH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mbedtls/ctr_drbg.h"
#include "mbedtls/ecdh.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/entropy.h"
#include "mpi.h"

/*!< r: [0, 32), s: [32, 64), v: 64 */
typedef uint8_t ucrypto_ecc_signature[65];
typedef uint8_t ucrypto_ecc_public_key[65];
typedef ucrypto_mpi ucrypto_ecc_private_key;
typedef mbedtls_ecdh_context ucrypto_ecc_ctx; /*!< caller ref */
typedef mbedtls_ecp_point ucrypto_ecp_point;  /*!< curve struct */

/**
 * @brief initialize a key context
 *
 * @param ucrypto_ecc_ctx
 *
 * @return
 */
int ucrypto_ecc_key_init(ucrypto_ecc_ctx*, const ucrypto_mpi* d);

/**
 * @brief
 *
 * @param ctx
 *
 * @return
 */
int ucrypto_ecc_init_keypair(ucrypto_ecc_ctx* ctx);

/**
 * @brief Given private key (d), calculate public key using group G.
 *
 * Q = d*G
 *
 * @param ucrypto_ecc_ctx
 *
 * @return
 */
int ucrypto_ecc_import_keypair(ucrypto_ecc_ctx*, const ucrypto_mpi* d);

/**
 * @brief
 *
 * @param ucrypto_ecc_ctx
 */
void ucrypto_ecc_key_deinit(ucrypto_ecc_ctx*);

/**
 * @brief
 *
 * @param ucrypto_ecc_ctx
 *
 * @return
 */
const ucrypto_ecp_point* ucrypto_ecc_pubkey(ucrypto_ecc_ctx*);

/**
 * @brief
 *
 * @param ucrypto_ecc_ctx
 *
 * @return
 */
const ucrypto_mpi* ucrypto_ecc_secret(ucrypto_ecc_ctx*);

/**
 * @brief
 *
 * @param str
 * @param rdx
 * @param q
 *
 * @return
 */
int ucrypto_ecc_atop(const char* str, int rdx, ucrypto_ecp_point* q);

/**
 * @brief
 *
 * @param ucrypto_ecc_ctx
 * @param b
 *
 * @return
 */
int ucrypto_ecc_ptob(ucrypto_ecc_ctx*, ucrypto_ecc_public_key* b);

/**
 * @brief
 *
 * @param ctx
 * @param k
 *
 * @return
 */
int ucrypto_ecc_agree(ucrypto_ecc_ctx* ctx, const ucrypto_ecc_public_key* k);

/**
 * @brief Compute a shared secret. Secret is updated into ctx z param
 *
 * @param ctx
 *
 * @return 0 OK or -1 err
 */
int ucrypto_ecc_agree_point(ucrypto_ecc_ctx* ctx, const ucrypto_ecp_point*);

/**
 * @brief
 *
 * @param ctx
 * @param b
 * @param sz
 * @param ucrypto_ecc_signature
 *
 * @return
 */
int ucrypto_ecc_sign(ucrypto_ecc_ctx* ctx,
                     const uint8_t* b,
                     uint32_t sz,
                     ucrypto_ecc_signature*);

/**
 * @brief
 *
 * @param q
 * @param b
 * @param sz
 * @param ucrypto_ecc_signature
 *
 * @return
 */
int ucrypto_ecc_verify(const ucrypto_ecp_point* q,
                       const uint8_t* b,
                       uint32_t sz,
                       ucrypto_ecc_signature*);

#ifdef __cplusplus
}
#endif
#endif
