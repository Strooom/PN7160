#include <pn7160configcollection.hpp>

// payload bytes for PN7160 configs

constexpr uint8_t setTotalDurationConfigData[2]{0xF4, 0x01};                                                              // TOTAL_DURATION = 500ms
constexpr uint8_t powerManagementConfigData[11]{0x11, 0x01, 0xC1, 0xB1, 0x00, 0xDA, 0x1E, 0x14, 0x00, 0xD0, 0x0C};        // PMU_CFG selects CFG1

const pn7160Config setTotalDurationConfig{0x0000, 0x02, setTotalDurationConfigData};
const pn7160Config powerManagementConfig{0xA00E, 0x0B, powerManagementConfigData};

uint32_t pn7160ConfigCollection::activeConfig{0};
const pn7160Config pn7160ConfigCollection::configs[pn7160ConfigCollection::nmbrOfConfigs] = {setTotalDurationConfig, powerManagementConfig};
