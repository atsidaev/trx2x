# trx2x
This is cross-platform port of ZX Spectrum floppy image converter trx2x.

# Description
This tool can help to convert files from/to various floppy disk formats: UDI, TRD, SCL, FDI, TD0, FDD.
Also it can be used to put TR-DOS hobeta files (*.$b, *.$c etc) to a floppy disk image, but this ability is limited.

File format is detected by the extension.

# Usage

```
trx2x <filename1> <filename2>
```

# Examples

## Convert TD0 image to TRD

```
trx2x file.td0 file.trd
```

## Create SCL file from Hobeta BASIC file

```
trx2x program.$b file.scl
```

# Authors

Original Win32 version (C) 2002 Alex Makeev \
Based on UltraDiskImage engine by Alex Makeev \
WWW: http://zxmak.chat.ru/

Cross-platform port (proven to work in 64-bit Linux) by Alexander Tsidaev.
