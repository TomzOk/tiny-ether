#ulib makefile

# setup install directory
PREFIX		?=	target
MKDIR_P		:= 	mkdir -p
CC		:=	gcc

# configurations
CONFIGS_D 	+= 	URLP_CONFIG_LINUX_EMU
CONFIGS_D 	+= 	URLPX_CONFIG_LINUX_EMU

# Get lib objects
MODULES 	+=	liburlp
MODULES 	+=	libucrypto/mbedtls/uaes
MODULES 	+=	libucrypto/mbedtls/uecc
MODULES 	+=	libucrypto/mbedtls/uhash
MODULES 	+=	libucrypto/secp256k1/uecc
MODULES 	+=	libucrypto/secp256k1/uhash

# Build test applications
APPLICATIONS 	+=	liburlp/test
APPLICATIONS 	+=	libucrypto/test
APPLICATIONS 	+=	libup2p/test

# Build vars
DIRS 		+=	$(addprefix $(PREFIX)/lib/,$(MODULES))
DIRS 		+=	$(addprefix $(PREFIX)/obj/,$(MODULES))
DIRS 		+=	$(addprefix $(PREFIX)/obj/,$(APPLICATIONS))
LIBS 		+= 	$(addsuffix .a,$(foreach mod, $(MODULES),$(subst /,-,$(mod))))
SRCS 		+=	$(shell find $(MODULES) -name '*.c')
OBJS		+=	$(addprefix $(PREFIX)/obj/,$(SRCS:.c=.o))
INCS		+=	$(addprefix -I./, $(MODULES))
INCS		+=	$(addsuffix /include,$(addprefix -I./, $(MODULES)))
INCS	 	+=	$(addprefix -I./,$(PREFIX)/include)
DEFS 		+= 	$(addprefix -D,$(CONFIGS_D))

CFLAGS 		+= 	$(DEFS)


all: $(DIRS) $(OBJS)

.PHONY: dirs libs clean test print

$(OBJS): $(SRCS)
	@echo "  CC $@"
	@${CC} -c ${CFLAGS} ${LDFLAGS} $(INCS) $< -o $@ 

$(DIRS):
	@echo "MKDR $@"
	@${MKDIR_P} ${DIRS}

# Makefile debug print
print:
	@echo \
		DIRS: $(DIRS) \
		LIBS: $(LIBS) \
		SRCS: $(SRCS) \
		INCS: $(INCS) \
		OBJS: $(OBJS) \
		DEFS: $(DEFS) \
		| sed -e 's/\s\+/\n/g' 
