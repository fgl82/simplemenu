consoles="/media/data/local/home/.simplemenu/section_groups/home computers.ini"

echo "[CONSOLES]" > "${consoles}"
echo "consoleList = ZX SPECTRUM,MSX,COMMODORE 64,AMIGA,AMSTRAD CPC,PICO-8,BBC MICRO,ATARI ST" >> "${consoles}"

echo "" >> "${consoles}"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [ZX SPECTRUM] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname *speccy*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname *speccy*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .tzx,.zip,.sna" >> ${consoles}
var=$(find /media/sdcard/ROMS/ZXSPECTRUM/ -type f -iname \*.tzx -o -iname \*.zip -o -iname \*.sna  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/ZXSPECTRUM/ -type f -iname \*.tzx -o -iname \*.zip -o -iname \*.sna  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> ${consoles}
fi
echo "scaling = 0" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [MSX] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname *msx*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname *msx*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .zip" >> ${consoles}
var=$(find /media/sdcard/ROMS/MSX/ -type f -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/MSX/ -type f -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> ${consoles}
fi
echo "scaling = 0" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [COMMODORE 64] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname *Vice*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname *Vice*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .crt,.d64,.bin" >> ${consoles}
var=$(find /media/sdcard/ROMS/C64/ -type f -iname \*.crt -o -iname \*.d64 -o -iname \*.bin | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/C64/ -type f -iname \*.crt -o -iname \*.d64 -o -iname \*.bin | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> ${consoles}
fi
echo "scaling = 0" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [AMIGA] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname *uae4all*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname *uae4all*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .adf,.zip,.sna" >> ${consoles}
var=$(find /media/sdcard/ROMS/AMIGA/ -type f -iname \*.adf -o -iname \*.zip -o -iname \*.sna | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/AMIGA/ -type f -iname \*.adf -o -iname \*.zip -o -iname \*.sna | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> ${consoles}
fi
echo "scaling = 0" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [AMSTRAD CPC] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname *arnold*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname *arnold*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .dsk,.zip" >> ${consoles}
var=$(find /media/sdcard/ROMS/AMSTRAD/ -type f -iname \*.dsk -o -iname \*.zip | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/AMSTRAD/ -type f -iname \*.dsk -o -iname \*.zip | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> ${consoles}
fi
echo "scaling = 0" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [PICO-8] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname *pico8*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname *pico8*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .png" >> ${consoles}
var=$(find /media/sdcard/ROMS/PICO8/ -type f -iname \*.png | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/PICO8/ -type f -iname \*.png | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> ${consoles}
fi
echo "scaling = 0" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [BBC MICRO] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname *beebem*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname *beebem*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .ssd,.zip" >> ${consoles}
var=$(find /media/sdcard/ROMS/BBC/ -type f -iname \*.ssd -o -iname \*.zip | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/BBC/ -type f -iname \*.ssd -o -iname \*.zip | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> ${consoles}
fi
echo "scaling = 0" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [ATARI ST] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname *castaway*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname *castway*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .a78,.rom,.ipf,.st" >> ${consoles}
var=$(find /media/sdcard/ROMS/BBC/ -type f -iname \*.a78 -o -iname \*.rom -o -iname \*.ipf -o -iname \*.st | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/BBC/ -type f -iname \*.a78 -o -iname \*.rom -o -iname \*.ipf -o -iname \*.st | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> ${consoles}
fi
echo "scaling = 0" >> ${consoles}