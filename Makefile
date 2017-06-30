###############
#
# Author      : undefined
# Version     : 1.0
# Description : make file for (blocker)
#
###############



# compiler:
CC = gcc

# compiler options.
# use -DOPTION1=value,OPTION2=value to specify macros.
# customisable macros are  
# [CONFIG_FILE | HOSTS_FILE | CONFIG_TAG]
# 
# read the documentation provided in the source code to read 
# about them.
#

# the build target executable with name :
TARGET = blocker

#name of the configure file for the tool:
CONFIG_FILE = .blockrc

all :$(TARGET)
	
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) -r $(TARGET)

.PHONY: install
install: $(TARGET)
	mkdir -p undefined/defaults
	cp $(TARGET) undefined/$(TARGET)
	cp config_defaults undefined/defaults
	cp config_defaults undefined/$(CONFIG_FILE)
	
	
.PHONY: uninstall
uninstall:
	rm -rf undefined
