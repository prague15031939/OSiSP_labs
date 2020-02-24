#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
	char *start_catalog_name, *target_file_name;
	start_catalog_name = argv[1];
	target_file_name = argv[2];

	DIR *start_catalog = opendir(start_catalog_name);
	struct dirent *entry;
	while ((entry = readdir(start_catalog)) != NULL) {
		puts(entry->d_name);
	}
	return 0;
}
