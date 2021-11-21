/*-*-c++-*-*/
/*
 * Copyright 2021 John Sallay
 *
 * SPDX-License-Identifier: LGPL-3.0
 *
 */
#pragma once

#include <pmtf/pmtf_generated.h>
#include <pmtf/base.hpp>
#include <pmtf/wrap.hpp>
#include <complex>
#include <ostream>
#include <map>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <type_traits>


namespace pmtf {

/**
 * @brief Class holds the implementation of a scalar pmt.
 *
 * The scalar types are defined in scalar.cpp.  This class should not be
 * used directly. It would be nice to move it to the .cpp and instantiate all
 * of the templates in there. This would involve a fairly large refactoring of
 * the code.
 */
template <class T>
class scalar_value : public base
{
public:
    typedef std::shared_ptr<scalar_value> sptr;
    static sptr make(const T value) { return std::make_shared<scalar_value<T>>(value); }
    static sptr from_buffer(const uint8_t* buf)
    {
        return std::make_shared<scalar_value<T>>(buf);
    }
    static sptr from_pmt(const pmtf::Pmt* fb_pmt)
    {
        return std::make_shared<scalar_value<T>>(fb_pmt);
    }

    void set_value(const T& val);
    T value();
    const T value() const;

    scalar_value& operator=(const T& other) // copy assignment
    {
        set_value(other);
        return *this;
    }
    scalar_value& operator=(const scalar_value& other)
    {
        if (this == &other) return *this;
        this->set_value(other.value());
        return *this;
    }

    flatbuffers::Offset<void> rebuild_data(flatbuffers::FlatBufferBuilder& fbb);

    scalar_value(const T& val);
    scalar_value(const uint8_t* buf);
    scalar_value(const pmtf::Pmt* fb_pmt);

    bool is_scalar() const noexcept { return true; }
    void print(std::ostream& os) const { os << value(); }
    
};


// These structures allow us to see if a arbitrary type is a scalar_value
// or not.
template <class T>
struct is_pmt_scalar_value : std::false_type {};

template <class T>
struct is_pmt_scalar_value<scalar_value<T>> : std::true_type {};

/**
 * @brief compare scalar_value against something else
 *
 * Allow for comparisons against other pmt scalars and other types.
 * For example scalar_value<int>(4) == 4.0 will be true.
 */
template <class T, class U>
bool operator==(const scalar_value<T>& x, const U& y) {
    if constexpr(std::is_same_v<T, U>)
        return x.value() == y;
    else if constexpr(is_pmt_scalar_value<U>::value)
        return x.value() == y.value();
    else if constexpr(std::is_convertible_v<U, T>)
        return x.value() == T(y);
    return false;
}

/**
 * @brief "Print" out a scalar_value
 */
template <class T>
std::ostream& operator<<(std::ostream& os, const scalar_value<T>& value) {
    os << value;
    return os;
}
}