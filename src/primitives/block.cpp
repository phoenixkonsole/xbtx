// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2017 The BitcoinSubsidium Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/block.h"

#include "hash.h"
#include "tinyformat.h"
#include "utilstrencodings.h"
#include "crypto/common.h"
#include "crypto/scrypt.h"
#include "chainparams.h"

uint256 CBlockHeader::GetHash() const
{
    return HashX16R(BEGIN(nVersion), END(nNonce), hashPrevBlock);
}

uint256 CBlockHeader::GetWorkHash() const
{
    uint256 thash;
    scryptHash(BEGIN(nVersion), BEGIN(thash));
    return thash;
}

uint256 CBlockHeader::GetMinedHash(const Consensus::Params& params, const int nHeight) const
{
    if (IsPeriodX16R(params, nHeight)) {
        return GetHash();
    }
    return GetWorkHash();
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}
