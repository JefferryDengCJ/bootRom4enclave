XLEN ?= 64
CROSSCOMPILE ?= riscv64-unknown-elf-
CC = ${CROSSCOMPILE}gcc

ifeq ($(XLEN), 64)
#CFLAGS = -O2 -ggdb -march=rv64gc -mabi=lp64 -Wall -mcmodel=medany -mexplicit-relocs -fno-builtin -fno-common -std=gnu11
CFLAGS = -O2 -march=rv64gc -mabi=lp64 -Wall -mcmodel=medany -mexplicit-relocs -fno-builtin -fno-common -std=gnu11
else
CFLAGS = -O2 -ggdb  -march=rv32im -mabi=ilp32 -W -Wall -mcmodel=medany -mexplicit-relocs -fno-builtin
endif

#CCASFLAGS = -mcmodel=medany -mexplicit-relocs -fPIC
LDFLAGS = -nostdlib -nodefaultlibs -nostartfiles
LDFLAGS = -nostartfiles

INCLUDES = -I./ -I./src -I./keystone

SRCS_C = src/uart.c src/spi.c src/sd.c src/gpt.c src/gpio.c src/testDDR.c \
	 keystone/ed25519/fe.c \
	 keystone/ed25519/ge.c \
	 keystone/ed25519/keypair.c \
	 keystone/ed25519/sc.c \
	 keystone/ed25519/sign.c \
	 keystone/ed25519/verify.c \
	 keystone/sha3/sha3.c \
	 keystone/bootloader.c \
	 keystone/string.c \
	 keystone/timer.c 

SRCS_ASM = keystone/bootloader_x.S
OBJS_C = $(SRCS_C:.c=.o)
OBJS_S = $(SRCS_ASM:.S=.o)

MAIN = bootrom_$(XLEN).elf
MAIN_BIN = $(MAIN:.elf=.bin)
MAIN_IMG = $(MAIN:.elf=.img)
MAIN_SV = $(MAIN:.elf=.sv)

DTB = cv$(XLEN)a6.dtb

#.PHONY: clean

$(MAIN): $(DTB) $(OBJS_C) $(OBJS_S) keystone/bootloader.lds
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) -T keystone/bootloader.lds $(OBJS_S) $(OBJS_C) -o $(MAIN)
	@echo "LD    >= $(MAIN)"

%.img: %.bin
	dd if=$< of=$@ bs=128

%.bin: %.elf
	$(CROSSCOMPILE)objcopy -O binary $< $@
	$(CROSSCOMPILE)objdump -D bootrom_$(XLEN).elf > bootrom_$(XLEN).dump
	$(CROSSCOMPILE)elf2hex --bit-width 64 --input bootrom_$(XLEN).elf --output bootrom_$(XLEN).hex
	./do_fpga.sh

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c  $<  -o $@
	@echo "CC    <= $<"

%.o: %.S
	@$(CC) $(CFLAGS) $(CCASFLAGS) $(INCLUDES) -c $<  -o $@
	@echo "CC    <= $<"

%.dtb: %.dts
	dtc -I dts $< -O dtb -o $@

%.sv: %.img
	./gen_rom.py $<
	@echo "PYTHON >= $(MAIN_SV)"

clean:
	$(RM) $(OBJS_C) $(OBJS_S) $(MAIN) $(MAIN_BIN) $(MAIN_IMG) *.dtb

all: $(MAIN) $(MAIN_BIN) $(MAIN_IMG) $(MAIN_SV)
	@echo "zero stage bootloader has been compiled!"

# DO NOT DELETE THIS LINE -- make depend needs it
