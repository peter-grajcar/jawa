/**
 * @file main.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */
#include <iostream>
#include "context.hpp"
#include "parser.hpp"

using namespace jawa;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <source file>" << std::endl;
        return 1;
    }

    Context ctx("/Users/petergrajcar/Desktop/classpath");

    FILE *iff = fopen(argv[1], "r");
    if (!iff) {
        std::cerr << "could not open file " << argv[1] << std::endl;
        return 1;
    }

    auto scn = lexer_init(iff);
    parser prs(scn, &ctx);

    prs.parse();

    lexer_shutdown(scn);

    return 0;
}
