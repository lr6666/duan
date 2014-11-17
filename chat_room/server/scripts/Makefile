CC := gcc
Target := chat_room_server
Source := $(wildcard src/*.c)
Objs := $(patsubst %.c,%.o,$(Source))
Modules += msg_handle block_read block_write main 
AllObjs := $(addsuffix /src/*.o,$(Modules))

