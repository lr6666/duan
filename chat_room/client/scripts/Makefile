CC := gcc
Target := chat_room_client
Source := $(wildcard src/*.c)
Objs := $(patsubst %.c,%.o,$(Source))
Modules += after_log block_write block_read getpasswd log_or_register main set_disp_mode thread_read thread_write
AllObjs := $(addsuffix /src/*.o,$(Modules))

