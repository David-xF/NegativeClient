OUTPUT_ARCH(powerpc)
ENTRY(entry)

MEMORY {
  rom(rxai) : ORIGIN = 0x01800000, LENGTH = 16m
}

SECTIONS {
  .text :
  {
    *(.entry)
    *(.text.*)
    *(.gcc_except_table)
    *(.gcc_except_table.*)
    *(.sdata)
    *(.sdata.*)
    *(.rodata)
    *(.rodata.*)
    *(.bss)
    *(.bss.*)
    *(.sbss)
    *(.sbss.*)
    *(.data)
    *(.data.*)
    *(.ext)
    *(.ext.*)
    data_end = .;
  } > rom
}