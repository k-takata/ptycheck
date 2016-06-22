# ptycheck
A sample to check if a program running on mintty (or some other terminal) on Cygwin/MSYS.


## How to compile

You can use VC6 or later to compile this sample.

If you want to execute the sample on WinXP or if you use VC8 (VC2005) or earlier, you need [Win32 FileID API Library](http://www.microsoft.com/en-us/download/details.aspx?id=22599).
You need to specify the directory of Win32 FileID API Library with `FILEIDAPIDIR`:

```
nmake FILEIDAPIDIR=..\FileIDAPI
```

If you want to execute the sample on Vista or later and if you use VC9 (VC2008) or later, you don't need Win32 FileID API Library. Just run `nmake`:

```
nmake
```

### Configurations of iscygpty.c

iscygpty.c has some configurations:

* `USE_FILEEXTD`: If this is defined, Win32 FileID API Library is linked. This is mandatory if you want to detect Cygwin pty on WinXP. If you want to use this, `_WIN32_WINNT` should be 0x0501 or lower.
* `USE_DYNFILEID`: If this is defined, `GetFileInformationByHandleEx` is loaded dynamically (more precisely, explicitly). You can run a program even on WinXP or earlier, but Cygwin pty is not detected on the old OSes when `USE_FILEEXTD` is not defined.
* `ENABLE_STUB_IMPL`: If this is defined and `USE_FILEEXTD` is not defined and when VC2005 or earlier is used, `is_cygpty()` becomes stub implementation; it always returns 0.

If both `USE_FILEEXTD` and `USE_DYNFILEID` are not defined, the program can run only on Vista or later.


## License

[The MIT License](LICENSE). (You can also choose the Vim license.)
