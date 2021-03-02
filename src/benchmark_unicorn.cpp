#include <iostream>
#include <fstream>
#include <unicorn/unicorn.h>
#include "utils.hpp"


constexpr std::uint32_t RETURN_TO =   0x00000100;
constexpr std::uint32_t MEMORY_SIZE = 5 * 1024 * 1024;

int main() {
  ElfReader elf("tester_arm");
  std::uint32_t ENTRY_POINT = elf.getSymbolAddress("_Z10findPrimesj");

  uc_engine *uc;
  uc_err err;
 
  err = uc_open(UC_ARCH_ARM, UC_MODE_ARM, &uc);
  if (err != UC_ERR_OK) {
    printf("Failed on uc_open() with error returned: %u\n", err);
    return -1;
  }
 
  uc_mem_map(uc, 0, MEMORY_SIZE, UC_PROT_ALL);
  elf.load( [uc](auto data, auto size, auto vaddr) {
      uc_mem_write(uc, vaddr, data, size);
    });
 
  // initialize machine registers
  uint32_t sp = MEMORY_SIZE - 128;
  uint32_t pc = ENTRY_POINT;
  uint32_t lr = RETURN_TO;
  uint32_t r0 = 100000;
  uc_reg_write(uc, UC_ARM_REG_SP, &sp);
  uc_reg_write(uc, UC_ARM_REG_PC, &pc);
  uc_reg_write(uc, UC_ARM_REG_LR, &lr);
  uc_reg_write(uc, UC_ARM_REG_R0, &r0);
 
  // emulate code in infinite time & unlimited instructions
  err=uc_emu_start(uc, ENTRY_POINT, RETURN_TO, 0, 0);
  if (err) {
    printf("Failed on uc_emu_start() with error returned %u: %s\n",
      err, uc_strerror(err));
  }
 
  // now print out some registers
  uc_reg_read(uc, UC_ARM_REG_R0, &r0);
  std::cout << "CPU state : R0=" << r0 << std::endl;
 
  uc_close(uc);
 
  return 0;
}