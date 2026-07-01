#include <gtest/gtest.h>

#include "../include/sequence/other/IGroupIRing.hpp"

TEST(IGroupIRingTests, DefaultRingAddWorkInt) {
    DefaultRing<int> ring;

    EXPECT_EQ(ring.Add(1, 2), 3);
}

TEST(IGroupIRingTests, DefaultRingZeroWorkInt) {
    DefaultRing<int> ring;

    EXPECT_EQ(ring.Zero(), 0);
}

TEST(IGroupIRingTests, DefaultRingInverseWorkInt) {
    DefaultRing<int> ring;

    EXPECT_EQ(ring.Inverse(2), -2);
}

TEST(IGroupIRingTests, DefaultRingTimesWorkInt) {
    DefaultRing<int> ring;

    EXPECT_EQ(ring.Times(3, 2), 6);
}

TEST(IGroupIRingTests, DefaultRingOneWorkInt) {
    DefaultRing<int> ring;

    EXPECT_EQ(ring.One(), 1);
}

TEST(IGroupIRingTests, DefaultRingAddWorkDouble) {
    DefaultRing<double> ring;

    EXPECT_EQ(ring.Add(1.0, 2.0), 3.0);
}

TEST(IGroupIRingTests, DefaultRingZeroWorkDouble) {
    DefaultRing<double> ring;

    EXPECT_EQ(ring.Zero(), 0);
}

TEST(IGroupIRingTests, DefaultRingInverseWorkDouble) {
    DefaultRing<double> ring;

    EXPECT_EQ(ring.Inverse(2.0), -2.0);
}

TEST(IGroupIRingTests, DefaultRingTimesWorkDouble) {
    DefaultRing<double> ring;

    EXPECT_EQ(ring.Times(3.0, 2.0), 6.0);
}

TEST(IGroupIRingTests, DefaultRingOneWorkDouble) {
    DefaultRing<double> ring;

    EXPECT_EQ(ring.One(), 1.0);
}