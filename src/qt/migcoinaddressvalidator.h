// Copyright (c) 2011-2020 The Migcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MIGCOIN_QT_MIGCOINADDRESSVALIDATOR_H
#define MIGCOIN_QT_MIGCOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class MigcoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit MigcoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** Migcoin address widget validator, checks for a valid migcoin address.
 */
class MigcoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit MigcoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // MIGCOIN_QT_MIGCOINADDRESSVALIDATOR_H
