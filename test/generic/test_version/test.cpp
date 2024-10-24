#define unitTest
#include <cstring>
#include <unity.h>
#include "version.hpp"
#include "buildinfo.hpp"

void setUp(void) {        // before each test
}
void tearDown(void) {        // after each test
}

void test_initialize() {
    // Before initialization values...
    TEST_ASSERT_EQUAL_UINT8(0, version::isVersionMajor);
    TEST_ASSERT_EQUAL_UINT8(0, version::isVersionMinor);
    TEST_ASSERT_EQUAL_UINT8(0, version::isVersionPatch);
    TEST_ASSERT_EQUAL(buildType::unknown, version::theBuildType);
    TEST_ASSERT_EQUAL(buildEnvironment::unknown, version::theBuildEnvironment);
    TEST_ASSERT_EQUAL_STRING("", version::isVersionString);

    version::initialize();
    // After initialization values...
    TEST_ASSERT_EQUAL_UINT8(buildInfo::mainVersionDigit, version::isVersionMajor);
    TEST_ASSERT_EQUAL_UINT8(buildInfo::minorVersionDigit, version::isVersionMinor);
    TEST_ASSERT_EQUAL_UINT8(buildInfo::patchVersionDigit, version::isVersionPatch);
    TEST_ASSERT_EQUAL(buildInfo::theBuildType, version::theBuildType);
    TEST_ASSERT_EQUAL(buildInfo::theBuildEnvironment, version::theBuildEnvironment);
    TEST_ASSERT_GREATER_THAN(0, strlen(version::getIsVersionAsString()));
}

void test_toString() {
    // For test coverage only
    TEST_ASSERT_EQUAL_STRING("unknown", toString(buildType::unknown));
    TEST_ASSERT_EQUAL_STRING("development", toString(buildType::development));
    TEST_ASSERT_EQUAL_STRING("production", toString(buildType::production));
    TEST_ASSERT_EQUAL_STRING("integrationTest", toString(buildType::integrationTest));

    TEST_ASSERT_EQUAL_STRING("unknown", toString(buildEnvironment::unknown));
    TEST_ASSERT_EQUAL_STRING("local", toString(buildEnvironment::local));
    TEST_ASSERT_EQUAL_STRING("ci", toString(buildEnvironment::ci));
}

int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_toString);
    UNITY_END();
}