#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

#define CURRENT_DIR ".\0"
#define PARENT_DIR "..\0"

const long MAX_BUF_LEN = sizeof(char) * 1048576;

char *script_name;
int max_proc_amount;
int cur_proc_amount = 1;
int root_pid;

char *create_new_path(char *new_path, char *current_catalog_name, char *entry) {
	strcpy(new_path, current_catalog_name);
	if (new_path[strlen(new_path) - 1] != '/')
		strcat(new_path, "/\0");
	strcat(new_path, entry);
	return new_path;
}

long count_words(char *buf) {
	long count = 0, i = 0;
	long buf_len = strlen(buf);
	while (i < buf_len) {
		while (i < buf_len && (buf[i] == ' ' || buf[i] == 9  || buf[i] == '\n'))
			i++;
		if (i < buf_len)
			count++;
		while (i < buf_len && !(buf[i] == ' ' || buf[i] == 9 || buf[i] == '\n'))
			i++;
	}
	return count;
}

void proccess_file(char *file_path) {
	FILE *f = fopen(file_path, "rb");

	char *buffer = (char *)malloc(MAX_BUF_LEN);
	long word_count = 0;
	long long byte_count = 0;
	while (!feof(f)) {
		long read_num = fread(buffer, sizeof(char), MAX_BUF_LEN, f);
		buffer[read_num] = '\0';
		byte_count += read_num;
		word_count += count_words(buffer);
	}
	fclose(f);

	printf("%d: %s: %lld: %ld\n", getpid(), file_path, byte_count, word_count);
}

void search_files(char *current_catalog_name) {
	DIR *current_catalog = opendir(current_catalog_name);
	struct dirent *entry;
	if (current_catalog == NULL) {
		fprintf(stderr, "%s: %s: %s\n", script_name, current_catalog_name, strerror(errno));
		return;
	}
	while ((entry = readdir(current_catalog)) != NULL) {
		char *new_path = malloc(sizeof(char) * 4096);
		if (entry->d_type == DT_REG) { 
			pid_t pid = root_pid;
			if (getpid() == root_pid) {
				if (cur_proc_amount < max_proc_amount) {
					pid = fork();
					cur_proc_amount++;
				}
				else {
					wait(NULL);
					cur_proc_amount--;
					pid = fork();
					cur_proc_amount++;
				}
			} 
			switch (pid) {
				case -1:
					fprintf(stderr, "%s: %s: fork error\n", script_name, current_catalog_name);	
					exit(1);
				case 0:
					proccess_file(create_new_path(new_path, current_catalog_name, entry->d_name));			
					exit(0);
			}			
		}
		else if (entry->d_type == DT_DIR) {
			if (strcmp(entry->d_name, CURRENT_DIR) != 0 && strcmp(entry->d_name, PARENT_DIR) != 0) {
				search_files(create_new_path(new_path, current_catalog_name, entry->d_name));
			}
		}
	}
	if (closedir(current_catalog) == -1)
		fprintf(stderr, "%s: %s: %s\n", script_name, current_catalog_name, strerror(errno));
}

int main(int argc, char *argv[]) {
	script_name = argv[0];
	char *start_catalog_name = argv[1];
	max_proc_amount = atoi(argv[2]);
	root_pid = getpid();

	search_files(start_catalog_name);

	return 0;
}

