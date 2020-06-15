#include "gtest/gtest.h"
#include "TTBParser.hxx"

#include <string>

#include <boost/random.hpp>

std::string random_str_with_delim(int size, const char delimiter=char(0))
{
    boost::random::mt19937 rng;
    boost::random::uniform_int_distribution<> char_rnd(32, 127);
    boost::random::uniform_int_distribution<> add_delim(0, 100);

    std::string _out;

    for(int i{0}; i < size; ++i)
    {
        _out += char_rnd(rng);
        if(add_delim(rng) > 75)
        {
            _out += delimiter;
        }
    }

    return _out;
}

TEST(TestTTBParser, TestFileNotFound)
{
    TTBParser::Parser* parser = new TTBParser::Parser;
    ASSERT_FALSE(parser->ParseServices("test.ttb"));
}

TEST(TestTTBParser, TestSplitByNullChar)
{
    TTBParser::_parser_impl* _impl = new TTBParser::_parser_impl;
    const std::string _test_str = random_str_with_delim(100);
    const int n_delim = std::count(_test_str.begin(), _test_str.end(), char(0));

    EXPECT_EQ(n_delim, (_impl->_split(_test_str)).size());
}

TEST(TestTTBParser, TestOpenRealFile)
{
    TTBParser::Parser* parser = new TTBParser::Parser;
    EXPECT_TRUE(parser->ParseServices("/Users/krizar/Documents/RailwayOpSimRepos/FR-ParisMetro/Program_Timetables/RATP_MetroMoFri.ttb"));
}

TEST(TestTTBParser, TestStringToTime)
{
    std::string test_str = "10:00";
    TTBParser::_parser_impl* _impl = new TTBParser::_parser_impl;
    std::string _result = boost::posix_time::to_simple_string(_impl->_get_time(test_str));
    EXPECT_TRUE(_result.find(test_str) != std::string::npos);
}