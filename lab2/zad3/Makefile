CC = gcc
CFLAGS = -Wall -O3 -fPIC

LIB_STATIC = libsignals.a
LIB_SHARED = libsignals.so

OBJS = sig_default.o sig_ignore.o sig_handle.o sig_mask.o

.PHONY: all clean static shared dynamic

# ===== DOMYŚLNIE =====
all: static shared

# ==== BIBLIOTEKA STATYCZNA ===== 
static: $(LIB_STATIC) main_static child_static

$(LIB_STATIC): $(OBJS)
	ar rcs $(LIB_STATIC) $(OBJS)

main_static: main.c
	$(CC) $(CFLAGS) main.c -o main_static

child_static: child.c $(LIB_STATIC)
	$(CC) $(CFLAGS) child.c -L. -lsignals -o child
# =================================

# ======== BIBLIOTEKA WSPÓŁDZIELONA =========
shared: $(LIB_SHARED) main_shared child_shared

$(LIB_SHARED): $(OBJS)
	$(CC) -shared -o $(LIB_SHARED) $(OBJS)

main_shared: main.c
	$(CC) $(CFLAGS) main.c -o main_shared

child_shared: child.c $(LIB_SHARED)
	$(CC) $(CFLAGS) child.c -L. -lsignals -o child_shared
# =========================

# ====== DYNAMIC LOADING =======  
dynamic: $(LIB_SHARED) main_dynamic child_dynamic

main_dynamic: main.c
	$(CC) $(CFLAGS) main.c -o main_dynamic

child_dynamic: child.c
	$(CC) $(CFLAGS) -DDYNAMIC child.c -ldl -o child_dynamic

# =========================


%.o: %.c signals.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.a *.so main_* child_*