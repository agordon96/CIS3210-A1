Aaron Gordon 0884023
Assignment 1 for CIS*3210

Data is present in report.txt and file.txt is used for sending file data. It can be edited at will.
All functionality should be present, although all text is still dealt with regardless of buffer size in the ./client argument since it iterates the char* through the entire file.

To run:
- make or make all(make clean is functional but isn't needed)
- ./server PORT (mine was 12027) to start the server and CTRL+C to end. Rarely wouldn't bind on some types of connections.
- ./client IP/HOSTNAME:PORT LEN where len is an optional buffer length, the IP and port are mandatory as is the colon.
- If you wish, you can run the script with bash script.sh IP/HOSTNAME which will use the 12027 port automatically and run the client many times

Error handling is done throughout.