#
# Makefile for security subsystem
#

# Base directory for the compiler
XTENSA_TOOLS_ROOT ?= $(ESP_HOME)/xtensa-lx106-elf/bin

# name for the target project
TARGET     = libsecurity
BUILD_BASE = build

# which modules (subdirectories) of the project to include in compiling

MODULES = libraries/cryptoauthlib
MODULES += libraries/cryptoauthlib/basic
MODULES += libraries/cryptoauthlib/hal
MODULES += libraries/cryptoauthlib/host
MODULES += libraries/cryptoauthlib/crypto
MODULES += libraries/cryptoauthlib/crypto/hashes
MODULES += libraries/i2c_driver


EXTRA_INCDIR =  $(SMING_HOME)
EXTRA_INCDIR += ${SMING_HOME}/security/libraries/cryptoauthlib
EXTRA_INCDIR += ${SMING_HOME}/security/libraries/cryptoauthlib/basic
EXTRA_INCDIR += ${SMING_HOME}/security/libraries/cryptoauthlib/hal
EXTRA_INCDIR += ${SMING_HOME}/security/libraries/cryptoauthlib/host
EXTRA_INCDIR += ${SMING_HOME}/security/libraries/cryptoauthlib/crypto
EXTRA_INCDIR += ${SMING_HOME}/security/libraries/i2c_driver

# compiler flags using during compilation of source files
#CFLAGS = -g -save-temps -Os -Wpointer-arith -Wundef -Werror -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals -mno-serialize-volatile -D__ets__ -DICACHE_FLASH
CFLAGS = -Os -g -Wpointer-arith -Wundef -Werror -Wl,-EL -nostdlib -mlongcalls -mtext-section-literals -finline-functions -fdata-sections -ffunction-sections -D__ets__ -DICACHE_FLASH -DARDUINO=106 -DCOM_SPEED_SERIAL=$(COM_SPEED_SERIAL) $(USER_CFLAGS)
CFLAGS += -DNDEBUG=1
CFLAGS += -DLWIP_DEBUG=0
CFLAGS += -DuECC_OPTIMIZATION_LEVEL=0 -DuECC_PLATFORM=uECC_arch_other
CFLAGS += -D__SIZEOF_INT128__=0 -Ddefault_RNG_defined=0
CFLAGS += -Dasm_clear=0 -Dasm_set=0 -Dasm_rshift1=0 -Dasm_add=0 -Dasm_sub=0 -Dasm_mult=0
# CXXFLAGS = $(CFLAGS) -std=c++11 -fno-rtti -fno-exceptions
CXXFLAGS = $(CFLAGS) -fno-rtti -fno-exceptions -std=c++11 -felide-constructors -Wno-literal-suffix
# linker flags used to generate the main object file
LDFLAGS = -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static -Wl,-Map=$(BUILD_BASE)/firmware.map

# linker script used for the above linkier step
LD_SCRIPT = eagle.app.v6.hr.ld
# various paths from the SDK used in this project
SDK_LIBDIR = lib
SDK_LDDIR  = ld
SDK_INCDIR = extra_include 
SDK_INCDIR += include 
SDK_INCDIR += include/espressif 
SDK_INCDIR += include/json 
#SDK_INCDIR += include/udhcp 
SDK_INCDIR += include/lwip 
SDK_INCDIR += include/lwip/lwip 
SDK_INCDIR += include/lwip/ipv4 
SDK_INCDIR += include/lwip/ipv6 
SDK_INCDIR += include/freertos

# select which tools to use as compiler, librarian and linker
CC  := $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-gcc
CXX := $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-g++
AR  := $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-ar
LD  := $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-gcc
OBJCOPY := $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-objcopy
OBJDUMP := $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-objdump

SRC_DIR   := $(MODULES)
BUILD_DIR := $(addprefix $(BUILD_BASE)/,$(MODULES))

USER_LIBDIR := $(SMING_HOME)/compiler/lib
SDK_LIBDIR  := $(addprefix $(SDK_BASE)/,$(SDK_LIBDIR))
SDK_INCDIR  := $(addprefix -I$(SDK_BASE)/,$(SDK_INCDIR))

SRC     := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c*))
C_OBJ   := $(patsubst %.c,%.o,$(SRC))
CXX_OBJ := $(patsubst %.cpp,%.o,$(C_OBJ))
OBJ     := $(patsubst %.o,$(BUILD_BASE)/%.o,$(CXX_OBJ))
LIBS    := $(addprefix -l,$(LIBS))
APP_AR  := $(addprefix $(BUILD_BASE)/,$(TARGET).a)

LD_SCRIPT := $(addprefix -Tcompiler/ld/,$(LD_SCRIPT))

INCDIR := $(addprefix -I,$(SRC_DIR))
EXTRA_INCDIR := $(addprefix -I,$(EXTRA_INCDIR))
MODULE_INCDIR := $(addsuffix /include,$(INCDIR))

V ?= $(VERBOSE)
ifeq ("$(V)","1")
Q :=
vecho := @true
else
Q := @
vecho := @echo
endif

vecho := @echo


vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)

define compile-objects
$1/%.o: %.c
	$(vecho) "CC $$<" 
	$(Q) $(CC) $(INCDIR) $(MODULE_INCDIR) $(EXTRA_INCDIR) $(SDK_INCDIR) $(CFLAGS)  -c $$< -o $$@
$1/%.o: %.cpp
	$(vecho) "C+ $$<"
	$(vecho) $(Q) $(CXX) $(EXTRA_INCDIR) $(SDK_INCDIR) $(CXXFLAGS)  -c $$< -o $$@
	$(Q) $(CXX) $(EXTRA_INCDIR) $(SDK_INCDIR) $(CXXFLAGS)  -c $$< -o $$@
endef

.PHONY: all checkdirs clean 

all: checkdirs $(APP_AR)

$(APP_AR): $(OBJ)
	$(vecho) "AR $@"
	$(Q) $(AR) cru $@ $^
	$(vecho) "Installing $(TARGET)"
	$(Q) cp -r $(APP_AR) $(USER_LIBDIR)
	$(vecho) "Done"
	
checkdirs: $(BUILD_DIR) $(FW_BASE)

$(BUILD_DIR):
	$(Q) mkdir -p $@
	
rebuild: clean all

clean:
	$(Q) rm -f $(USER_LIBDIR)/$(TARGET).a
	$(Q) rm -rf $(BUILD_BASE)

$(foreach bdir,$(BUILD_DIR),$(eval $(call compile-objects,$(bdir))))
