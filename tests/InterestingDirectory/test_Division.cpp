#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../src/InterestingDirectory/Division.h"


TEST(Test_Division, division_standard_2p4) {
    int a = 2;
    int b = 4;
    double c = Division(a,b);
    EXPECT_EQ(c, 0.5);
}

TEST(Test_Division, division_par_zer0) {
    EXPECT_EQ(Division(1,0), 0); 
}
