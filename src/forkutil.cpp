
// Copyright (c) 2017 The BitcoinSubsidium Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "forkutil.h"

bool IsBlockX16R(int blockHeight)
{
    return blockHeight < SCRYPT2_THRESHOLD;
}

bool IsBlockchainX16R()
{
    return IsBlockX16R(nCurrentHeight);
}