/**
 * @file error.cpp
 * Copyright (c) 2021 Peter Grajcar
 */

#include "error.hpp"

namespace jawa::errors
{
    err_n SYNTAX{"Syntax Error", "błąd składni: "};
    err_n RESERVED{"Reserved Keyword", "zarezerwowane słowo kluczowe: "};
    err_c UNEXPECTED_CHAR{"Unexpected Character", "nieoczekiwany znak: "};
}