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

/*
utils::~utils() {
	int i;
	for (i = 0; i < count; i++) {
		free(filename[i]);
	}
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
		//	if (de->d_type == DT_REG && name_len >= 4) {
		        if (name_len >= 4) {
				if (count >= f_alloc) {
					f_alloc *= 2;
					filename = (char **)realloc(filename, f_alloc * sizeof(char*));
				}
				filename[count] = strdup(de->d_name); //only get the filename
			//	printf("filename : '%s'\n", filename[count]);
				count++;
			}
		}
	}
	closedir(dir);
}


bool utils::Make_MD5(string backup_path) {
	int i;
//	int index = count;
	string name;
	string Full_File;
	get_file_in_folder(backup_path.c_str());
	//miui_func md5sum
	printf(" * Generating MD5...\n");
	 if ( count >= 0) {
	//	std::cout << "cout = " << count << std::endl;
	      //  std::cout << "index = " << index << std::endl;	
		for (i = 0; i < count; i++) {
		//	name = filename[i];
		//	std::cout << "i++ = " << i << std::endl;
			Full_File = backup_path + "/" + filename[i];
		//	std::cout << "Full_File is " << Full_File << std::endl;
			if (access(Full_File.c_str(), F_OK) == 0) {
				md5sum.setfn(backup_path + "/" + filename[i]);
				if (md5sum.computeMD5() == 0) {
				     if (md5sum.write_md5digest() == 0) {
					printf(" * MD5 Create. \n");
				//	return true;
				} else {
					printf(" * MD5 Error!\n");
					return false;
				}
			}
		}
	     }
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
//	string name;
	get_file_in_folder(backup_path.c_str());
	//miui_func md5sum;
	if (count == 0) {
		std::cout << "Not file found in the folder" << backup_path << std::endl;
		return false;
	}
                  // std::cout << "count = " << count << std::endl;	
		for (index = 0; index < count; index++) {
		//	name = filename[index];
			Full_Filename = backup_path + "/" + filename[index];
			if (access(Full_Filename.c_str(), F_OK) == 0) {
			//	std::cout << "Filename is: " << Full_Filename << std::endl;
				md5file = Full_Filename;
				md5file += ".md5";
				if (access(md5file.c_str(), F_OK) != 0) {
					printf("No md5 file found for '%s'\n", filename[index]);
					printf("Please unselect Enable MD5 verification to restore.\n");
					return false;
				}
				md5sum.setfn(Full_Filename);
				if (md5sum.verify_md5digest() != 0) {
					printf("MD5 failed to match on '%s' \n", filename[index]);
					return false;
				} else {
					printf("MD5 match...\n");
					return true;
				}
			}
		}
	return false;
}





