MODULE_big = numpgsql

EXTENSION = numpgsql
EXTVERSION   = $(shell grep default_version $(EXTENSION).control | sed -e "s/default_version[[:space:]]*=[[:space:]]*'\([^']*\)'/\1/")

DATA = $(EXTENSION)--$(EXTVERSION).sql

PGFILEDESC = "numpgsql - Scientific Libraries in PG/SQL"
OBJS = $(patsubst %.cpp, %.o, $(wildcard src/*.cpp)) $(patsubst %.cpp, %.o, $(wildcard src/aggregate/*.cpp))

REGRESS  = $(patsubst sql/%.sql, %, $(wildcard sql/*.sql))
REGRESS_OPTS  = --load-extension=numpgsql


PG_CPPFLAGS = -O3 -ftree-vectorize -ffast-math  -mavx -fPIC
PG_LIBS = -lm -lstdc++ -lgcov
SHLIB_LINK = -lm -lstdc++ -lgcov

ifdef ccov
	PG_CPPFLAGS += -fprofile-arcs -ftest-coverage
	PG_LIBS += --coverage
	SHLIB_LINK += --coverage
endif

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)


