// #############################################################################
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include <unity.h>
#include <nci.hpp>

void setUp(void) {
}
void tearDown(void) {
}

void test_initialize() {
    TEST_ASSERT_EQUAL(2, nci::pn7160configuration.nmbrOfConfigs);
    TEST_ASSERT_EQUAL(0, nci::pn7160configuration.activeConfig);

    TEST_ASSERT_EQUAL_UINT16(0x0000, nci::pn7160configuration.configs[0].tag);
    TEST_ASSERT_EQUAL_UINT16(0xA00E, nci::pn7160configuration.configs[1].tag);

    TEST_ASSERT_EQUAL_UINT8(0x02, nci::pn7160configuration.configs[0].length);
    TEST_ASSERT_EQUAL_UINT8(0x0B, nci::pn7160configuration.configs[1].length);

    uint8_t expectedData0[2] = {0xF4, 0x01};
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData0, nci::pn7160configuration.configs[0].data, 2);
    uint8_t expectedData1[11] = {0x11, 0x01, 0xC1, 0xB1, 0x00, 0xDA, 0x1E, 0x14, 0x00, 0xD0, 0x0C};
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData1, nci::pn7160configuration.configs[1].data, 11);
}

void test_get_config() {
    // nci::checkConfigStart();
    // uint8_t expectedMessage[] = {0x20, 0x03, 0x02, 0x01, 0x00}; // CORE_GET_CONFIG_CMD for TAG 0 = TOTAL_DURATION
    // TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedMessage, nci::txBuffer, 5);

    // pn7160configcollection::activeConfig++;
    // nci::requestConfigNext();
    // uint8_t expectedMessage2[] = {0x20, 0x03, 0x03, 0x01, 0x0E, 0xA0}; 
    // TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedMessage2, nci::txBuffer, 6);
}

void test_set_config() {
    pn7160configcollection::activeConfig = 0;
    nci::sendSetConfig();
    uint8_t expectedMessage[] = {0x20, 0x02, 0x05, 0x01, 0x00, 0x02, 0xF4, 0x01}; // CORE_SET_CONFIG_CMD for TAG 0 = TOTAL_DURATION = 500
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedMessage, nci::txBuffer, 7);

    pn7160configcollection::activeConfig++;
    nci::sendSetConfig();
    uint8_t expectedMessage2[] = {0x20, 0x02, 0x0F, 0x01, 0xA0, 0x0E, 0x0B, 0x11, 0x01, 0xC1, 0xB1, 0x00, 0xDA, 0x1E, 0x14, 0x00, 0xD0, 0x0C}; // CORE_SET_CONFIG_CMD for TAG 0xA00E = Power Management Config PMU_CFG
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedMessage2, nci::txBuffer, 17);
}


int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_get_config);
    RUN_TEST(test_set_config);
    UNITY_END();
}
