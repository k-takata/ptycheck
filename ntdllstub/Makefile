!IF "$(CPU)"=="AMD64" || "$(TARGET_CPU)"=="x64" || "$(Platform)"=="x64" || DEFINED(AMD64)
WIN64=1
!ENDIF

!IFDEF WIN64
outdir=x64
!ELSE
outdir=x86
!ENDIF

CFLAGS = -nologo


all: $(outdir)/ntdllstub.lib

$(outdir)/ntdllstub.lib: $(outdir)/ntdll.obj ntdll.def
	link /nologo /dll /out:$(outdir)/ntdll.dll /implib:$@ $(outdir)/ntdll.obj /def:ntdll.def

$(outdir)/ntdll.obj: ntdll.c
	-if not exist $(outdir) mkdir $(outdir)
	$(CC) $(CFLAGS) /c /Fo$@ $**

clean:
	-rd /s /q x86 x64
