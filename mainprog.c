#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define DEVICE "/dev/phonebookdevice"

int main() {
	int i, fd;
	char ch, write_buf[100], read_buf[100];
	char new_book[100];
	for(int i = 0; i < 100; i++) {
		write_buf[i] = ' '; 
		read_buf[i] = ' ';
		new_book[i] = ' ';
	}
	fd = open(DEVICE, O_RDWR); //open for reading and writing

	if(fd == -1) {
		printf("file %s either does not exist or has been locked by another process\n", DEVICE);
		exit(-1);
	}
	printf("r = read phonebook from device\n");
	printf("a = add new user (surname name age phone number email\n");
	printf("d = delete one user\n");
	printf("l = delete all users\n");
	printf("i = info about one user\n");
	printf("enter command:");
	scanf("%c", &ch);
	int last_symbol;
	int read_buf_size;
	int ind_for_new_book;
	int sim;
	switch(ch) {
	        
		case 'a':
			printf("enter data: ");
                       scanf(" %[^\n]", write_buf);
			read(fd, read_buf, sizeof(read_buf));

			//find last symbol
			read_buf_size = sizeof(read_buf);
			last_symbol = -1;
			//printf("%d\n", read_buf_size);
			for(int i = read_buf_size - 1; i >= 0; i--) {
				if(read_buf[i] != ' ') {
					last_symbol = i + 1;
					break;
				}
			}
			// find real size of write_buf
			int size_write_buf = 0;
			for(int i = 0; i < sizeof(write_buf); i++) {
				if('a' <= write_buf[i] && write_buf[i] <= 'z') {
					size_write_buf = i + 1;
				}
				if('A' <= write_buf[i] && write_buf[i] <= 'Z') {
					size_write_buf = i + 1;
				}
				if('0' <= write_buf[i] && write_buf[i] <= '9') {
					size_write_buf = i + 1;
				}
			}
			//printf("write size: %d\n", size_write_buf);
			// add new user
			for(int i = 0; i < size_write_buf; i++) {
				read_buf[last_symbol + i + 1] = write_buf[i];
			}
			//printf("%s\n", read_buf);
			//printf("%d %s\n", sizeof(read_buf), read_buf); // old_buf
			write(fd, read_buf, sizeof(read_buf));
			break;
		case 'r':
			read(fd, read_buf, sizeof(read_buf));
			int real_size = 0;
                       for(int i = 0; i < sizeof(read_buf); i++) {
                               if('a' <= read_buf[i] && read_buf[i] <= 'z') {
                                       real_size = i + 1;
                               }
                               if('A' <= read_buf[i] && read_buf[i] <= 'Z') {
                                       real_size = i + 1;
                               }
                               if('0' <= read_buf[i] && read_buf[i] <= '9') {
                                       real_size = i + 1;
                               }
                       }
			//printf("%d\n", real_size);
			int counter_spaces = 0;
			for(int i = 0; i < real_size; i++) {
				printf("%c", read_buf[i]);
				if(read_buf[i] == ' ') {
					counter_spaces++;
					if(counter_spaces == 5) {
						counter_spaces = 0;
						printf("\n");
					}
				}
			}
			printf("\n");
			break;
		case 'd':
			printf("enter data: ");
                       scanf(" %[^\n]", write_buf);
                       read(fd, read_buf, sizeof(read_buf));
                       //find last symbol
                       read_buf_size = sizeof(read_buf);
                       last_symbol = -1;
                       for(int i = read_buf_size - 1; i >= 0; i--) {
                               if(read_buf[i] != ' ') {
                                       last_symbol = i + 1;
                                       break;
                               }
                       }
                       //printf("last symbol: %d\n", last_symbol);
                       // find real size of write_buf
                       size_write_buf = 0;
                       for(int i = 0; i < sizeof(write_buf); i++) {
                               if('a' <= write_buf[i] && write_buf[i] <= 'z') {
                                       size_write_buf = i + 1;
                               }
                               if('A' <= write_buf[i] && write_buf[i] <= 'Z') {
                                       size_write_buf = i + 1;
                               }
                               if('0' <= write_buf[i] && write_buf[i] <= '9') {
                                       size_write_buf = i + 1;
                               }
                       }
			counter_spaces = 0;
			ind_for_new_book = 0;
			sim = 0;
			for(int i = 0; i < last_symbol; i++) {
				if(sim == 0 && counter_spaces == 0 && (i == 0 || read_buf[i - 1] == ' ')) {
					//check surname
					sim = 1;
					for(int j = 0; i + j < last_symbol && j < size_write_buf; j++) {
						if(read_buf[i + j] != write_buf[j]) {
							sim = 0;
						}
					}
					if(sim == 1) {
						counter_spaces = 0;
					}
				}
				if(sim == 1) {
					if(read_buf[i] == ' ') {
						counter_spaces++;
					}
					if(counter_spaces == 5) {
						sim = 0;
					}
				}
				else {
					new_book[ind_for_new_book] = read_buf[i];
					ind_for_new_book++;
				}
			}
			write(fd, new_book, sizeof(new_book));
                 	break;
                
		case 'l':
			write(fd, read_buf, sizeof(read_buf));
			break;
		case 'i':
			printf("enter data: ");
                       scanf(" %[^\n]", write_buf);
                       read(fd, read_buf, sizeof(read_buf));
                       //find last symbol
                       read_buf_size = sizeof(read_buf);
                       last_symbol = -1;
                       for(int i = read_buf_size - 1; i >= 0; i--) {
                               if(read_buf[i] != ' ') {
                                       last_symbol = i + 1;
                                       break;
                               }
                       }
                       //printf("last symbol: %d\n", last_symbol);
                       // find real size of write_buf
                       size_write_buf = 0;
                       for(int i = 0; i < sizeof(write_buf); i++) {
                               if('a' <= write_buf[i] && write_buf[i] <= 'z') {
                                       size_write_buf = i + 1;
                               }
                               if('A' <= write_buf[i] && write_buf[i] <= 'Z') {
                                       size_write_buf = i + 1;
                               }
                               if('0' <= write_buf[i] && write_buf[i] <= '9') {
                                       size_write_buf = i + 1;
                               }
                       }
			counter_spaces = 0;
			ind_for_new_book = 0;
			sim = 0;
			for(int i = 0; i < last_symbol; i++) {
				if(sim == 0 && counter_spaces == 0 && (i == 0 || read_buf[i - 1] == ' ')) {
					//check surname
					sim = 1;
					for(int j = 0; i + j < last_symbol && j < size_write_buf; j++) {
						if(read_buf[i + j] != write_buf[j]) {
							sim = 0;
						}
					}
					if(sim == 1) {
						counter_spaces = 0;
					}
				}
				if(sim == 1) {
					printf("%c", read_buf[i]);
					if(read_buf[i] == ' ') {
						counter_spaces++;
					}
					if(counter_spaces == 5) {
						sim = 0;
					}
				}
			}
			printf("\n");
                 	break;
		default:
			printf("command not recognised\n");
			break;
	}
	close(fd);
	
	return 0;
}
