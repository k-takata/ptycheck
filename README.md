# ptycheck
A sample to check if a program running on mintty (or some other terminal) on Cygwin/MSYS.


## How to compile

You can use VC6 or later to compile this sample.

If you want to execute the sample on WinXP or if you use VC8 or earlier, you need [Win32 FileID API Library](http://www.microsoft.com/en-us/download/details.aspx?id=22599).
You need to specify the directory of Win32 FileID API Library with `FILEIDAPIDIR`:

```
nmake FILEIDAPIDIR=..\FileIDAPI
```

If you want to execute the sample on Vista or later and if you use VC9 or later, you don't need Win32 FileID API Library. Just run `nmake`:

```
nmake
```


## License

[The MIT License](LICENSE). (You can also choose the Vim license.)
