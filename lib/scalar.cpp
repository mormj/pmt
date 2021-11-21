/*-*-c++-*-*/
/*
 * Copyright 2021 John Sallay
 *
 * SPDX-License-Identifier: LGPL-3.0
 *
 */
#include <pmtf/scalar.hpp>
#include "scalar_value.hpp"

namespace pmtf {

template <class T, Data dt> 
scalar<T> _get_pmt_scalar(const wrap& x) { 
    if constexpr(std::is_same_v<typename cpp_type<dt>::type, T>) 
        return scalar<T>(std::dynamic_pointer_cast<scalar_value<T>>(x.ptr())); 
    else 
        throw std::runtime_error("Cannot convert scalar types");
}

template <class T>
scalar<T> get_scalar(const wrap& x) {
    // Make sure that this is the right type.
    switch(auto dt = x.ptr()->data_type()) {
        case Data::ScalarFloat32: return _get_pmt_scalar<T, Data::ScalarFloat32>(x);
        case Data::ScalarFloat64: return _get_pmt_scalar<T, Data::ScalarFloat64>(x);
        case Data::ScalarComplex64: return _get_pmt_scalar<T, Data::ScalarComplex64>(x);
        case Data::ScalarComplex128: return _get_pmt_scalar<T, Data::ScalarComplex128>(x);
        case Data::ScalarInt8: return _get_pmt_scalar<T, Data::ScalarInt8>(x);
        case Data::ScalarInt16: return _get_pmt_scalar<T, Data::ScalarInt16>(x);
        case Data::ScalarInt32: return _get_pmt_scalar<T, Data::ScalarInt32>(x);
        case Data::ScalarInt64: return _get_pmt_scalar<T, Data::ScalarInt64>(x);
        case Data::ScalarUInt8: return _get_pmt_scalar<T, Data::ScalarUInt8>(x);
        case Data::ScalarUInt16: return _get_pmt_scalar<T, Data::ScalarUInt16>(x);
        case Data::ScalarUInt32: return _get_pmt_scalar<T, Data::ScalarUInt32>(x);
        case Data::ScalarUInt64: return _get_pmt_scalar<T, Data::ScalarUInt64>(x);
        case Data::ScalarBool: return _get_pmt_scalar<T, Data::ScalarBool>(x);
        default:
            throw std::runtime_error("Cannot convert non scalar pmt.");
    }
}


#define WrapGettersImpl(T,dt) \
template scalar<T> _get_pmt_scalar<T,dt>(const wrap& x); \
template <> \
T get_scalar_value(const wrap& x) { \
    return get_scalar<T>(x).ptr()->value();\
} 

#define WrapConstructImpl(type, dt) \
    template <> wrap::wrap<type>(const type& x) { d_ptr = scalar(x).ptr(); }
// Construct a wrap from a scalar
#define WrapConstructPmtImpl(type, dt) \
    template <> wrap::wrap<scalar<type>>(const scalar<type>& x) { d_ptr = x.ptr(); }

#define EqualsImpl(type, dt) \
    template <> bool operator==<type>(const wrap& x, const type& other) { \
       if (can_be<type>(x)) {                                           \
            auto value = get_scalar<type>(x);                       \
            return x == other;                                          \
        } else                                                          \
            return false;                                               \
    }

#define EqualsPmtImpl(type, dt) \
    template <> bool operator==<scalar<type>>(const wrap& x, const scalar<type>& other) { \
        return x == other.value();  \
    }

#define Apply(func) \
func(uint8_t, Data::ScalarUInt8) \
func(uint16_t, Data::ScalarUInt16) \
func(uint32_t, Data::ScalarUInt32) \
func(uint64_t, Data::ScalarUInt64) \
func(int8_t, Data::ScalarInt8) \
func(int16_t, Data::ScalarInt16) \
func(int32_t, Data::ScalarInt32) \
func(int64_t, Data::ScalarInt64) \
func(bool, Data::ScalarBool) \
func(float, Data::ScalarFloat32) \
func(double, Data::ScalarFloat64) \
func(std::complex<float>, Data::ScalarComplex64) \
func(std::complex<double>, Data::ScalarComplex128)

Apply(WrapGettersImpl)
Apply(WrapConstructImpl)
Apply(WrapConstructPmtImpl)
Apply(EqualsImpl)
Apply(EqualsPmtImpl)


} // namespace pmtf
