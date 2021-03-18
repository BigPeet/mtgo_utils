#include <filesystem>
#include "gtest/gtest.h"

#include "mtgo_utils/gamelog.h"

class GameLogTest : public ::testing::Test
{
  public:
    constexpr static char const* m_sentinel{"@P"};

  protected:
    void SetUp() override {}

    void TearDown() override {}
};


TEST_F(GameLogTest, IsMatchGameLog_Valid) // NOLINT
{
    std::filesystem::path const valid_01{"Match_GameLog_1234.dat"};
    std::filesystem::path const valid_02{"/some/path/Match_GameLog_1234.dat"};

    ASSERT_TRUE(mtgo_utils::IsMatchGameLog(valid_01));
    ASSERT_TRUE(mtgo_utils::IsMatchGameLog(valid_02));
}


TEST_F(GameLogTest, IsMatchGameLog_Invalid) // NOLINT
{
    std::filesystem::path const invalid_01{"Match_GameLog_1234.txt"};
    std::filesystem::path const invalid_02{"Match_GameLog_1234.log"};
    std::filesystem::path const invalid_03{"MyGamelog.dat"};
    std::filesystem::path const invalid_04{"Match_GameChat_1234.dat"};

    ASSERT_FALSE(mtgo_utils::IsMatchGameLog(invalid_01));
    ASSERT_FALSE(mtgo_utils::IsMatchGameLog(invalid_02));
    ASSERT_FALSE(mtgo_utils::IsMatchGameLog(invalid_03));
    ASSERT_FALSE(mtgo_utils::IsMatchGameLog(invalid_04));
}


TEST_F(GameLogTest, ParseGameLogFileStr_Empty_01) // NOLINT
{
    std::vector<std::string> expected{};
    ASSERT_EQ(mtgo_utils::ParseGameLogFile(std::string{}), expected);
}


TEST_F(GameLogTest, ParseGameLogFileStr_Empty_02) // NOLINT
{
    std::string content{"Here is some nonsense.\nNo sentinel characters included."};
    std::vector<std::string> expected{};
    ASSERT_EQ(mtgo_utils::ParseGameLogFile(content), expected);
}


TEST_F(GameLogTest, ParseGameLogFileStr_Sentinel_01) // NOLINT
{
    std::string s1{"Here is some text with just a"};
    std::string s2{" single sentinel character."};
    std::string content{s1 + m_sentinel + s2};
    std::vector<std::string> expected{s2};
    ASSERT_EQ(mtgo_utils::ParseGameLogFile(content), expected);
}


TEST_F(GameLogTest, ParseGameLogFileStr_Sentinel_02) // NOLINT
{
    std::string s1{"Here is some text with "};
    std::string s2{"multiple sentinel characters."};
    std::string s3{"Another sentence."};
    std::string content{s1 + m_sentinel + s2 + m_sentinel + s3};
    std::vector<std::string> expected{s2, s3};
    ASSERT_EQ(mtgo_utils::ParseGameLogFile(content), expected);
}


TEST_F(GameLogTest, ParseGameLogFileStr_Sentinel_03) // NOLINT
{
    std::string s1{"Here is some text with "};
    std::string s2{"multiple sentinel characters in a row."};
    std::string s3{"Another sentence."};
    std::string content{s1 + m_sentinel + s2 + m_sentinel + m_sentinel + s3};
    std::vector<std::string> expected{s2, s3};
    ASSERT_EQ(mtgo_utils::ParseGameLogFile(content), expected);
}


TEST_F(GameLogTest, ParseGameLogFileStr_CutOff_01) // NOLINT
{
    std::string s1{"Here is some text with "};
    std::string s2{"multiple sentinel characters."};
    std::string s3{"Another sentence."};
    std::string non_ascii{"ſþÖÄ"};
    std::string content{s1 + m_sentinel + s2 + non_ascii + m_sentinel + s3};
    std::vector<std::string> expected{s2, s3};
    ASSERT_EQ(mtgo_utils::ParseGameLogFile(content), expected);
}


TEST_F(GameLogTest, ParseGameLogFileStr_CutOff_02) // NOLINT
{
    std::string s1{"Here is some text with "};
    std::string s2{"a single sentinel character but an additional sentence. Another sentence."};
    std::string content{s1 + m_sentinel + s2};
    std::vector<std::string> expected{s2};
    ASSERT_EQ(mtgo_utils::ParseGameLogFile(content), expected);
}


TEST_F(GameLogTest, ParseGameLogFileStr_CutOff_03) // NOLINT
{
    std::string s1{"Here is some text with "};
    std::string s2{"a single sentinel character and a linebreak."};
    std::string s3{"Another sentence which should not be in the result."};
    std::string content{s1 + m_sentinel + s2 + '\n' + s3};
    std::vector<std::string> expected{s2};
    ASSERT_EQ(mtgo_utils::ParseGameLogFile(content), expected);
}


TEST_F(GameLogTest, ParseGameLogFileStr_CutOff_04) // NOLINT
{
    std::string prefix1{"Player1 rolled a 2."};
    std::string prefix2{"Player2 rolled a 4."};
    std::string t1{"Turn 1: Player1"};
    std::string t2{"Turn 1: Player2"};
    std::string ignored{"asdaAsd123"};
    std::string content{m_sentinel + prefix1 + m_sentinel + prefix2 + m_sentinel + t1 + m_sentinel +
                        t2 + ignored};
    std::vector<std::string> expected{prefix1, prefix2, t1, t2};
    ASSERT_EQ(mtgo_utils::ParseGameLogFile(content), expected);
}


TEST_F(GameLogTest, ParseGameLogFileStr_CutOff_05) // NOLINT
{
    std::string s1{"Here is some text with "};
    std::string s2{"a single sentinel character (and some reminder text example...)."};
    std::string content{s1 + m_sentinel + s2};
    std::vector<std::string> expected{s2};
    ASSERT_EQ(mtgo_utils::ParseGameLogFile(content), expected);
}
