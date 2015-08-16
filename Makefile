!IF "$(CPU)"=="AMD64" || "$(TARGET_CPU)"=="x64" || "$(Platform)"=="x64" || DEFINED(AMD64)
WIN64=1
!ENDIF

!IFDEF WIN64
arch=x64
fileidarch=amd64
!ELSE
arch=x86
fileidarch=i386
!ENDIF

CFLAGS = -nologo -MD

# Specify the directory of Win32 FileID API Library with FILEIDAPIDIR.
!IFDEF FILEIDAPIDIR
# _WIN32_WINNT should be < 0x0600 in order not to include FileID API
# declaration from winbase.h.  Needed for WinXP.
CFLAGS = $(CFLAGS) -DWINVER=0x0501 -D_WIN32_WINNT=0x0501 -I$(FILEIDAPIDIR)/inc
LIB = $(FILEIDAPIDIR)/lib/$(fileidarch);$(LIB)
!ENDIF

all: ptycheck.exe

ptycheck.exe: ptycheck.obj iscygpty.obj ntdllstub/$(arch)/ntdllstub.lib
	$(CC) $(CFLAGS) /Fe$@ $** fileextd.lib kernel32.lib

ptycheck.obj: ptycheck.c
	$(CC) $(CFLAGS) /c /Fo$@ $*.c

iscygpty.obj: iscygpty.c
	$(CC) $(CFLAGS) /c /Fo$@ $*.c

ntdllstub/$(arch)/ntdllstub.lib:
	cd ntdllstub
	$(MAKE)
	cd ..

clean:
	del ptycheck.exe ptycheck.obj iscygpty.obj
	cd ntdllstub
	$(MAKE) clean
	cd ..
