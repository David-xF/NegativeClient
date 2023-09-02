OUTPUT_ARCH(powerpc)
ENTRY(startup)

MEMORY {
  rom(rxai) : ORIGIN = 0x01800000, LENGTH = 16m
}

SECTIONS {
  .text :
  {
    *(.startup)
    *(.text.*)
    *(.gcc_except_table.*)
    *(.sdata)
    *(.sdata.*)
    *(.rodata)
    *(.rodata.*)
    *(.bss)
    *(.bss.*)
    *(.sbss)
    *(.sbss.*)
    *(.data.*)
    data_end = .;
  } > rom
}