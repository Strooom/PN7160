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
}

void test_getIdByte() {
    tag testTag;
    static constexpr uint8_t testTagLength{4};
    static constexpr uint8_t testTagData[testTagLength]{0x01, 0x02, 0x03, 0x04};
    testTag.setUniqueId(testTagLength, testTagData);

    TEST_ASSERT_EQUAL_UINT8(testTagLength, testTag.getUniqueIdLength());
    TEST_ASSERT_EQUAL_UINT8_ARRAY(testTagData, testTag.getUniqueId(), testTagLength);

    testTag.setUniqueId(1, testTagData);
    TEST_ASSERT_EQUAL_UINT8(0, testTag.getUniqueIdLength());
}

void test_isEqual() {
    tag tag1;
    tag tag2;
    TEST_ASSERT_TRUE(tag1 == tag2);
    static constexpr uint8_t testTagLength{4};
    static constexpr uint8_t testTagData[testTagLength]{0x01, 0x02, 0x03, 0x04};
    tag1.setUniqueId(testTagLength, testTagData);
    tag2.setUniqueId(testTagLength, testTagData);
    TEST_ASSERT_TRUE(tag1 == tag2);
    tag2.setUniqueId(1, testTagData);
    TEST_ASSERT_FALSE(tag1 == tag2);
    static constexpr uint8_t testTagData2[testTagLength]{0x01, 0x02, 0x03, 0xFF};
    tag2.setUniqueId(testTagLength, testTagData2);
    TEST_ASSERT_FALSE(tag1 == tag2);
}

void test_dummy() {
    tag testTag;
    testTag.dump();
    static constexpr uint8_t testTagLength{4};
    static constexpr uint8_t testTagData[testTagLength]{0x01, 0x02, 0x03, 0x04};
    testTag.setUniqueId(testTagLength, testTagData);
    testTag.dump();
    TEST_IGNORE_MESSAGE("For Coverage Only");
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_getIdByte);
    RUN_TEST(test_isEqual);
    RUN_TEST(test_dummy);
    UNITY_END();
}
