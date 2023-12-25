FILE = tarsau
LOCAL_DIR = /usr/local/bin

make:
	gcc flaga.c -o flaga
	gcc flagb.c -o flagb
	gcc tarsau.c -o tarsau
	sudo cp $(FILE) $(LOCAL_DIR)