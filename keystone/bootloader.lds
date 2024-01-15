OUTPUT_ARCH( "riscv" )

ENTRY( _entry )

SECTIONS
{
  . = 0x10000; /* boot loader lives in boot ROM after the device tree */
  PROVIDE( reset_vector = . );
  .text :
  {
    PROVIDE( _entry = . );
    *(.reset)
    *(*)
  }
  . = ALIGN(4);
   .rodata : ALIGN(0x100) {
    _RODATA_START_ = .;
        *(.rodata)
        *(.dtb*)
        *(.rodata*)
    _RODATA_END_ = .;
    }
}

INCLUDE ./keystone/sanctum_params.lds
