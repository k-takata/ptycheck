!IF "$(CPU)"=="AMD64" || "$(TARGET_CPU)"=="x64" || "$(Platform)"=="x64" || DEFINED(AMD64)
WIN64=1
!ENDIF

!IFDEF WIN64
arch=x64
!ELSE
arch=x86
!ENDIF

CFLAGS = -nologo


all: $(arch)/ntdllstub.lib

$(arch)/ntdllstub.lib: $(arch)/ntdll.obj ntdll.def
	link /nologo /dll /out:$(arch)/ntdll.dll /implib:$@ $(arch)/ntdll.obj /def:ntdll.def

$(arch)/ntdll.obj: ntdll.c
	-if not exist $(arch) mkdir $(arch)
	$(CC) $(CFLAGS) /c /Fo$@ $**

clean:
	-rd /s /q x86 x64
