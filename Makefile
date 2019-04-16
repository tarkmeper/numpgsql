MODULE_big = numpgsql

EXTENSION = numpgsql
EXTVERSION   = $(shell grep default_version $(EXTENSION).control | sed -e "s/default_version[[:space:]]*=[[:space:]]*'\([^']*\)'/\1/")

DATA = $(EXTENSION)--$(EXTVERSION).sql

PGFILEDESC = "numpgsql - Scientific Libraries in PG/SQL"
OBJS = $(patsubst %.cpp, %.o, $(wildcard src/*.cpp)) $(patsubst %.cpp, %.o, $(wildcard src/aggregate/*.cpp)) src/magic.o

REGRESS      = $(wildcard test/*/*.sql)

PG_CPPFLAGS = -O3 -ftree-vectorize -ffast-math  -mavx
PG_LIBS = -lm -lstdc++
SHLIB_LINK = -lm -lstdc++
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
