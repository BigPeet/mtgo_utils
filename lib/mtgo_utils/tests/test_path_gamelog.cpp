#include <filesystem>
#include "gtest/gtest.h"

#include "mtgo_utils/gamelog.h"

class GameLogPathTest : public ::testing::Test
{
  public:
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}

    static std::filesystem::path const& GetRootDir()
    {
        static std::filesystem::path root_dir{std::filesystem::temp_directory_path() /
                                              "test_path_gamelog"};
        return root_dir;
    }

  protected:
    void SetUp() override {}

    void TearDown() override { std::filesystem::remove_all(GetRootDir()); }
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


TEST_F(GameLogPathTest, FindMTGORootEasy) // NOLINT
{
    std::filesystem::path const target{GameLogPathTest::GetRootDir() /
                                       "Users/Dummy/AppData/Local/Apps/2.0"};
    std::filesystem::create_directories(target);

    auto res = mtgo_utils::FindMTGORoot(GameLogPathTest::GetRootDir());
    ASSERT_TRUE(res.has_value());
    ASSERT_EQ(res.value(), target);
}


TEST_F(GameLogPathTest, FindMTGORootLowercaseUsers) // NOLINT
{
    std::filesystem::path const target{GameLogPathTest::GetRootDir() /
                                       "users/Dummy/AppData/Local/Apps/2.0"};
    std::filesystem::create_directories(target);

    auto res = mtgo_utils::FindMTGORoot(GameLogPathTest::GetRootDir());
    ASSERT_TRUE(res.has_value());
    ASSERT_EQ(res.value(), target);
}


TEST_F(GameLogPathTest, FindMTGORootMultipleUsers) // NOLINT
{
    std::filesystem::path const target{GameLogPathTest::GetRootDir() /
                                       "Users/Dummy/AppData/Local/Apps/2.0"};
    std::filesystem::path const trap{GameLogPathTest::GetRootDir() / "Users/NotReal/AppData/OhNo/"};
    std::filesystem::create_directories(target);
    std::filesystem::create_directories(trap);

    auto res = mtgo_utils::FindMTGORoot(GameLogPathTest::GetRootDir());
    ASSERT_TRUE(res.has_value());
    ASSERT_EQ(res.value(), target);
}


TEST_F(GameLogPathTest, FindMTGORootNoInstall) // NOLINT
{
    std::filesystem::path const trap{GameLogPathTest::GetRootDir() / "Users/NotReal/AppData/OhNo/"};
    std::filesystem::create_directories(trap);

    auto res = mtgo_utils::FindMTGORoot(GameLogPathTest::GetRootDir());
    ASSERT_FALSE(res.has_value());
}


TEST_F(GameLogPathTest, FindMTGORootNoUsers) // NOLINT
{
    std::filesystem::path const trap{GameLogPathTest::GetRootDir() /
                                     "Dummy/AppData/Local/Apps/2.0"};
    std::filesystem::create_directories(trap);

    auto res = mtgo_utils::FindMTGORoot(GameLogPathTest::GetRootDir());
    ASSERT_FALSE(res.has_value());
}
