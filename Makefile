FILE = tarsau
LOCAL_DIR = /usr/local/bin

make:
	gcc tarsau.c -o tarsau
	sudo cp $(FILE) $(LOCAL_DIR)