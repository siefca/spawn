## Process this file with automake to produce Makefile.in

@SET_MAKE@

bin_PROGRAMS = spawn

spawn_COMMON_SRC = spawn.c \
		   spawn.h \
		   invokes.c \
		   invokes.h \
		   variables.c \
		   variables.h \
		   defaults.h \
		   signals.c \
		   signals.h \
		   localestuff.h \
		   initlocale.h

spawn_SNPRINTF = ../shared/snprintf.c
spawn_SIGBLOCK = ../shared/sigblock.c ../shared/sigblock.h

if HAVE_SNPRINTF
spawn_SNPRINTF_OBJ =
else
spawn_SNPRINTF_OBJ = ../shared/snprintf.o
endif

if HAVE_SIGBLOCK
spawn_SIGBLOCK_OBJ =
else 
spawn_SIGBLOCK_OBJ = ../shared/sigblock.o
endif 

spawn_SOURCES = $(spawn_COMMON_SRC)

spawn_LDADD = $(spawn_SNPRINTF_OBJ) $(spawn_SIGBLOCK_OBJ) $(INTLLIBS)

spawn_DEPENDENCIES = $(spawn_SNPRINTF_OBJ)\
                     $(spawn_SIGBLOCK_OBJ)\
		     $(INTLDEPS)


if HAVE_SNPRINTF
else
../shared/snprintf.o: $(spawn_SNPRINTF)
endif

if HAVE_SIGBLOCK
else 
../shared/sigblock.o: $(spawn_SIGBLOCK)
endif 

EXTRA_DIST = $(spawn_SNPRINTF) $(spawn_SIGBLOCK)
