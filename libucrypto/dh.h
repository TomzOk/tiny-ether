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

typedef mbedtls_ecdh_context ucrypto_ecdh_ctx; /*!< caller ref */
typedef mbedtls_ecp_point ecp_point;           /*!< curve struct */
typedef uint8_t ecp_signature[65]; /*!< r: [0, 32), s: [32, 64), v: 64 */

/**
 * @brief Initialize ecdh key context onto heap
 *
 * @return Heap pointer or NULL;
 */
ucrypto_ecdh_ctx* ucrypto_ecdh_key_alloc(mpi*);

/**
 * @brief initialize a key context
 *
 * @param ucrypto_ecdh_ctx
 *
 * @return
 */
int ucrypto_ecdh_key_init(ucrypto_ecdh_ctx*);

/**
 * @brief Given private key (d), calculate public key using group G.
 *
 * Q = d*G
 *
 * @param ucrypto_ecdh_ctx
 *
 * @return
 */
ucrypto_ecdh_ctx* ucrypto_ecdh_import_private_key_alloc(mpi* d);

/**
 * @brief Given private key (d), calculate public key using group G.
 *
 * Q = d*G
 *
 * @param ucrypto_ecdh_ctx
 *
 * @return
 */
int ucrypto_ecdh_import_private_key(ucrypto_ecdh_ctx*, mpi* d);

/**
 * @brief
 *
 * @param ucrypto_ecdh_ctx
 *
 * @return
 */
const ecp_point* ucrypto_ecdh_pubkey(ucrypto_ecdh_ctx*);

/**
 * @brief
 *
 * @param ucrypto_ecdh_ctx
 *
 * @return
 */
const mpi* ucrypto_ecdh_secret(ucrypto_ecdh_ctx*);

/**
 * @brief Compute a shared secret. Secret is updated into ctx z param
 *
 * @param ctx
 *
 * @return 0 OK or -1 err
 */
int ucrypto_ecdh_agree(ucrypto_ecdh_ctx* ctx, const ecp_point*);

/**
 * @brief
 *
 * @param ctx
 * @param b
 * @param sz
 * @param ecp_signature
 *
 * @return
 */
int ucrypto_ecdh_sign(ucrypto_ecdh_ctx* ctx,
                      const uint8_t* b,
                      uint32_t sz,
                      ecp_signature);

/**
 * @brief
 *
 * @param q
 * @param b
 * @param sz
 * @param ecp_signature
 *
 * @return
 */
int ucrypto_ecdh_verify(const ecp_point* q,
                        const uint8_t* b,
                        uint32_t sz,
                        ecp_signature);
/**
 * @brief return any heap from key
 *
 * @param
 */
void ucrypto_ecdh_key_free(ucrypto_ecdh_ctx**);

/**
 * @brief
 *
 * @param ucrypto_ecdh_ctx
 */
void ucrypto_ecdh_key_deinit(ucrypto_ecdh_ctx*);

#ifdef __cplusplus
}
#endif
#endif