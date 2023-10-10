// Copyright (c) 2020-2022 The Migcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MIGCOIN_TEST_UTIL_INDEX_H
#define MIGCOIN_TEST_UTIL_INDEX_H

class BaseIndex;

/** Block until the index is synced to the current chain */
void IndexWaitSynced(const BaseIndex& index);

#endif // MIGCOIN_TEST_UTIL_INDEX_H
