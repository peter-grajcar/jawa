/**
 * @file tables.hpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_TABLES_HPP
#define JAWA_TABLES_HPP

#include <unordered_set>

#include "type.hpp"

namespace jawa
{

    using TypeObs = const jasm::Type *;
    using VoidTypeObs = const jasm::VoidType *;
    using IntTypeObs = const jasm::IntType *;
    using MethodTypeObs = const jasm::MethodSignatureType *;
    using TypeObsArray = std::vector<TypeObs>;
    using ClassTypeObs = const jasm::ClassType *;
    using ArrayTypeObs = const jasm::ArrayType *;

    class TypeTable
    {
    private:
        struct type_hasher_t
        {
            std::size_t operator()(const jasm::Type &t) const { return t.hash(); }
        };

        jasm::VoidType void_type_;
        jasm::IntType int_type_;
        std::unordered_set<jasm::MethodSignatureType, type_hasher_t> method_types_;
        std::unordered_set<jasm::ArrayType, type_hasher_t> array_types_;
        std::unordered_set<jasm::ClassType, type_hasher_t> class_types_;

    public:
        TypeTable() = default;

        VoidTypeObs get_void_type() const { return &void_type_; }

        IntTypeObs get_int_type() const { return &int_type_; }

        MethodTypeObs get_method_type(TypeObs return_type, TypeObsArray arguments)
        {
            return &*method_types_.emplace(return_type, std::move(arguments)).first;
        }

        ClassTypeObs get_class_type(const Name &class_name)
        {
            return &*class_types_.emplace(class_name).first;
        }

        ArrayTypeObs get_array_type(TypeObs type, size_t dims)
        {
            return &*array_types_.emplace(type, dims).first;
        }

    };


}

#endif //JAWA_TABLES_HPP
