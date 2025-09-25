#include <gtest/gtest.h>

#include <VectorDB.h>

TEST(VectorDBTest, Upsert)
{
    WW::RocksVectorDB db;
    ASSERT_TRUE(db.Open("/tmp/test", 5));
    ASSERT_TRUE(db.Upsert("a", {1, 2, 3, 4, 5}));
    ASSERT_TRUE(db.Upsert("b", {4, 5, 6, 7, 8}));
}

TEST(VectorDBTest, Get)
{
    WW::RocksVectorDB db;
    ASSERT_TRUE(db.Open("/tmp/test", 5));
    ASSERT_TRUE(db.Upsert("a", {1, 2, 3, 4, 5}));
    ASSERT_TRUE(db.Upsert("b", {4, 5, 6, 7, 8}));
    ASSERT_EQ(db.Get("a"), std::vector<float>({1, 2, 3, 4, 5}));
    ASSERT_EQ(db.Get("b"), std::vector<float>({4, 5, 6, 7, 8}));
}

TEST(VectorDBTest, Search)
{
    WW::RocksVectorDB db;
    ASSERT_TRUE(db.Open("/tmp/test", 5));

    // insert some vectors
    ASSERT_TRUE(db.Upsert("a", {1, 0, 0, 0, 0}));
    ASSERT_TRUE(db.Upsert("b", {0, 1, 0, 0, 0}));
    ASSERT_TRUE(db.Upsert("c", {1, 1, 0, 0, 0}));

    // search [1, 0, 0, 0, 0]
    auto results = db.Search({1, 0, 0, 0, 0}, 2);

    ASSERT_FALSE(results.empty());
    EXPECT_EQ(results[0].id, "a");
    EXPECT_TRUE(results[1].id == "c" || results[1].id == "b");
    
    EXPECT_LE(results[0].score, results[1].score);
}
