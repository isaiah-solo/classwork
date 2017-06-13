Isaiah Solomon
Brian DeGuzman

README.TXT

Note: We edited chmod() to allow the sticky bit to be toggled without using sudo

PROTECTFILE EXAMPLE
To compile: 	make
To encrypt: 	./protectfile -e 0x1234567898765432 testfile
To decrypt: 	./protectfile -d 0x1234567898765432 testfile

SETKEY EXAMPLE
To compile: 	make
To set key: 	./setkey 0x1234567898765432

MOUNTING CRYPTOFS
To mount: 	sudo mount_cryptofs <target dir> <src>
