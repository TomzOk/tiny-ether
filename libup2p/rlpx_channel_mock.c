#include "rlpx_channel.h"

rlpx_channel*
rlpx_ch_mock_alloc(async_io_settings* s,
                   uecc_private_key* skey,
                   uecc_private_key* ekey)
{
    rlpx_channel* ch = rlpx_malloc(sizeof(rlpx_channel));
    if (ch) rlpx_ch_mock_init(ch, s, skey, ekey);
    return ch;
}

int
rlpx_ch_mock_init(rlpx_channel* ch,
                  async_io_settings* settings,
                  uecc_private_key* skey,
                  uecc_private_key* ekey)
{
    rlpx_ch_init(ch, skey, ekey);
    // Override io
    if (settings->connect) ch->io.settings.connect = settings->connect;
    if (settings->ready) ch->io.settings.ready = settings->ready;
    if (settings->close) ch->io.settings.close = settings->close;
    if (settings->tx) ch->io.settings.tx = settings->tx;
    if (settings->rx) ch->io.settings.rx = settings->rx;
    return 0;
}