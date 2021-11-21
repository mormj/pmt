#include "scalar_value.hpp"

namespace pmtf
{

#define IMPLEMENT_PMT_SCALAR(datatype, fbtype)                      \
    template <>                                                     \
    datatype scalar_value<datatype>::value()                          \
    {                                                               \
        auto pmt = GetSizePrefixedPmt(_fbb.GetBufferPointer());     \
        return pmt->data_as_Scalar##fbtype()->value();              \
    }                                                               \
                                                                    \
    template <>                                                     \
    const datatype scalar_value<datatype>::value() const              \
    {                                                               \
        auto pmt = GetSizePrefixedPmt(_fbb.GetBufferPointer());     \
        return pmt->data_as_Scalar##fbtype()->value();              \
    }                                                               \
                                                                    \
    template <>                                                     \
    flatbuffers::Offset<void> scalar_value<datatype>::rebuild_data(   \
        flatbuffers::FlatBufferBuilder& fbb)                        \
    {                                                               \
        Scalar##fbtype##Builder sb(fbb);                            \
        auto val = value();                                         \
        sb.add_value(val);                                          \
        return sb.Finish().Union();                                 \
    }                                                               \
                                                                    \
    template <>                                                     \
    void scalar_value<datatype>::set_value(const datatype& val)       \
    {                                                               \
        Scalar##fbtype##Builder sb(_fbb);                           \
        sb.add_value(val);                                          \
        _data = sb.Finish().Union();                                \
        build();                                                    \
    }                                                               \
                                                                    \
    template <>                                                     \
    scalar_value<datatype>::scalar_value(const datatype& val)           \
        : base(Data::Scalar##fbtype)                            \
    {                                                               \
        set_value(val);                                             \
    }                                                               \
                                                                    \
    template <>                                                     \
    scalar_value<datatype>::scalar_value(const uint8_t* buf)            \
        : base(Data::Scalar##fbtype)                            \
    {                                                               \
        auto data = GetPmt(buf)->data_as_Scalar##fbtype()->value(); \
        set_value(data);                                            \
    }                                                               \
                                                                    \
    template <>                                                     \
    scalar_value<datatype>::scalar_value(const pmtf::Pmt* fb_pmt)       \
        : base(Data::Scalar##fbtype)                            \
    {                                                               \
        auto data = fb_pmt->data_as_Scalar##fbtype()->value();      \
        set_value(data);                                            \
    }                                                               \
                                                                    \
    template class scalar_value<datatype>;


#define IMPLEMENT_PMT_SCALAR_CPLX(datatype, fbtype)                    \
    template <>                                                        \
    datatype scalar_value<datatype>::value()                             \
    {                                                                  \
        auto pmt = GetSizePrefixedPmt(_fbb.GetBufferPointer());        \
        return *((datatype*)(pmt->data_as_Scalar##fbtype()->value())); \
    }                                                                  \
                                                                       \
    template <>                                                        \
    const datatype scalar_value<datatype>::value() const                 \
    {                                                                  \
        auto pmt = GetSizePrefixedPmt(_fbb.GetBufferPointer());        \
        return *((datatype*)(pmt->data_as_Scalar##fbtype()->value())); \
    }                                                                  \
                                                                       \
    template <>                                                        \
    flatbuffers::Offset<void> scalar_value<datatype>::rebuild_data(      \
        flatbuffers::FlatBufferBuilder& fbb)                           \
    {                                                                  \
        Scalar##fbtype##Builder sb(fbb);                               \
        auto val = value();                                            \
        sb.add_value((fbtype*)&val);                                   \
        return sb.Finish().Union();                                    \
    }                                                                  \
                                                                       \
    template <>                                                        \
    void scalar_value<datatype>::set_value(const datatype& val)          \
    {                                                                  \
        Scalar##fbtype##Builder sb(_fbb);                              \
        sb.add_value((fbtype*)&val);                                   \
        _data = sb.Finish().Union();                                   \
        build();                                                       \
    }                                                                  \
                                                                       \
    template <>                                                        \
    scalar_value<datatype>::scalar_value(const datatype& val)              \
        : base(Data::Scalar##fbtype)                               \
    {                                                                  \
        set_value(val);                                                \
    }                                                                  \
                                                                       \
    template <>                                                        \
    scalar_value<datatype>::scalar_value(const uint8_t* buf)               \
        : base(Data::Scalar##fbtype)                               \
    {                                                                  \
        auto data = GetPmt(buf)->data_as_Scalar##fbtype()->value();    \
        set_value(*((const datatype*)data));                           \
    }                                                                  \
                                                                       \
    template <>                                                        \
    scalar_value<datatype>::scalar_value(const pmtf::Pmt* fb_pmt)          \
        : base(Data::Scalar##fbtype)                               \
    {                                                                  \
        auto data = fb_pmt->data_as_Scalar##fbtype()->value();         \
        set_value(*((const datatype*)data));                           \
    }                                                                  \
                                                                       \
                                                                       \
    template class scalar_value<datatype>;

IMPLEMENT_PMT_SCALAR(int8_t, Int8)
IMPLEMENT_PMT_SCALAR(uint8_t, UInt8)
IMPLEMENT_PMT_SCALAR(int16_t, Int16)
IMPLEMENT_PMT_SCALAR(uint16_t, UInt16)
IMPLEMENT_PMT_SCALAR(int32_t, Int32)
IMPLEMENT_PMT_SCALAR(uint32_t, UInt32)
IMPLEMENT_PMT_SCALAR(int64_t, Int64)
IMPLEMENT_PMT_SCALAR(uint64_t, UInt64)
IMPLEMENT_PMT_SCALAR(bool, Bool)
IMPLEMENT_PMT_SCALAR(float, Float32)
IMPLEMENT_PMT_SCALAR(double, Float64)

IMPLEMENT_PMT_SCALAR_CPLX(std::complex<float>, Complex64)
IMPLEMENT_PMT_SCALAR_CPLX(std::complex<double>, Complex128)

} // namespace pmtf
