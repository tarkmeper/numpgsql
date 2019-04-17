MODULE_big = numpgsql

EXTENSION = numpgsql
EXTVERSION   = $(shell grep default_version $(EXTENSION).control | sed -e "s/default_version[[:space:]]*=[[:space:]]*'\([^']*\)'/\1/")

DATA = $(EXTENSION)--$(EXTVERSION).sql

PGFILEDESC = "numpgsql - Scientific Libraries in PG/SQL"
OBJS = $(patsubst %.cpp, %.o, $(wildcard src/*.cpp)) $(patsubst %.cpp, %.o, $(wildcard src/aggregate/*.cpp)) src/magic.o

REGRESS  = $(patsubst sql/%.sql, %, $(wildcard sql/*.sql))
REGRESS_OPTS  = --load-extension=numpgsql


PG_CPPFLAGS = -O3 -ftree-vectorize -ffast-math  -mavx
PG_LIBS = -lm -lstdc++
SHLIB_LINK = -lm -lstdc++
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
