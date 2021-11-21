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

// Forward declare the scalar_value class
template <class T>
class scalar_value;

/**
 * @brief Wrapper class around a smart pointer to a scalar_value.
 *
 * This is the interface that should be used for dealing with scalar values.
 */
template <class T>
class scalar {
public:
    using sptr = std::shared_ptr<scalar_value<T>>;
    //! Construct a scalar from a scalar value
    scalar(const T& val): d_ptr(scalar_value<T>::make(val)) {}
    //! Construct a scalar from a scalar_value pointer.
    scalar(sptr ptr):
        d_ptr(ptr) {}
    //! Copy constructor.
    scalar(const scalar<T>& x):
        d_ptr(x.d_ptr) {}
   
    //! Get at the smart pointer.
    sptr ptr() const { return d_ptr; }
    bool operator==(const T& val) const { return *d_ptr == val;}
    bool operator==(const scalar<T>& val) const { return *d_ptr == *val.d_ptr; }
    auto data_type() { return d_ptr->data_type(); }
    T value() const { return d_ptr->value(); }


    // Make it act like a pointer.  Probably need a better way
    // to think about it.
    T& operator*() const { return *d_ptr; }
    // Cast operators
    //! Cast to a T value.
    //! Explicit means that the user must do something like T(scalar<T>(val));
    //! Implicit conversions can cause lots of problems, so we are avoiding them.
    explicit operator T() const { return d_ptr->value(); }
    //! Cast to another type
    //! Will cause a compilation failure if we can't do the cast.
    template <class U>
    explicit operator U() const { return U(d_ptr->value()); }
    
private:
    sptr d_ptr;
};

template <class T>
std::ostream& operator<<(std::ostream& os, const scalar<T>& value) {
    os << *(value.ptr());
    return os;
}

template <class T, Data dt>
scalar<T> _get_pmt_scalar(const wrap& x);

template <class T>
scalar<T> get_scalar(const wrap& x);

template <class T>
T get_scalar_value(const wrap& x);

// Fix this later with SFINAE
template <class T, Data dt>
bool _can_be(const wrap& x) {
    auto value = get_scalar<typename cpp_type<dt>::type>(x);
    return std::is_convertible_v<typename cpp_type<dt>::type, T>;
}

template <class T>
bool can_be(const wrap& x) {
    switch(auto dt = x.ptr()->data_type()) {
        case Data::ScalarFloat32: return _can_be<T, Data::ScalarFloat32>(x);
        case Data::ScalarFloat64: return _can_be<T, Data::ScalarFloat64>(x);
        case Data::ScalarComplex64: return _can_be<T, Data::ScalarComplex64>(x);
        case Data::ScalarComplex128: return _can_be<T, Data::ScalarComplex128>(x);
        case Data::ScalarInt8: return _can_be<T, Data::ScalarInt8>(x);
        case Data::ScalarInt16: return _can_be<T, Data::ScalarInt16>(x);
        case Data::ScalarInt32: return _can_be<T, Data::ScalarInt32>(x);
        case Data::ScalarInt64: return _can_be<T, Data::ScalarInt64>(x);
        case Data::ScalarUInt8: return _can_be<T, Data::ScalarUInt8>(x);
        case Data::ScalarUInt16: return _can_be<T, Data::ScalarUInt16>(x);
        case Data::ScalarUInt32: return _can_be<T, Data::ScalarUInt32>(x);
        case Data::ScalarUInt64: return _can_be<T, Data::ScalarUInt64>(x);
        case Data::ScalarBool: return _can_be<T, Data::ScalarBool>(x);
        //case Data::PmtString: return _can_be<T, Data::PmtString>(x);
        default: return false;
    }
    
}

// template <class T, Data dt>
// T _get_as(const wrap& x) {
//     auto value = get_scalar<typename cpp_type<dt>::type>(x);
//     if constexpr(std::is_convertible_v<typename cpp_type<dt>::type, T>)
//         return T(value.ptr()->value());
//     else
//         throw std::runtime_error("Cannot convert types");
// }

// template <class T>
// T get_as(const wrap& x) {
//     switch(auto dt = x.ptr()->data_type()) {
//         case Data::ScalarFloat32: return _get_as<T, Data::ScalarFloat32>(x);
//         case Data::ScalarFloat64: return _get_as<T, Data::ScalarFloat64>(x);
//         case Data::ScalarComplex64: return _get_as<T, Data::ScalarComplex64>(x);
//         case Data::ScalarComplex128: return _get_as<T, Data::ScalarComplex128>(x);
//         case Data::ScalarInt8: return _get_as<T, Data::ScalarInt8>(x);
//         case Data::ScalarInt16: return _get_as<T, Data::ScalarInt16>(x);
//         case Data::ScalarInt32: return _get_as<T, Data::ScalarInt32>(x);
//         case Data::ScalarInt64: return _get_as<T, Data::ScalarInt64>(x);
//         case Data::ScalarUInt8: return _get_as<T, Data::ScalarUInt8>(x);
//         case Data::ScalarUInt16: return _get_as<T, Data::ScalarUInt16>(x);
//         case Data::ScalarUInt32: return _get_as<T, Data::ScalarUInt32>(x);
//         case Data::ScalarUInt64: return _get_as<T, Data::ScalarUInt64>(x);
//         case Data::ScalarBool: return _get_as<T, Data::ScalarBool>(x);
//     }
// }

// Define constructors for wrap for the scalar types
// In c++20, I think we could do this with a concept.
// I'm struggling to get SFINAE working.  I'm not sure if it is possible here, so I'm using macros.  Sorry.
// Construct a wrap from a scalar type
#define WrapConstruct(type) \
    template <> wrap::wrap<type>(const type& x);
// Construct a wrap from a scalar
#define WrapConstructPmt(type) \
    template <> wrap::wrap<scalar<type>>(const scalar<type>& x);

#define Equals(type) \
    template <> bool operator==<type>(const wrap& x, const type& other);

#define EqualsPmt(type) \
    template <> bool operator==<scalar<type>>(const wrap& x, const scalar<type>& other);

#define Apply(func) \
func(uint8_t) \
func(uint16_t) \
func(uint32_t) \
func(uint64_t) \
func(int8_t) \
func(int16_t) \
func(int32_t) \
func(int64_t) \
func(bool) \
func(float) \
func(double) \
func(std::complex<float>) \
func(std::complex<double>)

Apply(WrapConstruct)
Apply(WrapConstructPmt)
Apply(Equals)
Apply(EqualsPmt)

#undef WrapConstruct
#undef WrapConstantPmt
#undef Equals
#undef EqualsPmt
#undef Apply


// These structures allow us to write template functions that depend on the
// flatbuffer data type.  This allows us to do things like verify that the
// datatype is correct when we want to interpret a pmt as another type.
template <> struct cpp_type<Data::ScalarInt8> { using type=int8_t; };
template <> struct cpp_type<Data::ScalarInt16> { using type=int16_t; };
template <> struct cpp_type<Data::ScalarInt32> { using type=int32_t; };
template <> struct cpp_type<Data::ScalarInt64> { using type=int64_t; };
template <> struct cpp_type<Data::ScalarUInt8> { using type=uint8_t; };
template <> struct cpp_type<Data::ScalarUInt16> { using type=uint16_t; };
template <> struct cpp_type<Data::ScalarUInt32> { using type=uint32_t; };
template <> struct cpp_type<Data::ScalarUInt64> { using type=uint64_t; };
template <> struct cpp_type<Data::ScalarFloat32> { using type=float; };
template <> struct cpp_type<Data::ScalarFloat64> { using type=double; };
template <> struct cpp_type<Data::ScalarComplex64> { using type=std::complex<float>; };
template <> struct cpp_type<Data::ScalarComplex128> { using type=std::complex<double>; };
template <> struct cpp_type<Data::ScalarBool> { using type=bool; };


} // namespace pmtf
