#include "gtest/gtest.h"
#include "Structure.hxx"
#include "Utilities.hxx"

TEST(TestTTBStructures, TestPrintEntry)
{
    TTBParser::Entry entry;
    entry.headcode = "1V01";
    entry.description = "London Waterloo to Wimbledon";
    entry.addSingleEvent(TTBParser::getTimeFromString("11:49"), "Waterloo");
    entry.addDurationEvent(TTBParser::getTimeFromString("11:53"),
                           TTBParser::getTimeFromString("11:54"),
                           "Vauxhall");
    entry.start_time = TTBParser::getTimeFromString("11:48");
    entry.finish_time = TTBParser::getTimeFromString("12:04");

    EXPECT_NO_FATAL_FAILURE(std::cout << entry.toString() << std::endl);
}

TEST(TestTTBStructures, TestTemperalOffset)
{
    TTBParser::Entry entry;
    entry.headcode = "1V01";
    entry.start_time = TTBParser::getTimeFromString("11:48");
    entry.finish_time = TTBParser::getTimeFromString("12:04");

    entry.TemperalOffset(60);

    EXPECT_EQ(TTBParser::ptimeToString(entry.start_time), "12:48");
    EXPECT_EQ(TTBParser::ptimeToString(entry.finish_time), "13:04");
}