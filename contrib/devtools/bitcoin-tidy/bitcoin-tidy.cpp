// Copyright (c) 2023 Migcoin Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "logprintf.h"

#include <clang-tidy/ClangTidyModule.h>
#include <clang-tidy/ClangTidyModuleRegistry.h>

class MigcoinModule final : public clang::tidy::ClangTidyModule
{
public:
    void addCheckFactories(clang::tidy::ClangTidyCheckFactories& CheckFactories) override
    {
        CheckFactories.registerCheck<migcoin::LogPrintfCheck>("migcoin-unterminated-logprintf");
    }
};

static clang::tidy::ClangTidyModuleRegistry::Add<MigcoinModule>
    X("migcoin-module", "Adds migcoin checks.");

volatile int MigcoinModuleAnchorSource = 0;
