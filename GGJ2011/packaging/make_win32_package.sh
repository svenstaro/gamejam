#!/bin/bash

DATADIR=$(readlink -f $(dirname ${0})/..)
PACKDIR=$(readlink -f $(dirname ${0}))
cd ${PACKDIR}
cp -vr ${DATADIR}/{AUTHORS,README,bin,data} . 

# workaround for really stupid bug in wix
mkdir -p "$HOME/.wine/drive_c/Program%20Files/Windows%20Installer%20XML%20v3.5/bin/"
cp -v "$HOME/.wine/drive_c/Program Files/Windows Installer XML v3.5/bin/"*.dll "$HOME/.wine/drive_c/Program%20Files/Windows%20Installer%20XML%20v3.5/bin/"

wine "C:\Program Files\Windows Installer XML v3.5\bin\heat.exe" dir bin -out bin-files.wxs -ag -cg GameBinFiles -dr INSTALLDIR -template fragment -var var.BinDir
wine "C:\Program Files\Windows Installer XML v3.5\bin\heat.exe" dir data -out data-files.wxs -ag -cg GameDataFiles -dr INSTALLDIR -template fragment -var var.DataDir
wine "C:\Program Files\Windows Installer XML v3.5\bin\candle.exe" ai-and-the-bomb.wxs bin-files.wxs data-files.wxs -dBinDir=bin -dDataDir=data
wine "C:\Program Files\Windows Installer XML v3.5\bin\light.exe" -sval ai-and-the-bomb.wixobj bin-files.wixobj data-files.wixobj -out ai-and-the-bomb.msi -cultures:en-US -ext WixUIExtension

# clean up the workaround
rm -r "$HOME/.wine/drive_c/Program%20Files"
