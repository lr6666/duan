CC := gcc
CFLAGS := -Wall -O3
Libs = -lsqlite3 -L./lib -lmystr
Target := people
Source := $(wildcard src/*.c)
Objs := $(patsubst %.c,%.o,$(Source))
Modules += display_link find insert_data is_null_link menu main delete sort_link re_wr_db
AllObjs := $(addsuffix /src/*.o,$(Modules))
