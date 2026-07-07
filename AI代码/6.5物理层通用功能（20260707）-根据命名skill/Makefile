# NearLink 物理层通用功能 C 库
# 依据 T/XS 10001-2025 第 6.5 节

CC ?= gcc
CFLAGS ?= -std=c99 -Wall -Wextra -O2
LDFLAGS ?= -lm

SRCS = nl_phy_common.c \
       nl_phy_pwr_meas.c \
       nl_phy_mod.c \
       nl_phy_scram.c \
       nl_phy_scfdma.c \
       nl_phy_zc.c

OBJS = $(SRCS:.c=.o)

.PHONY: all clean test

all: libnl_phy.a

libnl_phy.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

nl_phy_test: nl_phy_test.c libnl_phy.a
	$(CC) $(CFLAGS) -o $@ nl_phy_test.c -L. -lnl_phy $(LDFLAGS)

test: nl_phy_test
	./nl_phy_test

%.o: %.c nl_phy.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) libnl_phy.a nl_phy_test nl_phy_test.exe libslb_phy.a slb_phy_test slb_phy_test.exe
