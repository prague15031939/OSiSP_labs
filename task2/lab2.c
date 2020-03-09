#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include <locale.h>

#define CURRENT_DIR ".\0"
#define PARENT_DIR "..\0"

long total_catalogs_checked = 0;
long total_files_checked = 0;

char *script_name;

char *create_new_path(char *new_path, char *current_catalog_name, char *entry) {
	strcpy(new_path, current_catalog_name);
	if (new_path[strlen(new_path) - 1] != '/')
		strcat(new_path, "/\0");
	strcat(new_path, entry);
	return new_path;
}

void form_rights(char *dest, int mode) {
	int entry_type_arr[7][2] = {{S_IFSOCK, 's'}, 
								{S_IFLNK, 'l'}, 
								{S_IFREG, '-'}, 
								{S_IFBLK, 'b'}, 
								{S_IFDIR, 'd'}, 
								{S_IFCHR, 'c'}, 
								{S_IFIFO, 'p'}};
	int entry_rights_arr[9][2] = {{S_IRUSR, 'r'},
							  	  {S_IWUSR, 'w'}, 
   							      {S_IXUSR, 'x'}, 
							      {S_IRGRP, 'r'}, 
							      {S_IWGRP, 'w'}, 
							      {S_IXGRP, 'x'}, 
							  	  {S_IROTH, 'r'}, 
							      {S_IWOTH, 'w'}, 
							  	  {S_IXOTH, 'x'}};
	int count = 0;
	for (int i = 0; i < 7; i++) {
		if ((mode & entry_type_arr[i][0]) == entry_type_arr[i][0]) {			
			dest[count++] = entry_type_arr[i][1];
			break;
		}
	}
	for (int i = 0; i < 9; i++) {
		if (mode & entry_rights_arr[i][0])
			dest[count] = entry_rights_arr[i][1];
		else
			dest[count] = '-';	
		count++;
	}
	dest[count] = '\0';
}

void print_file_info(char *cur_catalog, char* file_name) {
	char *file_path = malloc(sizeof(char) * 4096);
	create_new_path(file_path, cur_catalog, file_name);
	struct stat file_info;
	if (!stat(file_path, &file_info)) {
		char access_rights[15];
		char creation_date[100];
		form_rights(access_rights, file_info.st_mode);
		struct tm lt;
		localtime_r(&file_info.st_ctime, &lt);
		strftime(creation_date, 100, "%d %B %Y", &lt);
		printf("%s %ld %s %ld %s\n", 
				file_path, 
				file_info.st_size,
				access_rights,
				file_info.st_ino,
				creation_date);
	}
	else 
		fprintf(stderr, "%s: %s: %s\n", script_name, file_path, strerror(errno));	
}

void search_files(char *current_catalog_name, char *target_file) {
	DIR *current_catalog = opendir(current_catalog_name);
	struct dirent *entry;
	if (current_catalog != NULL) {
		while ((entry = readdir(current_catalog)) != NULL) {
			if (entry->d_type == DT_REG) {
				total_files_checked++;
				if (strcmp(entry->d_name, target_file) == 0)
					print_file_info(current_catalog_name, entry->d_name); 
			}
			else if (entry->d_type == DT_DIR) {
				if (strcmp(entry->d_name, CURRENT_DIR) != 0 && strcmp(entry->d_name, PARENT_DIR) != 0) {
					total_catalogs_checked++;
					char *new_path = malloc(sizeof(char) * 4096);
					search_files(create_new_path(new_path, current_catalog_name, entry->d_name), target_file);
				}
			}
		}
		if (closedir(current_catalog) == -1)
			fprintf(stderr, "%s: %s: %s\n", script_name, current_catalog_name, strerror(errno));
	}
	else 
		fprintf(stderr, "%s: %s: %s\n", script_name, current_catalog_name, strerror(errno));
}

int main(int argc, char *argv[]) {
	setlocale(LC_TIME, "ru_RU.UTF-8");

	char *start_catalog_name, *target_file_name;
	script_name = argv[0];
	start_catalog_name = argv[1];
	target_file_name = argv[2];

	search_files(start_catalog_name, target_file_name);

	printf("total files checked: %ld\ntotal catalogs checked: %ld\n", 
			total_files_checked, total_catalogs_checked);
	return 0;
}

