// Copyright (c) 2017 The BitcoinSubsidium Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

/**
 * Server/client environment: argument handling, config file parsing,
 * logging, thread wrappers, startup time
 */
#ifndef BitcoinSubsidium_FORKUTIL_H
#define BitcoinSubsidium_FORKUTIL_H

static int nCurrentHeight = 0;

static const int SCRYPT2_THRESHOLD = 667956;
bool IsBlockX16R(int blockHeight);

bool IsBlockchainX16R();

#endif // BitcoinSubsidium_FORKUTIL_H