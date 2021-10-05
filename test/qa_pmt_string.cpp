#include <gtest/gtest.h>

#include <pmtf/pmtf.hpp>
#include <pmtf/pmtf_string.hpp>

#include <iostream>

using namespace pmtf;

TEST(PmtString, Basic)
{
    {
    auto str_pmt = pmt_string("hello");
    EXPECT_EQ(str_pmt, "hello");
    }
    {
    pmt_string str_pmt = "goodbye";
    EXPECT_EQ(str_pmt, "goodbye");
    }
}


TEST(PmtString, Assignment)
{
    auto str_pmt = pmt_string("hello");
    
    str_pmt = "goodbye";

    EXPECT_EQ(str_pmt, "goodbye");
}


TEST(PmtString, Serdes)
{
    auto str_pmt = pmt_string("hello");
    
    std::stringbuf sb; // fake channel
    sb.str("");        // reset channel to empty
    bool ret = str_pmt.ptr()->serialize(sb);
    std::cout << ret << std::endl;
    auto base_ptr = pmt_base::deserialize(sb);

    
    EXPECT_EQ(get_pmt_string(base_ptr), "hello");
}

TEST(Pmt, PmtWrap)
{
    pmt_wrap x;
    x = std::string("hello");
    
    EXPECT_EQ(get_pmt_string(x), "hello");
}
