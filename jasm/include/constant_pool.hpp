/**
 * @file constant_pool.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_CONSTANT_POOL_HPP
#define JAWA_CONSTANT_POOL_HPP

#include <cinttypes>
#include <vector>
#include <memory>

#include "byte_code.hpp"
#include "constant.hpp"

namespace jasm
{

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
        template <typename T, typename ...Args>
        void make_constant(Args ...args)
        {
            pool_.push_back(std::make_unique<T>(args...));
        }

        std::vector<std::unique_ptr<Constant>>::iterator begin()
        {
            return pool_.begin();
        }

        std::vector<std::unique_ptr<Constant>>::iterator end()
        {
            return pool_.end();
        }

    };


}

#endif //JAWA_CONSTANT_POOL_HPP
