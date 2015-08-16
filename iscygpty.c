#include <ctype.h>
#include <io.h>
#include <wchar.h>
#include <windows.h>

#if _WIN32_WINNT < 0x0600
/* VC 7.1 or earlier doesn't support SAL. */
#if !defined(_MSC_VER) || (_MSC_VER < 1400)
#define __out
#define __in
#define __in_opt
#endif
/* Win32 FileID API Library:
 * http://www.microsoft.com/en-us/download/details.aspx?id=22599
 * Needed for WinXP. */
#include <fileextd.h>
#endif

#include "iscygpty.h"


#define is_wprefix(s, prefix) \
	(wcsncmp((s), (prefix), sizeof(prefix) / sizeof(WCHAR) - 1) == 0)

/* Check if the fd is a cygwin/msys's pty. */
int is_cygpty(int fd)
{
	HANDLE h;
	int size = sizeof(FILE_NAME_INFO) + sizeof(WCHAR) * MAX_PATH;
	FILE_NAME_INFO *nameinfo;
	WCHAR *p = NULL;

	h = (HANDLE) _get_osfhandle(fd);
	/* Cygwin/msys's pty is a pipe. */
	if (GetFileType(h) != FILE_TYPE_PIPE) {
		return 0;
	}
	nameinfo = malloc(size); 
	if (nameinfo == NULL) {
		return 0;
	}
	/* Check the name of the pipe:
	 * '\{cygwin,msys}-XXXXXXXXXXXXXXXX-ptyN-{from,to}-master' */
	if (GetFileInformationByHandleEx(h, FileNameInfo, nameinfo, size)) {
		nameinfo->FileName[nameinfo->FileNameLength / sizeof(WCHAR)] = L'\0';
		p = nameinfo->FileName;
		if (is_wprefix(p, L"\\cygwin-")) {
			p += 8;
		} else if (is_wprefix(p, L"\\msys-")) {
			p += 6;
		} else {
			p = NULL;
		}
		if (p != NULL) {
			while (*p && isxdigit(*p))	/* Skip 16-digit hexadecimal. */
				++p;
			if (is_wprefix(p, L"-pty")) {
				p += 4;
			} else {
				p = NULL;
			}
		}
		if (p != NULL) {
			while (*p && isdigit(*p))	/* Skip pty number. */
				++p;
			if (is_wprefix(p, L"-from-master")) {
				//p += 12;
			} else if (is_wprefix(p, L"-to-master")) {
				//p += 10;
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

/* vim: set ts=4 sw=4: */
