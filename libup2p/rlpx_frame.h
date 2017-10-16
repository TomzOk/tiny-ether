#ifndef RLPX_FRAME_H_
#define RLPX_FRAME_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Single-frame packet:
 * header || header-mac || frame || frame-mac
 *
 * Multi-frame packet:
 * header || header-mac || frame-0 ||
 * [ header || header-mac || frame-n || ... || ]
 * header || header-mac || frame-last || mac
 *
 * Initiator egress-mac: sha3(mac-secret^recipient-nonce || auth-sent-init)
 *  	       ingress-mac: sha3(mac-secret^initiator-nonce || auth-recvd-ack)
 * Recipient egress-mac: sha3(mac-secret^initiator-nonce || auth-sent-ack)
 *	         ingress-mac: sha3(mac-secret^recipient-nonce ||auth-recvd-init)
 *
 * shared-secret = sha3(ecdhe-shared-secret || sha3(nonce || initiator-nonce))
 * aes-secret = sha3(ecdhe-shared-secret || shared-secret)
 * mac-secret = sha3(ecdhe-shared-secret || aes-secret)
 **/

#include "uaes.h"
#include "uecc.h"
#include "ukeccak256.h"
#include "urlp.h"

typedef struct
{
    ukeccak256_ctx emac; /*!< egress mac */
    ukeccak256_ctx imac; /*!< ingress mac */
    uaes_ctx aes_enc;    /*!< aes dec */
    uaes_ctx aes_dec;    /*!< aes dec */
    uaes_ctx aes_mac;    /*!< aes ecb of egress/ingress mac updates */
} rlpx_coder;

int rlpx_frame_write(rlpx_coder* x,
                     uint32_t type,
                     uint32_t context_id,
                     uint8_t* data,
                     size_t datalen,
                     uint8_t* out,
                     uint32_t* l);
int rlpx_frame_parse(rlpx_coder* x, const uint8_t* frame, size_t l, urlp**);

#ifdef __cplusplus
}
#endif
#endif
