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
#include <unordered_map>
#include <class.hpp>

#include "types.hpp"
#include "type.hpp"

namespace jawa
{

    using TypeObs = const jasm::Type *;
    using VoidTypeObs = const jasm::VoidType *;
    using IntTypeObs = const jasm::IntType *;
    using ByteTypeObs = const jasm::ByteType *;
    using CharTypeObs = const jasm::CharType *;
    using ShortTypeObs = const jasm::ShortType *;
    using LongTypeObs = const jasm::LongType *;
    using FloatTypeObs = const jasm::FloatType *;
    using DoubleTypeObs = const jasm::DoubleType *;
    using BooleanTypeObs = const jasm::BooleanType *;
    using MethodTypeObs = const jasm::MethodType *;
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
        jasm::BooleanType boolean_type_;
        jasm::ShortType short_type_;
        jasm::LongType long_type_;
        jasm::ByteType byte_type_;
        jasm::FloatType float_type_;
        jasm::DoubleType double_type_;
        jasm::CharType char_type_;
        std::unordered_set<jasm::MethodType, type_hasher_t> method_types_;
        std::unordered_set<jasm::ArrayType, type_hasher_t> array_types_;
        std::unordered_set<jasm::ClassType, type_hasher_t> class_types_;

    public:
        TypeTable() = default;

        TypeObs from_descriptor(const Name &descriptor);

        VoidTypeObs get_void_type() const { return &void_type_; }

        IntTypeObs get_int_type() const { return &int_type_; }

        ByteTypeObs get_byte_type() const { return &byte_type_; }

        ShortTypeObs get_short_type() const { return &short_type_; }

        CharTypeObs get_char_type() const { return &char_type_; }

        LongTypeObs get_long_type() const { return &long_type_; }

        FloatTypeObs get_float_type() const { return &float_type_; }

        DoubleTypeObs get_double_type() const { return &double_type_; }

        BooleanTypeObs get_boolean_type() const { return &boolean_type_; }

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

    struct JawaMethodSignature
    {
        Name name;
        TypeObsArray argument_types;

        JawaMethodSignature(Name name, TypeObsArray argument_types) : name(std::move(name)),
                                                                      argument_types(std::move(argument_types))
        {
            for (TypeObs arg : argument_types)
                assert(arg != nullptr);
        }

        std::size_t hash() const;

        bool operator==(const JawaMethodSignature &signature) const;
    };

    struct JawaMethod
    {
        Name name;
        MethodTypeObs type;

        JawaMethod(Name name, MethodTypeObs type) : name(std::move(name)), type(type)
        {
            assert(type != nullptr);
        }

        JawaMethodSignature signature() const;
    };

    struct JawaField
    {
        Name name;
        TypeObs type;
        jasm::u2 access_flags;

        JawaField(Name name, TypeObs type, jasm::u2 access_flags)
                : name(std::move(name)), type(type), access_flags(access_flags)
        {
            assert(type != nullptr);
        }

        std::size_t hash() const;

        bool operator==(const JawaField &field) const;
    };

    struct JawaImport
    {
        Name fully_qualified_name;
        Name class_file_path;

        JawaImport(Name fully_qualified_name, Name class_file_path) : fully_qualified_name(fully_qualified_name),
                                                                      class_file_path(class_file_path) {}
    };

    class JawaClass
    {
    private:
        struct signature_hasher_t
        {
            std::size_t operator()(const JawaMethodSignature &signature) const
            {
                return signature.hash();
            }
        };

        Name name_;
        std::unordered_map<JawaMethodSignature, JawaMethod, signature_hasher_t> methods_;
        std::unordered_map<Name, JawaField> fields_;
    public:
        JawaClass(TypeTable &type_table, jasm::Class &clazz);

        const JawaMethod *get_method(const JawaMethodSignature &signature) const;

        const JawaField *get_field(const Name &name) const;

        inline Name class_name() const
        {
            return name_;
        }

        std::size_t hash() const;

        bool operator==(const JawaClass &clazz) const;
    };

    class ClassTable
    {
    private:
        std::string class_paths_;

        TypeTable &type_table_;

        std::unordered_map<Name, JawaClass> classes_;

        std::unordered_map<Name, JawaImport> imported_classes_;

        Name find_class_file(const Name &class_name) const;

        void implicit_import();

    public:
        ClassTable(TypeTable &type_table, std::string class_paths) : class_paths_(std::move(class_paths)),
                                                                     type_table_(type_table)
        {
            implicit_import();
        }

        Name get_fully_qualified_name(const Name &name);

        /**
         * Imports a class. Note that import itself does not load the class' content.
         *
         * @param fully_qualified_name fully qualified class name.
         * @return true if the class was successfully imported, false otherwise.
         */
        bool import_class(const Name &fully_qualified_name);

        /**
         * Loads the content of a given class.
         *
         * @param class_name name of the class to be loaded.
         * @return loaded class.
         */
        const JawaClass *load_class(const Name &class_name);
    };

}

#endif //JAWA_TABLES_HPP
