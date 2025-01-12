Then PN7160 may need additional configuration, according the application datasheet (UM11495) chapter 13.
As each config rewrites the EEPROM, and this EEPROM has a limited number of writecycles, the SW reads the existing config, compares is with the TO BE values, and only writes it in case of a mismatch.

See 7.4.7 : The 'Tags' are stored as 16 bit, as this is needed for the extended range of the PN7160. 
    For the standard NFC range (Table 138 of the NCI specification), we use eg 0x0000 for TOTAL_DURATION
    For the extended range we use 0xA00E for PMU_CFG