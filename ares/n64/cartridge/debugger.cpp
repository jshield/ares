auto Cartridge::Debugger::load(Node::Object parent) -> void {
  memory.rom = parent->append<Node::Debugger::Memory>("Cartridge ROM");
  memory.rom->setSize(cartridge.rom.size);
  memory.rom->setRead([&](u32 address) -> u8 {
    return cartridge.rom.read<Byte>(address);
  });
  memory.rom->setWrite([&](u32 address, u8 data) -> void {
    return cartridge.rom.write<Byte>(address, data);
  });

  if(cartridge.ram) {
    memory.ram = parent->append<Node::Debugger::Memory>("Cartridge SRAM");
    memory.ram->setSize(cartridge.ram.size);
    memory.ram->setRead([&](u32 address) -> u8 {
      return cartridge.ram.read<Byte>(address);
    });
    memory.ram->setWrite([&](u32 address, u8 data) -> void {
      return cartridge.ram.write<Byte>(address, data);
    });
  }

  if(cartridge.eeprom) {
    memory.eeprom = parent->append<Node::Debugger::Memory>("Cartridge EEPROM");
    memory.eeprom->setSize(cartridge.eeprom.size);
    memory.eeprom->setRead([&](u32 address) -> u8 {
      return cartridge.eeprom.read<Byte>(address);
    });
    memory.eeprom->setWrite([&](u32 address, u8 data) -> void {
      return cartridge.eeprom.write<Byte>(address, data);
    });
  }

  if(cartridge.flash) {
    memory.flash = parent->append<Node::Debugger::Memory>("Cartridge Flash");
    memory.flash->setSize(cartridge.flash.size);
    memory.flash->setRead([&](u32 address) -> u8 {
      return cartridge.flash.read<Byte>(address);
    });
    memory.flash->setWrite([&](u32 address, u8 data) -> void {
      return cartridge.flash.write<Byte>(address, data);
    });
  }

  if(cartridge.isviewer.ram) {
    memory.isviewer = parent->append<Node::Debugger::Memory>("ISViewer64");
    memory.isviewer->setSize(cartridge.isviewer.ram.size);
    memory.isviewer->setRead([&](u32 address) -> u8 {
        return cartridge.isviewer.ram.read<Byte>(address);
    });
    memory.isviewer->setWrite([&](u32 address, u8 data) -> void {
      return cartridge.isviewer.ram.write<Byte>(address, data);
    });
    cartridge.isviewer.tracer.log = parent->append<Node::Debugger::Tracer::Notification>("Log", "IS Viewer-64");
  }
}

auto Cartridge::Debugger::unload(Node::Object parent) -> void {
  parent->remove(memory.rom);
  parent->remove(memory.ram);
  parent->remove(memory.eeprom);
  parent->remove(memory.flash);
  parent->remove(cartridge.isviewer.tracer.log);
  memory.rom.reset();
  memory.ram.reset();
  memory.eeprom.reset();
  memory.flash.reset();
  cartridge.isviewer.tracer.log.reset();
}
