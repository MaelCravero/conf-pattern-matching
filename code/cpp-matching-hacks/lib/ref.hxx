/**
 ** \file misc/ref.hxx
 ** \brief Implementation of misc::ref.
 **/

#pragma once

#include <typeinfo>

#include "ref.hh"

namespace misc
{
    /*-----------------.
    | Ctors and dtor.  |
    `-----------------*/

    template <typename T>
    template <typename U>
    ref<T>::ref(const ref<U>& other)
        : super_type(other)
    {}

    template <typename T>
    ref<T>::ref(const ref<T>& other)
        : super_type(other)
    {}

    template <typename T>
    ref<T>::ref(T* p)
        : super_type(p)
    {}

    /*---------------------.
    | Equality operators.  |
    `---------------------*/

    template <typename T>
    bool ref<T>::operator==(const T* other) const
    {
        return this->get() == other;
    }

    template <typename T>
    bool ref<T>::operator!=(const T* other) const
    {
        return !(*this == other);
    }

    /*--------.
    | Casts.  |
    `--------*/

    template <typename T>
    template <typename U>
    ref<U> ref<T>::unsafe_cast() const
    {
        ref<U> res;
        (std::dynamic_pointer_cast<U, element_type>(*this)).swap(res);
        return res;
    }

    template <typename T>
    template <typename U>
    ref<U> ref<T>::cast() const
    {
        if (!this->get() || !this->is_a<U>())
            throw std::bad_cast();
        return unsafe_cast<U>();
    }

    template <typename T>
    template <typename U>
    bool ref<T>::is_a() const
    {
        return dynamic_cast<U*>(this->get());
    }

} // namespace misc
