#include <dirent.h>
#include <dlfcn.h>
#include <cstdlib>
#include <cstdio>

#include <unistd.h>

void dumpVtable(char *file, void **vtable) {
	FILE* dump = fopen(file, "w");
	if (!dump) return;
	int index = 0;

	while (*vtable) {
		Dl_info info;
		dladdr(*vtable++, &info);
		fprintf(dump, "%d %p %s \n", index++, info.dli_saddr, info.dli_sname);
	}

	fclose(dump);
}

void** getVtable(char *symbol) {
	return (void **)((int)dlsym(RTLD_DEFAULT, symbol) + 8);
}

int getVtableIndex(void **vtable, char *symbol) {
	int index = 0;
	while (*vtable) {
		Dl_info info;
		dladdr(*vtable++, &info);
		if (strstr(info.dli_sname, symbol))
			return index;
		index++;
	}
	return -1;
}

pid_t getPid(char *cmdline) {
	pid_t pid = -1;

	DIR *dp = opendir("/proc");
	if (!dp) return -1;
	dirent *dirp;

	while (dirp = readdir(dp)) {
		int id = atoi(dirp->d_name);
		if (id <= 0) continue;

		char buffer[1024] = { 0 };

		sprintf(buffer, "/proc/%d/cmdline", id);
		FILE *file = fopen(buffer, "rt");

		if (!fgets(buffer, sizeof(buffer), file)) {
			fclose(file);
			continue;
		}
		fclose(file);

		if (!strstr(buffer, cmdline)) continue;

		pid = id;
		break;
	}

	closedir(dp);

	return pid;
}

uintptr_t findLibrary(char *library, pid_t pid = -1) {
	char filename[0xFF] = { 0 }, buffer[1024] = { 0 };
	FILE *fp = NULL;

	sprintf(filename, "/proc/%d/maps", pid == -1 ? getpid() : pid);

	fp = fopen(filename, "rt");
	if (fp == NULL) return 0;

	uintptr_t address = 0;
	while (fgets(buffer, sizeof(buffer), fp)) {
		if (!strstr(buffer, library)) continue;
		address = (uintptr_t)strtoul(buffer, NULL, 16);
		break;
	}

	fclose(fp);
	return address;
}
