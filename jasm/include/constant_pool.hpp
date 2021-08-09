/**
 * @file constant_pool.hpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_CONSTANT_POOL_HPP
#define JAWA_CONSTANT_POOL_HPP

#include <cinttypes>
#include <memory>
#include <vector>

#include "byte_code.hpp"
#include "constant.hpp"

namespace jasm {

    class ConstantPool
    {
    private:
        std::vector<std::unique_ptr<Constant>> pool_;

    public:
        /**
         * Constant pool tags defined as defined in the JVM specification.
         */
        enum Tag : u1
        {
            CONSTANT_UTF_8 = 1,
            CONSTANT_INTEGER = 3,
            CONSTANT_FLOAT = 4,
            CONSTANT_LONG = 5,
            CONSTANT_DOUBLE = 6,
            CONSTANT_CLASS = 7,
            CONSTANT_STRING = 8,
            CONSTANT_FIELD_REF = 9,
            CONSTANT_METHOD_REF = 10,
            CONSTANT_INTERFACE_METHOD_REF = 11,
            CONSTANT_NAME_AND_TYPE = 12,
            CONSTANT_METHOD_HANDLE = 15,
            CONSTANT_METHOD_TYPE = 16,
            CONSTANT_INVOKE_DYNAMIC = 18,
        };

        /**
         * Creates a new constant in the constant pool.
         *
         * @tparam T Constant type.
         * @tparam Args Constant type constructor argument types.
         * @param args Constant type constructor arguments.
         */
        template<typename T, typename... Args>
        inline u2
        make_constant(Args... args)
        {
            pool_.emplace_back(std::make_unique<T>(args...));
            return pool_.size();
        }

        inline std::vector<std::unique_ptr<Constant>>::const_iterator
        begin() const
        {
            return pool_.begin();
        }

        inline std::vector<std::unique_ptr<Constant>>::const_iterator
        end() const
        {
            return pool_.end();
        }

        inline std::vector<std::unique_ptr<Constant>>::iterator
        begin()
        {
            return pool_.begin();
        }

        inline std::vector<std::unique_ptr<Constant>>::iterator
        end()
        {
            return pool_.end();
        }

        inline const Constant *
        operator[](u2 index) const
        {
            return get(index);
        }

        inline Constant *
        operator[](u2 index)
        {
            return get(index);
        }

        inline const Constant *
        get(u2 index) const
        {
            assert(index > 0);
            return pool_[index - 1].get();
        }

        inline Constant *
        get(u2 index)
        {
            assert(index > 0);
            return pool_[index - 1].get();
        }

        inline u2
        count() const
        {
            return pool_.size();
        }
    };

}

#endif // JAWA_CONSTANT_POOL_HPP
