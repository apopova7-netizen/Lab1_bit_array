#include <gtest/gtest.h>
#include "BitArray.h"

TEST(BitArrayConstructors, DefaultConstructor) {

    BitArray a;
    EXPECT_EQ(a.size(), 0);
    EXPECT_TRUE(a.empty());
}

TEST(BitArrayConstructors, ConstructorWithParametrs) {

    BitArray a(7, 19);
    EXPECT_EQ(a.size(), 7);
    EXPECT_FALSE(a.empty());
    EXPECT_EQ(a.to_string(), "0010011");

    BitArray b(13, 1);
    EXPECT_EQ(b.size(), 13);
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(b.to_string(), "0000000000001");

    BitArray default_val(5);
    EXPECT_EQ(default_val.size(),5);
    EXPECT_FALSE(default_val.empty());
    EXPECT_EQ(default_val.to_string(), "00000");

    BitArray empty(0);
    EXPECT_EQ(empty.size(),0);
    EXPECT_TRUE(empty.empty());
    EXPECT_EQ(empty.to_string(), "");

    BitArray big(70, 19);
    EXPECT_EQ(big.size(), 70);

    std::string expected_str1(65, '0');
    expected_str1 += "10011";
    EXPECT_EQ(big.to_string(), expected_str1);

    EXPECT_THROW(BitArray(-1), std::length_error);
}


TEST(BitArrayConstructors, CopyConstructor) {

    BitArray original1(14, 28);
    BitArray copy1(original1);

    EXPECT_EQ(copy1.size(), original1.size());
    EXPECT_EQ(copy1.to_string(), original1.to_string());

    BitArray original2(78, 11142);
    BitArray copy2(original2);

    EXPECT_EQ(copy2.size(), original2.size());
    EXPECT_EQ(copy2.to_string(), original2.to_string());

    BitArray original_empty;
    BitArray copy_empty(original_empty);

    EXPECT_EQ(copy_empty.size(), 0);
    EXPECT_TRUE(copy_empty.empty());
    EXPECT_EQ(copy_empty.to_string(), "");
}


TEST(BitArrayConstructors, AssignmentOperator) {

    BitArray a(7, 19);
    BitArray b;

    b = a;
    EXPECT_EQ(b.to_string(), "0010011");

    b = b;
    EXPECT_EQ(b.to_string(), "0010011");

    BitArray empty1;
    BitArray empty2;
    empty1 = empty2;
    EXPECT_TRUE(empty1.empty());

    empty1 = empty1;
    EXPECT_TRUE(empty1.empty());

}


TEST(BitArrayModification, SetAndResetOneBit) {

    BitArray a(5, 0);

    a.set(0, true);
    a.set(4, true);
    EXPECT_EQ(a.to_string(), "10001");

    a.reset(0);
    EXPECT_EQ(a.to_string(), "10000");

    BitArray b(5, 31); // 11111
    b.reset(2);
    EXPECT_EQ(b.to_string(), "11011");
}


TEST(BitArrayModification, FullSetAndReset) {

    BitArray a(5, 0);

    a.set();
    EXPECT_EQ(a.to_string(), "11111");

    a.reset();
    EXPECT_EQ(a.to_string(), "00000");
}


TEST(BitArrayLogicOperators, CompoundAssignment) {
    BitArray a(4, 10); // 1010
    BitArray b(4, 12); // 1100

    BitArray and_test = a;
    and_test &= b;
    EXPECT_EQ(and_test.to_string(), "1000");


    BitArray or_test = a;
    or_test |= b;
    EXPECT_EQ(or_test.to_string(), "1110");


    BitArray xor_test = a;
    xor_test ^= b;
    EXPECT_EQ(xor_test.to_string(), "0110");

    int big_size = 130;
    BitArray big_a(big_size, 0);
    BitArray big_b(big_size, 0);
    big_a.set();
    big_b.set(0);
    big_b.set(3);
    big_b.set(129);

    big_a &= big_b;
    std::string expected_big = "1" +  std::string(125, '0') + "1001";
    EXPECT_EQ(big_a.to_string(), expected_big);
}

TEST(BitArrayLogicOperators, BitwiseOperations) {
    BitArray a(4, 10); // 1010
    BitArray b(4, 12); // 1100

    EXPECT_EQ((a & b).to_string(), "1000");
    EXPECT_EQ((a | b).to_string(), "1110");
    EXPECT_EQ((a ^ b).to_string(), "0110");
    EXPECT_EQ((~a).to_string(),    "0101");
}


TEST(BitArrayLogicOperators, ErrorDifferentSizes) {
    BitArray a(4, 10);
    BitArray b(5, 10);

    EXPECT_THROW(a &= b, std::length_error);
    EXPECT_THROW(a |= b, std::length_error);
    EXPECT_THROW(a ^= b, std::length_error);
}


TEST(BitArrayShifts, Shifts) {

    BitArray a(6, 5); // 000101

    a <<= 2;
    EXPECT_EQ(a.to_string(), "010100");
    a >>= 1;
    EXPECT_EQ(a.to_string(), "001010");

    BitArray big(200, 19); // 10011

    big <<= 150;
    std::string expected_str = std::string(45, '0') + "10011" + std::string(150, '0');
    EXPECT_EQ(big.to_string(), expected_str);

    big >>= 128;
    expected_str = std::string(173, '0') + "10011" + std::string(22, '0');
    EXPECT_EQ(big.to_string(), expected_str);
}


TEST(BitArrayShifts, BoundaryShifts) {

    BitArray a(5, 31); // 11111
    EXPECT_EQ((a << 5).to_string(), "00000");
    EXPECT_EQ((a >> 10).to_string(), "00000");
    EXPECT_EQ((a << 0).to_string(), "11111");
}


TEST(BitArrayShifts, NegativeShiftException) {
    BitArray a(5, 1);
    EXPECT_THROW(a <<= -1, std::invalid_argument);
    EXPECT_THROW(a >>= -5, std::invalid_argument);
}



TEST(BitArrayResizing, ResizeMethod) {
    BitArray a(3, 5); // 101

    a.resize(6, true);
    EXPECT_EQ(a.size(), 6);
    EXPECT_EQ(a.to_string(), "111101");

    a.resize(2);
    EXPECT_EQ(a.size(), 2);
    EXPECT_EQ(a.to_string(), "01");

    a.resize(2);
    EXPECT_EQ(a.size(), 2);
    EXPECT_EQ(a.to_string(), "01");

    a.resize(0);
    EXPECT_EQ(a.size(), 0);
    EXPECT_EQ(a.to_string(), "");

    a.resize(128,1);
    EXPECT_EQ(a.size(), 128);
    EXPECT_EQ(a.to_string(), std::string(128, '1'));

    a.resize(6);
    EXPECT_EQ(a.size(), 6);
    EXPECT_EQ(a.to_string(), "111111");

    EXPECT_THROW(a.resize(-5), std::length_error);
}


TEST(BitArrayResizing, PushBackMethod) {
    BitArray a;
    a.push_back(true);
    a.push_back(false);
    a.push_back(true);

    EXPECT_EQ(a.size(), 3);
    EXPECT_EQ(a.to_string(), "101");

    for (int i = 0; i < 70; i++)
        a.push_back(true);

    EXPECT_EQ(a.size(), 73);
    EXPECT_EQ(a.to_string(), std::string(70, '1') + "101");
}


TEST(BitArrayResizing, Clear) {
    BitArray a(3, 7); // 111
    BitArray b(2, 0); // 00
    BitArray c;

    a.clear();
    b.clear();
    c.clear();
    EXPECT_EQ(a.size(), 0);
    EXPECT_EQ(b.size(), 0);
    EXPECT_EQ(c.size(), 0);
    EXPECT_TRUE(a.empty());
    EXPECT_TRUE(b.empty());
    EXPECT_TRUE(c.empty());
}

TEST(BitArrayResizing, Swap) {
    BitArray a(3, 7); // 111
    BitArray b(2, 0); // 00

    a.swap(b);
    EXPECT_EQ(a.size(), 2);
    EXPECT_EQ(b.size(), 3);
    EXPECT_EQ(a.to_string(), "00");
    EXPECT_EQ(b.to_string(), "111");

}


TEST(BitArrayInfoAndCompare, ContentCheck) {

    BitArray a(4, 0);
    EXPECT_TRUE(a.none());
    EXPECT_FALSE(a.any());
    EXPECT_EQ(a.count(), 0);


    a.set(1, true);
    EXPECT_FALSE(a.none());
    EXPECT_TRUE(a.any());
    EXPECT_EQ(a.count(), 1);


    BitArray full(BitArray::BITS_PER_BLOCK, 0);
    full.set();
    EXPECT_FALSE(full.none());
    EXPECT_TRUE(full.any());
    EXPECT_EQ(full.count(), BitArray::BITS_PER_BLOCK);


    int big_size = BitArray::BITS_PER_BLOCK + 11;
    BitArray big(big_size, 0);

    EXPECT_TRUE(big.none());
    EXPECT_FALSE(big.any());
    EXPECT_EQ(big.count(), 0);

    big.set(big_size - 1, true);
    big.set(big_size - 3, true);
    EXPECT_FALSE(big.none());
    EXPECT_TRUE(big.any());
    EXPECT_EQ(big.count(), 2);

    big.set(big_size - 1, false);
    big.set(0, true);
    EXPECT_FALSE(big.none());
    EXPECT_TRUE(big.any());
    EXPECT_EQ(big.count(), 2);
}


TEST(BitArrayInfAndCompare,  IndexingOperator) {
    BitArray a(4, 9); //1001

    EXPECT_TRUE(a[0]);
    EXPECT_FALSE(a[1]);
    EXPECT_FALSE(a[2]);
    EXPECT_TRUE(a[3]);
}


TEST(BitArrayInfAndCompare, EqualityOperators) {
    BitArray a(4, 9);  // 1001
    BitArray b(4, 9);  // 1001
    BitArray c(4, 10); // 1010
    BitArray d(5, 9);  // 01001

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
    EXPECT_FALSE(a == d);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
