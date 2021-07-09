/**
 * @file error.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#include "error.hpp"

namespace jawa::errors
{
    err_nn SYNTAX{"błąd składni: nieoczekiwany znak: \"%\", oczekiwany znak: \"%\""};
    err_s RESERVED{"zarezerwowane słowo kluczowe: \"%\""};
    err_n MALFORMED_STRING{"zniekształcony łańcuch: \"%\""};
    err_c UNEXPECTED_CHAR{"nieoczekiwany znak: \'%\'"};
}