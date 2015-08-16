!IF "$(CPU)"=="AMD64" || "$(TARGET_CPU)"=="x64" || "$(Platform)"=="x64" || DEFINED(AMD64)
WIN64=1
!ENDIF

!IFDEF WIN64
outdir=x64
fileiddir=amd64
!ELSE
outdir=x86
fileiddir=i386
!ENDIF

CFLAGS = -nologo -MD

!IFDEF FILEIDAPIDIR
CFLAGS = $(CFLAGS) -I$(FILEIDAPIDIR)/inc
LDFLAGS = /link /libpath:$(FILEIDAPIDIR)/lib/$(fileiddir)
!ENDIF

all: ptycheck.exe

ptycheck.exe: ptycheck.obj iscygpty.obj ntdllstub/$(outdir)/ntdllstub.lib
	$(CC) $(CFLAGS) /Fe$@ $** fileextd.lib kernel32.lib $(LDFLAGS)

ptycheck.obj: ptycheck.c
	$(CC) $(CFLAGS) /c /Fo$@ $*.c

iscygpty.obj: iscygpty.c
	$(CC) $(CFLAGS) /c /Fo$@ $*.c

ntdllstub/$(outdir)/ntdllstub.lib:
	cd ntdllstub
	$(MAKE)
	cd ..

clean:
	del ptycheck.exe ptycheck.obj
	cd ntdllstub
	$(MAKE) clean
	cd ..
