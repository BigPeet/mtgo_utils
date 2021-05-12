#include <filesystem>
#include "gtest/gtest.h"

#include "mtgo_utils/gamelog.h"

class GameLogPathTest : public ::testing::Test
{
  protected:
    void SetUp() override {}

    void TearDown() override {}
};


TEST_F(GameLogPathTest, IsMatchGameLog_Valid) // NOLINT
{
    std::filesystem::path const valid_01{"Match_GameLog_1234.dat"};
    std::filesystem::path const valid_02{"/some/path/Match_GameLog_1234.dat"};

    ASSERT_TRUE(mtgo_utils::IsMatchGameLog(valid_01));
    ASSERT_TRUE(mtgo_utils::IsMatchGameLog(valid_02));
}


TEST_F(GameLogPathTest, IsMatchGameLog_Invalid) // NOLINT
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
