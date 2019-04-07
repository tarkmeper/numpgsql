MODULE_big = numpgsql

EXTENSION = numpgsql
EXTVERSION   = $(shell grep default_version $(EXTENSION).control | sed -e "s/default_version[[:space:]]*=[[:space:]]*'\([^']*\)'/\1/")

DATA = $(EXTENSION)--$(EXTVERSION).sql

PGFILEDESC = "numpgsql - Scientific Libraries in PG/SQL"
OBJS = src/magic.o src/unary.o src/binary.o src/sort.o src/agg.o

REGRESS      = $(wildcard test/*/*.sql)

PG_CPPFLAGS = -O3 -ftree-vectorize -ffast-math -ftree-vectorize  -mavx
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
