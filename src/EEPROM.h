#ifdef __SAMD51__

#ifndef MOCK_EEPROM_H
#define MOCK_EEPROM_H
// eventually, make this do something

class Mock_EEPROM {
 public:
  uint8_t read(uint8_t addr) { return 0xFF; }
  void write(uint8_t addr, uint8_t val) { }
  void update(uint8_t addr, uint8_t val) { }
};

//class Mock_EEPROM EEPROM;
#endif

#endif
