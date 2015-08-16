#define WINVER 0x501
/* Do not include FileID API declaration from winbase.h.
 * Needed for WinXP. */
#define _WIN32_WINNT 0x501
#include <locale.h>
#include <io.h>
#include <stdio.h>
#include <windows.h>
#include <ctype.h>
#include <tchar.h>

#if !defined(_MSC_VER) || (_MSC_VER < 1400)
#define __out
#define __in
#define __in_opt
#endif
/* Win32 FileID API Library:
 * http://www.microsoft.com/en-us/download/details.aspx?id=22599
 * Needed for WinXP. */
#include <fileextd.h>



/* Check if the fd is a cygwin/msys's pty. */
int is_cygpty(int fd)
{
	HANDLE h;
	int size = sizeof(FILE_NAME_INFO) + sizeof(WCHAR) * MAX_PATH;
	FILE_NAME_INFO *nameinfo;
	WCHAR *p = NULL;

	h = (HANDLE) _get_osfhandle(fd);
	if (GetFileType(h) != FILE_TYPE_PIPE) {
		return 0;
	}
	nameinfo = malloc(size); 
	if (nameinfo == NULL) {
		return 0;
	}
	if (GetFileInformationByHandleEx(h, FileNameInfo, nameinfo, size)) {
		nameinfo->FileName[nameinfo->FileNameLength / sizeof(WCHAR)] = L'\0';
		p = nameinfo->FileName;
		if (wcsncmp(p, L"\\cygwin-", 8) == 0) {
			p += 8;
		} else if (wcsncmp(p, L"\\msys-", 6) == 0) {
			p += 6;
		} else {
			p = NULL;
		}
		if (p != NULL) {
			p += 16;	/* Skip 16-digit hexadecimal. */
			if (wcsncmp(p, L"-pty", 4) == 0) {
				p += 4;
			} else {
				p = NULL;
			}
		}
		if (p != NULL) {
			while (*p && isdigit(*p))	/* Skip pty number. */
				++p;
			if (wcsncmp(p, L"-from-master", 12) == 0) {
				p += 12;
			} else if (wcsncmp(p, L"-to-master", 10) == 0) {
				p += 10;
			} else {
				p = NULL;
			}
		}
	}
	free(nameinfo);
	return (p != NULL);
}

/* Check if at least one cygwin/msys pty is used. */
int is_cygpty_used(void)
{
	int fd, ret = 0;

	for (fd = 0; fd < 3; fd++) {
		ret |= is_cygpty(fd);
	}
	return ret;
}



const char *get_file_type_name(HANDLE h)
{
	int type = GetFileType(h);

	switch (type) {
	case FILE_TYPE_UNKNOWN:
		return "Unknown";
	case FILE_TYPE_DISK:
		return "Disk";
	case FILE_TYPE_CHAR:
		return "Char";
	case FILE_TYPE_PIPE:
		return "Pipe";
	}
	return "";
}

int main(int argc, char argv[])
{
	int istty[3] = {0};
	int i;

	setlocale(LC_ALL, "");

	for (i = 0; i < 3; i++) {
		istty[i] = _isatty(i);
	}
	printf("Is stdin a tty? %s\n",  istty[0] ? "yes": "no");
	printf("Is stdout a tty? %s\n", istty[1] ? "yes": "no");
	printf("Is stderr a tty? %s\n", istty[2] ? "yes": "no");
	printf("\n");

	for (i = 0; i < 3; i++) {
		HANDLE h;
		int size = sizeof(FILE_NAME_INFO) + sizeof(WCHAR) * MAX_PATH;
		FILE_NAME_INFO *nameinfo;
		const char *type;

		h = (HANDLE) _get_osfhandle(i);
		nameinfo = malloc(size); 
		if (nameinfo == NULL) {
			printf("Not enough memory\n");
			exit(1);
		}
		type = get_file_type_name(h);
		if (GetFileInformationByHandleEx(h, FileNameInfo, nameinfo, size)) {
			nameinfo->FileName[nameinfo->FileNameLength / sizeof(WCHAR)] = L'\0';
			printf("%d (%s): %S, is_cygpty: %d\n", i, type,
					nameinfo->FileName, is_cygpty(i));
		} else {
			printf("%d: GetFileInformationByHandleEx error: %d (%s)\n",
					i, GetLastError(), type);
		}
		free(nameinfo);
	}
	printf("\n");
	printf("Is running on mintty (or other term)? %s\n", is_cygpty_used() ? "yes" : "no");

	return 0;
}
