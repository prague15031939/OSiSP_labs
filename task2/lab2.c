#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

#define CURRENT_DIR ".\0"
#define PARENT_DIR "..\0"

long total_catalogs_checked = 0;
long total_files_checked = 0;

void search_files(char *current_catalog_name) {
	DIR *current_catalog = opendir(current_catalog_name);
	struct dirent *entry;
	while ((entry = readdir(current_catalog)) != NULL) {
		if (entry->d_type == DT_REG) {
			total_files_checked++;
			printf("%s\n", entry->d_name);
		}
		else if (entry->d_type == DT_DIR) {
			total_catalogs_checked++;
			if (strcmp(entry->d_name, CURRENT_DIR) != 0 && strcmp(entry->d_name, PARENT_DIR) != 0)
				search_files(entry->d_name);
		}
	}
}

int main(int argc, char *argv[]) {
	char *start_catalog_name, *target_file_name;
	start_catalog_name = argv[1];
	target_file_name = argv[2];

	search_files(start_catalog_name);

	printf("total files checked: %ld\ntotal catalogs checked: %ld\n", 
			total_files_checked, total_catalogs_checked);
	return 0;
}
