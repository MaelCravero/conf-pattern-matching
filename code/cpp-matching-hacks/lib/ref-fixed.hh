/**
 ** \file misc/ref.hh
 ** \brief Declaration of misc::ref.
 **/

#pragma once

#include <memory>

namespace misc
{
    /// A std::shared_ptr wrapper.
    ///
    /// Compared to its super type, this implementation provides
    /// cast operators, and implicit constructors.
    template <typename T>
    class ref : public std::shared_ptr<T>
    {
    public:
        /// The parent class.
        using super_type = std::shared_ptr<T>;
        /// The type pointed to.
        using element_type = T;

        /// \name Constructors & Destructor.
        /// \{
        /** \brief Construct a new reference to the value pointed to by \a
         *other.
         ** The new reference shares the property of the object with \a other.
         */
        template <typename U>
        requires std::derived_from<U, T> ref(const ref<U>& other);

        /** \brief Copy constructor.
         **
         ** Although the implementation is subsumed by the previous, more
         ** generic one, the C++ standard still mandates this specific
         ** signature.  Otherwise, the compiler will provide a default
         ** implementation, which is of course wrong.  Note that the
         ** same applies for the assignment operator. */
        ref(const ref<T>& other);

        /** \brief Construct a counted reference to a newly allocated object.
         ** The new reference takes the property of the object pointed to
         ** by \a p.  If \a p is null, then the reference is invalid and
         ** must be \c reset() before use. */
        ref(T* p = nullptr);

        /// \name Equality operators.
        /// \{

        /** \brief Reference comparison.
         ** Returns true if this points to \a p. */
        bool operator==(const T* p) const;

        /** \brief Reference comparison.
         ** Returns false if this points to \a p. */
        bool operator!=(const T* p) const;

        /// \}

        /// \name Assignment operators.
        /// \{

        /** \brief Use default copy-assignment operator. */
        ref<T>& operator=(const ref<T>& r) = default;

        /// \}

        /// \name Casts.
        /// \{

        /** \brief Cast the reference.
         ** Return a new reference, possibly throwing an exception if the
         ** dynamic_cast is invalid.
         **/
        template <typename U>
        ref<U> cast() const;

        /** \brief Cast the reference (unsafe).
         ** Return a new reference, possibly a null reference if the
         ** dynamic_cast is invalid.
         **/
        template <typename U>
        ref<U> unsafe_cast() const;
        /// \}

        /** \brief Test fellowship.
         ** Return true if the reference points to an object which is
         ** really of the specified type.
         **/
        template <typename U>
        bool is_a() const;
    };

} // namespace misc

#include "ref-fixed.hxx"
