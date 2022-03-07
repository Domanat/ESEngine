#include "gtest/gtest.h"
#include <iostream>
#include "../../source/Utils.hpp"

TEST(Utils, UtilsTest)
{
	EXPECT_EQ("C:\\Development\\ESEngine\\VsSolution\\x64\\Release", Utils::GetWorkingDirectory());
}

