// Copyright (c) 2011-2014 The Bitcoin Core developers
// Copyright (c) 2017 The BitcoinSubsidium Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BitcoinSubsidium_QT_BitcoinSubsidiumADDRESSVALIDATOR_H
#define BitcoinSubsidium_QT_BitcoinSubsidiumADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class BitcoinSubsidiumAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BitcoinSubsidiumAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** BitcoinSubsidium address widget validator, checks for a valid BitcoinSubsidium address.
 */
class BitcoinSubsidiumAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BitcoinSubsidiumAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // BitcoinSubsidium_QT_BitcoinSubsidiumADDRESSVALIDATOR_H
