.PHONY: results_d

MODULE_big = numpgsql

EXTENSION = numpgsql
EXTVERSION   = $(shell grep default_version $(EXTENSION).control | sed -e "s/default_version[[:space:]]*=[[:space:]]*'\([^']*\)'/\1/")

DATA = $(EXTENSION)--$(EXTVERSION).sql

PGFILEDESC = "numpgsql - Scientific Libraries in PG/SQL"
OBJS = $(patsubst %.cpp, %.o, $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp))

REGRESS  = $(patsubst sql/%.sql, %, $(wildcard sql/*.sql) $(wildcard sql/*/*.sql))
REGRESS_OPTS  = --load-extension=numpgsql


PG_CPPFLAGS = -O3 -ftree-vectorize -ffast-math  -mavx -fPIC
PG_LIBS = -lm -lstdc++
SHLIB_LINK = -lm -lstdc++

ifdef ccov
	PG_CPPFLAGS += -fprofile-arcs -ftest-coverage
	PG_LIBS += --coverage
	SHLIB_LINK += --coverage
endif


all: $(EXTENSION)--$(EXTVERSION).sql
installcheck: results_d

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)


$(EXTENSION)--$(EXTVERSION).sql: $(wildcard src/*.sql) $(wildcard src/*/*.sql)
	cat $^  > $@

results_d:
	mkdir -p results results/math results/random
