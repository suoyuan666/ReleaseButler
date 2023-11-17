PROGRAM=relesebutler
SRC_DIR=src

CC=/usr/bin/gcc
CFLAGS=  -lcurl -Isrc

os-detect: 
	$(CC) $(SRC_DIR)/os-detect.c  -c -o $(SRC_DIR)/os-detect.o

libconf: 
	$(CC) $(SRC_DIR)/libconf.c  -c -o $(SRC_DIR)/libconf.o

$(PROGRAM):  os-detect libconf
	$(CC) $(SRC_DIR)/main.c $(SRC_DIR)/os-detect.o $(SRC_DIR)/libconf.o  $(CFLAGS) -o $(PROGRAM)


test_install: $(PROGRAM)
	rm ~/.config/releasebuter/config.conf && ./relesebutler -i fastfetch -f https://github.com/fastfetch-cli/fastfetch/


clean_test:
	rm -rf $(PROGRAM) fastfetch*

install:
	sudo mv ./$(PROGRAM) /usr/local/bin
