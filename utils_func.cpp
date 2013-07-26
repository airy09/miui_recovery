#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include "miui_func.hpp"
#include "utils_func.hpp"

utils::utils() {
	count = 0;
	f_alloc = 20;
	filename = (char **)malloc(f_alloc * sizeof(char *));
}

utils::~utils() {
	int i;
	for (i = 0; i < count; i++) {
		free(filename[i]);
	}

//	char* cursor = filename[0];
//	while (cursor != NULL) {
//		free(cursor);
//		cursor = filename[++i];
//	}
//	free(filename);

}
/*
void utils::free_string_array(char** arrary) {
	if (arrary == NULL)
		return;
	char* cursor = arrary[0];
	int i = 0;
	while (cursor != NULL) {
		free(cursor);
		cursor = arrary[++i];
	}
	free(arrary);
}

*/
void utils::get_file_in_folder(const char *backup_path) {
	char *full_name;
	DIR *dir;
	struct dirent *de;
	dir = opendir(backup_path);
	if (dir) {
		while ((de = readdir(dir)) != NULL) {
			int name_len = strlen(de->d_name);
			if (de->d_type == DT_DIR ||
				       	strcmp(de->d_name, ".") == 0 ||
				strcmp(de->d_name, "..") == 0 ||
					strstr(de->d_name, ".md5") != NULL)
				continue;
		       if (de->d_type == DT_REG && name_len >= 4) {
		       // if (name_len >= 4) {
				if (count >= f_alloc) {
					f_alloc *= 2;
					filename = (char **)realloc(filename, f_alloc * sizeof(char*));
				}
				filename[count] = strdup(de->d_name); //only get the filename
				count++;
			}
		}
	}
	closedir(dir);
}


bool utils::Make_MD5(string backup_path) {
	int i;
	string name;
	string Full_File;
	get_file_in_folder(backup_path.c_str());
	//miui_func md5sum
	printf(" * Generating MD5...\n");
	 if ( count >= 0) {
		for (i = 0; i < count; i++) {
			Full_File = backup_path + "/" + filename[i];
			if (access(Full_File.c_str(), F_OK) == 0) {
			//	md5sum.setfn(backup_path + "/" + filename[i]);
                                setfn(backup_path + "/" + filename[i]);
				//if (md5sum.computeMD5() == 0) {
				if (computeMD5() == 0) {
				     //if (md5sum.write_md5digest() == 0) {
				     if (write_md5digest() == 0) {
					printf("File: <%s>\n",filename[i]);
					printf(" * MD5 Create. \n");
				} else {
					printf("File: <%s>\n",filename[i]);
					printf(" * MD5 Error!\n");
				//	free_string_array(filename);
					return false;
				}
			}
		}
	     }
	//	free_string_array(filename);
		return true;
	} else {
		printf("Not file found in: %s\n", backup_path.c_str());
		return false;
	}
	return false;
}


bool utils::Check_MD5(string backup_path) {
	string Full_Filename, md5file;
	int index = 0;
	get_file_in_folder(backup_path.c_str());
	//miui_func md5sum;
	if (count > 0) {
		
		for (index = 0; index < count; index++) {
			Full_Filename = backup_path + "/" + filename[index];
			if (access(Full_Filename.c_str(), F_OK) == 0) {
				md5file = Full_Filename;
				md5file += ".md5";
				if (access(md5file.c_str(), F_OK) != 0) {
					printf("No md5 file found for '%s'\n", filename[index]);
					printf("Please unselect Enable MD5 verification to restore.\n");
					return false;
				}
				//md5sum.setfn(Full_Filename);
				setfn(Full_Filename);
				//if (md5sum.verify_md5digest() != 0) {
				if (verify_md5digest() != 0) {
					printf("MD5 failed to match on '%s' \n", filename[index]);
					return false;
				} else {
					printf("File: <%s>\n",filename[index]);
					printf("MD5 match...\n");
				//	return true;
				}
			}
		}
		//	free_string_array(filename);
			return true;
	} else {
		printf("Not file found in folder: <%s>\n",backup_path.c_str());
		return false;
	}

	return false;
}


