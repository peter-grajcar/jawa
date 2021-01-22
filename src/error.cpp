/**
 * @file error.cpp
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