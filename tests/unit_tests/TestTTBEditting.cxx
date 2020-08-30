#include "gtest/gtest.h"
#include "TTBParser.hxx"
#include "Structure.hxx"
#include "Editting.hxx"
#include "Utilities.hxx"

#ifndef ROOT_DIR
#error Root Directory Must be Set!
#endif

#include <string>

#include <boost/random.hpp>

TEST(TestTTBEditting, TestInsertEntry)
{
    TTBParser::TTBBuilder* _builder;
    TTBParser::Entry entry_a, entry_b;
    entry_a.headcode = "1V02";
    entry_b.headcode = "2V01"; 
    _builder->insertEntry(entry_a);
    _builder->insertEntry(entry_b, 0);
}

TEST(TestTTBEditting, TestNewTimetable)
{
}

TEST(TestTTBEditting, TestCloneEntry)
{
}
