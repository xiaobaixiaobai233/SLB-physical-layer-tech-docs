# SLB 物理层 6.5 通用功能 C 库
# 依据 T/XS 10001-2025 第 6.5 节

CC ?= gcc
CFLAGS ?= -std=c99 -Wall -Wextra -O2
LDFLAGS ?= -lm

SRCS = slb_phy_common.c \
       slb_phy_pwr_meas.c \
       slb_phy_mod.c \
       slb_phy_scram.c \
       slb_phy_scfdma.c \
       slb_phy_zc.c

OBJS = $(SRCS:.c=.o)

.PHONY: all clean test

all: libslb_phy.a

libslb_phy.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

slb_phy_test: slb_phy_test.c libslb_phy.a
	$(CC) $(CFLAGS) -o $@ slb_phy_test.c -L. -lslb_phy $(LDFLAGS)

test: slb_phy_test
	./slb_phy_test

%.o: %.c slb_phy.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) libslb_phy.a slb_phy_test slb_phy_test.exe
