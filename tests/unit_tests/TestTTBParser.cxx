#include "gtest/gtest.h"
#include "TTBParser.hxx"
#include "Structure.hxx"
#include "Utilities.hxx"

#ifndef ROOT_DIR
#error Root Directory Must be Set!
#endif

#include <string>

#include <boost/random.hpp>

/**
 * @brief Generate random string with a delimiter
 * 
 * @param size length of string
 * @param delimiter character to separate by
 * @return std::string 
 */
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
    ASSERT_FALSE(parser->ParseEntries("test.ttb"));
}

TEST(TestTTBParser, TestSplitByNullChar)
{
    TTBParser::_parser_impl* _impl = new TTBParser::_parser_impl;
    const std::string _test_str = random_str_with_delim(100);
    const int n_delim = std::count(_test_str.begin(), _test_str.end(), char(0));

    EXPECT_EQ(n_delim+1, (_impl->_split(_test_str)).size());
}

TEST(TestTTBParser, TestParseCoordinate)
{
    std::string test_str = "N31-22";
    const TTBParser::Coordinate test_coord = {-31, 22};
    TTBParser::_parser_impl* _impl = new TTBParser::_parser_impl;

    EXPECT_EQ(_impl->_parse_coordinate(test_str), test_coord);
}

TEST(TestTTBParser, TestOpenFile1)
{
    TTBParser::Parser* parser = new TTBParser::Parser;
    EXPECT_TRUE(parser->ParseEntries(std::string(ROOT_DIR)+"/tests/RATP_MetroMoFri.ttb"));
    EXPECT_EQ(parser->getEntries()["1L01"].parent->headcode, "1L00");
    EXPECT_EQ(parser->getEntries()["1L00"].entry.first.X, 174);
    EXPECT_EQ(parser->getEntries()["1L00"].entry.first.Y, -6);
    EXPECT_EQ(parser->getEntries()["1L00"].duration_events.begin()->second.label, "Berault");
    EXPECT_EQ(parser->getEntries()["1L00"].single_events.begin()->second.label, "Chateau de Vincennes");
}

TEST(TestTTBParser, TestOpenFile2)
{
    TTBParser::Parser* parser = new TTBParser::Parser;
    EXPECT_TRUE(parser->ParseEntries(std::string(ROOT_DIR)+"/tests/Birmingham 0700 Start.ttb"));
}

TEST(TestTTBParser, TestOutputEqInput)
{
    TTBParser::Parser* parser = new TTBParser::Parser;
    const std::string file = std::string(ROOT_DIR)+"/tests/RATP_MetroMoFri.ttb";
    std::ifstream file_str(file);
    parser->ParseEntries(file);
    std::string _data_str;
    getline(file_str, _data_str);
    file_str.close();
    TTBParser::Timetable timetable = parser->getTimetableObject("Test1");
    EXPECT_EQ(timetable._send_to_string(), _data_str);
}


