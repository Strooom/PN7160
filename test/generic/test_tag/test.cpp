// #############################################################################
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################
#include <unity.h>
#include <tag.hpp>

void setUp(void) {        // before each test
}
void tearDown(void) {        // after each test
}

void test_initialize() {
    tag testTag;
    TEST_ASSERT_EQUAL(0, testTag.uniqueIdLength);
    TEST_ASSERT_EQUAL(0, testTag.getUniqueIdLength());
    uint8_t allZeroes[tag::maxUniqueIdLength]{};
    TEST_ASSERT_EQUAL_UINT8_ARRAY(allZeroes, testTag.uniqueId, tag::maxUniqueIdLength);
    TEST_ASSERT_EQUAL(0, testTag.detectionTimestamp);
}

void test_getIdByte() {
    tag testTag;
    testTag.uniqueIdLength = 4;
    testTag.uniqueId[0]    = 0x01;
    testTag.uniqueId[1]    = 0x02;
    testTag.uniqueId[2]    = 0x03;
    testTag.uniqueId[3]    = 0x04;
    TEST_ASSERT_EQUAL_UINT8(0x01, testTag[0]);
    TEST_ASSERT_EQUAL_UINT8(0x02, testTag[1]);
    TEST_ASSERT_EQUAL_UINT8(0x03, testTag[2]);
    TEST_ASSERT_EQUAL_UINT8(0x04, testTag[3]);

    testTag.uniqueId[4]    = 0x05;
    TEST_ASSERT_EQUAL_UINT8(tag::defaultIdByte, testTag[4]);
}

void test_getCompareID() {
    tag tag1;
    tag tag2;
    TEST_ASSERT_TRUE(tag1 == tag2);

    tag1.uniqueIdLength = 4;
    tag1.uniqueId[0] = 0x01;
    tag1.uniqueId[1] = 0x02;
    tag1.uniqueId[2] = 0x03;
    tag1.uniqueId[3] = 0x04;

    tag2.uniqueIdLength = 4;
    tag2.uniqueId[0] = 0x01;
    tag2.uniqueId[1] = 0x02;
    tag2.uniqueId[2] = 0x03;
    tag2.uniqueId[3] = 0x04;

    TEST_ASSERT_TRUE(tag1 == tag2);

    tag2.uniqueId[3] = 0x05;
    TEST_ASSERT_FALSE(tag1 == tag2);

    tag2.uniqueId[3] = 0x04;
    tag2.uniqueIdLength = 7;
    TEST_ASSERT_FALSE(tag1 == tag2);
}

void test_dummy() {
    tag testTag;
    testTag.dump();
    testTag.uniqueIdLength = 4;
    testTag.uniqueId[0]    = 0x01;
    testTag.uniqueId[1]    = 0x02;
    testTag.uniqueId[2]    = 0x03;
    testTag.uniqueId[3]    = 0x04;
    testTag.dump();

    TEST_IGNORE_MESSAGE("For Coverage Only");
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_getIdByte);
    RUN_TEST(test_getCompareID);
    RUN_TEST(test_dummy);
    UNITY_END();
}
