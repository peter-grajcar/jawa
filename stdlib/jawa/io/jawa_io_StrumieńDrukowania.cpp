/**
 * @file jawa_io_StrumieńDrukowania.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 * 
 * Copyright (c) 2021 Peter Grajcar
 */
#include "jawa_io_StrumieńDrukowania.hpp"
#include <iostream>

JNIEXPORT void JNICALL
Java_jawa_io_Strumie_00144Drukowania_wydrukova_00107(JNIEnv *env, jobject obj, jstring str)
{
    jboolean is_copy = false;
    std::cout << (const char *) env->GetStringChars(str, &is_copy) << std::endl;
}
