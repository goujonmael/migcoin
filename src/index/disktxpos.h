// Copyright (c) 2019-2020 The Migcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MIGCOIN_INDEX_DISKTXPOS_H
#define MIGCOIN_INDEX_DISKTXPOS_H

#include <flatfile.h>
#include <serialize.h>

struct CDiskTxPos : public FlatFilePos
{
    unsigned int nTxOffset{0}; // after header

    SERIALIZE_METHODS(CDiskTxPos, obj)
    {
        READWRITE(AsBase<FlatFilePos>(obj), VARINT(obj.nTxOffset));
    }

    CDiskTxPos(const FlatFilePos &blockIn, unsigned int nTxOffsetIn) : FlatFilePos(blockIn.nFile, blockIn.nPos), nTxOffset(nTxOffsetIn) {
    }

    CDiskTxPos() {}
};

#endif // MIGCOIN_INDEX_DISKTXPOS_H
