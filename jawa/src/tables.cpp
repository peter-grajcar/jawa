/**
 * @file tables.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#include "tables.hpp"
#include <class.hpp>
#include <filesystem>
#include <sstream>

namespace jawa {

    size_t JawaMethodSignature::hash() const
    {
        size_t h = std::hash<std::string>()(name);
        for (TypeObs type : argument_types)
            h ^= type->hash();
        return h;
    }

    bool JawaMethodSignature::operator==(const JawaMethodSignature &signature) const
    {
        if (argument_types.size() != signature.argument_types.size())
            return false;
        for (auto type_a = argument_types.begin(), type_b = signature.argument_types.begin();
             type_a < argument_types.end(); ++type_a, ++type_b) {
            if (*type_a != *type_b)
                return false;
        }
        return name == signature.name;
    }

    JawaMethodSignature JawaMethod::signature() const
    {
        return JawaMethodSignature(name_, method_type()->argument_types());
    }

    size_t JawaField::hash() const { return std::hash<std::string>()(name_) ^ type_->hash(); }

    bool JawaField::operator==(const JawaField &field) const { return name_ == field.name_ && type_ == field.type_; }

    size_t JawaClass::hash() const { return std::hash<std::string>()(name_); }

    bool JawaClass::operator==(const JawaClass &clazz) const { return name_ == clazz.name_; }

    const JawaMethod *JawaClass::get_method(const JawaMethodSignature &signature) const
    {
        auto search = methods_.find(signature);
        if (search != methods_.end())
            return &search->second;
        return nullptr;
    }

    JawaClass::JawaClass(TypeTable &type_table, jasm::Class &clazz)
    {
        jasm::ClassConstant *this_class = clazz.this_class();
        jasm::u2 index = this_class->name_index();
        auto *constant = dynamic_cast<jasm::Utf8Constant *>(clazz.constant_pool().get(index));
        name_ = constant->value();

        for (auto &field : clazz.fields()) {
            index = field.name_index();
            auto name_constant = dynamic_cast<jasm::Utf8Constant *>(clazz.constant_pool().get(index));

            index = field.descriptor_index();
            auto descriptor_constant = dynamic_cast<jasm::Utf8Constant *>(clazz.constant_pool().get(index));
            TypeObs type = type_table.from_descriptor(descriptor_constant->value());

            jasm::u2 access_flags = field.access_flags();

            fields_.insert({ name_constant->value(), JawaField(name_constant->value(), type, access_flags) });
        }

        for (auto &method : clazz.methods()) {
            index = method.name_index();
            auto name_constant = dynamic_cast<jasm::Utf8Constant *>(clazz.constant_pool().get(index));

            index = method.descriptor_index();
            auto descriptor_constant = dynamic_cast<jasm::Utf8Constant *>(clazz.constant_pool().get(index));
            auto type = dynamic_cast<MethodTypeObs>(type_table.from_descriptor(descriptor_constant->value()));
            assert(type != nullptr);

            // TODO: modifiers
            jasm::u2 access_flags = method.access_flags();

            JawaMethod jawa_method(name_constant->value(), type, access_flags);
            methods_.insert({ jawa_method.signature(), std::move(jawa_method) });
        }
    }

    const JawaField *JawaClass::get_field(const Name &name) const
    {
        auto search = fields_.find(name);
        if (search != fields_.end())
            return &search->second;
        return nullptr;
    }

    Name ClassTable::find_class_file(const Name &class_name) const
    {
        size_t start = 0;
        do {
            size_t end = class_paths_.find(':');
            std::string class_path(class_paths_, start, end);

            std::string path = class_path;
            path += '/';
            path += class_name;
            path += ".class";

            if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path)) {
                return path;
            }

            start = end;
        } while (start != std::string::npos);

        return "";
    }

    bool ClassTable::import_class(const Name &fully_qualified_name)
    {
        Name file = find_class_file(fully_qualified_name);
        if (file.empty())
            return false;

        std::size_t index = fully_qualified_name.rfind('/');
        Name last_part = fully_qualified_name.substr(index, std::string::npos);

        imported_classes_.insert({ last_part, JawaImport(fully_qualified_name, file) });
        return true;
    }

    void ClassTable::implicit_import()
    {
        std::size_t start = 0;
        do {
            std::size_t end = class_paths_.find(':', start);
            std::string class_path(class_paths_, start, end);
            class_path += "/jawa/jȩzyk";

            if (std::filesystem::exists(class_path) && std::filesystem::is_directory(class_path)) {
                for (const auto &entry : std::filesystem::directory_iterator(class_path)) {
                    if (!entry.is_regular_file())
                        continue;

                    if (entry.path().extension() != ".class")
                        continue;

                    std::string class_name(entry.path().stem());
                    std::string fully_qualified_name("jawa/jȩzyk/");
                    fully_qualified_name += class_name;

                    imported_classes_.insert({ class_name, JawaImport(fully_qualified_name, entry.path()) });
                }
            }

            if (end == std::string::npos)
                break;

            start = end + 1;
        } while (start < class_paths_.size());
    }

    const JawaClass *ClassTable::load_class(const Name &class_name)
    {
        auto search = classes_.find(class_name);
        if (search != classes_.end())
            return &search->second;

        // TODO: check imports
        Name file = find_class_file(class_name);
        if (file.empty())
            return nullptr;

        std::cout << "load " << file << std::endl;
        std::ifstream is(file);
        jasm::Class clazz(is);
        is.close();

        JawaClass jawa_class(type_table_, clazz);
        auto inserted = classes_.insert({ class_name, std::move(jawa_class) });

        return &inserted.first->second;
    }

    Name ClassTable::get_fully_qualified_name(const Name &name)
    {
        auto search = imported_classes_.find(name);
        if (search != imported_classes_.end())
            return search->second.fully_qualified_name;
        if (!find_class_file(name).empty())
            return name;
        return "";
    }

    TypeObs TypeTable::from_descriptor(const Name &descriptor)
    {
        enum Action
        {
            GET_TYPE,     // reads one type
            ADD_ARGUMENT, // adds last read type to the argument list on top of the stack
            MAKE_ARRAY,   // creates an array type with last read type as an element type and
                          // dimensions from the top of the stack
            MAKE_METHOD   // creates a method type using the argument list on the top of the stack and
                          // read type as return type
        };

        std::stack<Action> action_stack;
        std::stack<std::size_t> array_dims;
        std::stack<TypeObsArray> argument_types;

        std::istringstream is(descriptor);
        action_stack.push(GET_TYPE);

        TypeObs type = nullptr;

        while (!action_stack.empty()) {
            Action action = action_stack.top();
            action_stack.pop();
            switch (action) {
            case GET_TYPE:
                switch (is.get()) {
                case jasm::VoidTypePrefix:
                    type = &void_type_;
                    break;
                case jasm::IntTypePrefix:
                    type = &int_type_;
                    break;
                case jasm::ShortTypePrefix:
                    type = &short_type_;
                    break;
                case jasm::LongTypePrefix:
                    type = &long_type_;
                    break;
                case jasm::ByteTypePrefix:
                    type = &byte_type_;
                    break;
                case jasm::BooleanTypePrefix:
                    type = &boolean_type_;
                    break;
                case jasm::FloatTypePrefix:
                    type = &float_type_;
                    break;
                case jasm::DoubleTypePrefix:
                    type = &double_type_;
                    break;
                case jasm::CharTypePrefix:
                    type = &char_type_;
                    break;
                case jasm::ClassTypePrefix: {
                    std::ostringstream name;
                    char ch;
                    while ((ch = is.get()) != ';')
                        name << ch;
                    type = get_class_type(name.str());
                    break;
                }
                case jasm::ArrayTypePrefix: {
                    std::size_t dims = 1;
                    for (; is.peek() == jasm::ArrayTypePrefix; ++dims)
                        is.get();
                    array_dims.push(dims);
                    action_stack.push(MAKE_ARRAY);
                    action_stack.push(GET_TYPE);
                    break;
                }
                case '(':
                    argument_types.emplace();
                    action_stack.push(MAKE_METHOD);
                    action_stack.push(ADD_ARGUMENT);
                    action_stack.push(GET_TYPE);
                    break;
                case ')':
                    // get return type
                    assert(action_stack.top() == ADD_ARGUMENT);
                    action_stack.pop();
                    action_stack.push(GET_TYPE);
                    break;
                default:
                    return nullptr;
                }
                break; // GET_TYPE
            case MAKE_ARRAY:
                type = get_array_type(type, array_dims.top());
                array_dims.pop();
                break;
            case MAKE_METHOD:
                type = get_method_type(type, argument_types.top());
                argument_types.pop();
                break;
            case ADD_ARGUMENT:
                argument_types.top().push_back(type);
                if (is.peek() == ')') {
                    is.get();
                    // get return type
                    action_stack.push(GET_TYPE);
                } else {
                    // get next argument
                    action_stack.push(ADD_ARGUMENT);
                    action_stack.push(GET_TYPE);
                }
                break;
            }
        }

        is.get();
        if (is)
            return nullptr;

        return type;
    }

    const LocalVariable *VariableScope::get_var(const Name &name) const
    {
        auto search = local_variables_.find(name);
        if (search == local_variables_.end())
            return nullptr;
        return &search->second;
    }

    void VariableScope::add_var(const Name &name, TypeObs type, jasm::u2 index)
    {
        local_variables_.emplace(name, LocalVariable{ name, type, index });
    }

    void VariableScopeTable::enter_scope() { scopes_.emplace_back(); }

    void VariableScopeTable::leave_scope()
    {
        assert(scopes_.size() > 0);
        count_ -= scopes_.back().local_variables_.size();
        scopes_.pop_back();
    }

    const LocalVariable *VariableScopeTable::get_var(const Name &name) const
    {
        for (auto scope = scopes_.rbegin(); scope < scopes_.rend(); ++scope) {
            const LocalVariable *var = scope->get_var(name);
            if (var != nullptr)
                return var;
        }
        return nullptr;
    }

    void VariableScopeTable::add_var(const Name &name, TypeObs type)
    {
        assert(scopes_.size() > 0);
        scopes_.back().add_var(name, type, ++count_);
    }
}