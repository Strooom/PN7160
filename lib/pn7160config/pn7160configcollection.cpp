#include <pn7160configcollection.hpp>

// payload bytes for PN7160 configs

constexpr uint8_t getConfigA002Data[0]{};
const pn7160Config configA002{0xA002, 0x00, getConfigA002Data};

constexpr uint8_t getConfigA003Data[0]{};
const pn7160Config configA003{0xA003, 0x00, getConfigA003Data};

constexpr uint8_t getConfigA004Data[0]{};
const pn7160Config configA004{0xA004, 0x00, getConfigA004Data};

constexpr uint8_t getConfigA005Data[0]{};
const pn7160Config configA005{0xA005, 0x00, getConfigA005Data};

constexpr uint8_t getConfigA006Data[0]{};
const pn7160Config configA006{0xA006, 0x00, getConfigA006Data};

constexpr uint8_t getConfigA007Data[0]{};
const pn7160Config configA007{0xA007, 0x00, getConfigA007Data};

constexpr uint8_t getConfigA009Data[0]{};
const pn7160Config configA009{0xA009, 0x00, getConfigA009Data};

constexpr uint8_t getConfigA00EData[0]{};
const pn7160Config configA00E{0xA00E, 0x00, getConfigA00EData};

constexpr uint8_t getConfigA0A7Data[0]{};
const pn7160Config configA0A7{0xA0A7, 0x00, getConfigA0A7Data};

constexpr uint8_t getConfigA016Data[0]{};
const pn7160Config configA016{0xA016, 0x00, getConfigA016Data};

constexpr uint8_t getConfigA00FData[0]{};
const pn7160Config configA00F{0xA00F, 0x00, getConfigA00FData};

constexpr uint8_t getConfigA014Data[0]{};
const pn7160Config configA014{0xA014, 0x00, getConfigA014Data};

constexpr uint8_t getConfigA020Data[0]{};
const pn7160Config configA020{0xA020, 0x00, getConfigA020Data};

constexpr uint8_t getConfigA026Data[0]{};
const pn7160Config configA026{0xA026, 0x00, getConfigA026Data};


uint32_t pn7160ConfigCollection::activeConfig{0};
const pn7160Config pn7160ConfigCollection::configs[pn7160ConfigCollection::nmbrOfConfigs] = {
    configA002, configA003, configA004, configA005, configA006, configA007, configA009, configA00E, configA0A7, configA016, configA00F, configA014, configA020, configA026
    };
