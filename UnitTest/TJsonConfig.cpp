#include "JsonConfig.h"

#include <gtest/gtest.h>

TEST(JsonConfig,basis)
{
    JsonConfig jsonConfig("../../config/config.json");
    ASSERT_EQ(cg::RET_SUCESS, jsonConfig.Init());

    int nMaxSSD = jsonConfig.GetInt("Tracker.MiniPatchMaxSSD");
    std::cout << "nMaxSSD: " << nMaxSSD << std::endl;
}
