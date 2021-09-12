consoles="/media/data/local/home/.simplemenu/section_groups/consoles.ini"

echo "[CONSOLES]" > "${consoles}"
echo "consoleList = NES,SNES,MASTER SYSTEM,SEGA GENESIS,SEGA CD,SEGA 32X,PLAYSTATION,NEO-GEO,TURBOGRAFX-16,TURBOGRAFX-CD,ATARI 2600,ATARI 7800" >> "${consoles}"

echo "" >> "${consoles}"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [SNES] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname pocketsnes*.opk | head -n 1)
if [ -z "$var" ]
then
		var=$(find /media/data/apps/ -type f -iname pocketsnes*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .sfc,.smc,.zip" >> ${consoles}
var=$(find /media/sdcard/ROMS/SFC/ -type f  -iname \*.smc -o -iname \*.zip -o -iname \*.sfc  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/sdcard/ROMS/SNES/ -type f  -iname \*.smc -o -iname \*.zip -o -iname \*.sfc  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/SFC -type f  -iname \*.smc -o -iname \*.zip -o -iname \*.sfc  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/SNES/ -type f  -iname \*.smc -o -iname \*.zip -o -iname \*.sfc  | head -n 1 | rev | cut -d"/" -f2-  | rev)
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
echo [NES] >> "${consoles}"       
var=$(find /media/sdcard/APPS/ -type f -iname fceux*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname fceux*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .nes,.zip,7z" >> ${consoles}
var=$(find /media/sdcard/ROMS/FC/ -type f  -iname \*.nes -o -iname \*.zip -o -iname \*.7z  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/sdcard/ROMS/NES/ -type f  -iname \*.nes -o -iname \*.zip -o -iname \*.7z  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/FC/ -type f  -iname \*.nes -o -iname \*.zip -o -iname \*.7z  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/NES/ -type f  -iname \*.nes -o -iname \*.zip -o -iname \*.7z  | head -n 1 | rev | cut -d"/" -f2-  | rev)
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
echo [MASTER SYSTEM] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname gen*plus*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname gen*plus*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=$(find /media/sdcard/APPS/ -type f -iname pico*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname pico*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .sms,.zip" >> ${consoles}
var=$(find /media/sdcard/ROMS/SMS/ -type f  -iname \*.sms -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/SMS/ -type f  -iname \*.sms -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/sdcard/ROMS/GGSMS/SMS/ -type f  -iname \*.sms -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/GGSMS/SMS/ -type f  -iname \*.sms -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> "$consoles"
fi
echo "scaling = 0" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [SEGA GENESIS] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname gen*plus*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname gen*plus*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=$(find /media/sdcard/APPS/ -type f -iname pico*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname pico*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .md,.smd,.bin,.zip" >> ${consoles}
var=$(find /media/sdcard/ROMS/MD/ -type f  -iname \*.md -o -iname \*.smd -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/sdcard/ROMS/SMD/ -type f  -iname \*.md -o -iname \*.smd -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/MD/ -type f  -iname \*.md -o -iname \*.smd -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/SMD/ -type f  -iname \*.md -o -iname \*.smd -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >>  "$consoles"
fi
echo "scaling = 0" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [SEGA CD] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname gen*plus*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname gen*plus*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=$(find /media/sdcard/APPS/ -type f -iname pico*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname pico*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .cue,.bin,.img" >> ${consoles}
var=$(find /media/sdcard/ROMS/SEGA\ CD/ -type f  -iname \*.cue -o -iname \*.bin -o -iname \*.img  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/SEGA\ CD/ -type f  -iname \*.cue -o -iname \*.bin -o -iname \*.img  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/"  >> "$consoles"
fi
echo "scaling = 0" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [SEGA 32X] >> "${consoles}"

var=$(find /media/sdcard/APPS/ -type f -iname pico*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname pico*.opk | head -n 1)	
fi
if [ -z "$var" ]
then
	var=$(find /media/sdcard/APPS/ -type f -iname gen*plus*.opk | head -n 1)	
fi
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname gen*plus*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .zip,.32x" >> ${consoles}
var=$(find /media/sdcard/ROMS/32X/ -type f  -iname \*.zip -o -iname \*.32x  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/32X/ -type f  -iname \*.zip -o -iname \*.32x  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> "$consoles"
fi
echo "scaling = 0" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [PLAYSTATION] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname pcsx*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname pcsx*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .bin,.pbp,.chd,.zip,.img,.iso" >> ${consoles}

var=$(find /media/sdcard/ROMS/PS/ -type f  -iname \*.bin -o -iname \*.pbp -o -iname \*.chd -o -iname \*.zip -o -iname \*.img -iname \*.iso  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/sdcard/ROMS/PSX/ -type f  -iname \*.bin -o -iname \*.pbp -o -iname \*.chd -o -iname \*.zip -o -iname \*.img -iname \*.iso  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/PS/ -type f  -iname \*.bin -o -iname \*.pbp -o -iname \*.chd -o -iname \*.zip -o -iname \*.img -iname \*.iso  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/PSX/ -type f  -iname \*.bin -o -iname \*.pbp -o -iname \*.chd -o -iname \*.zip -o -iname \*.img -iname \*.iso  | head -n 1 | rev | cut -d"/" -f2-  | rev)
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
echo [NEO GEO] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname gngeo*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname gngeo*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=$(find /media/sdcard/APPS/ -type f -iname fba*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname fba*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .zip" >> ${consoles}

var=$(find /media/sdcard/ROMS/NEOGEO/ -type f  -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/sdcard/ROMS/NEO-GEO/ -type f  -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/NEOGEO/ -type f  -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/NEO-GEO/ -type f  -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
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
echo [TURBOGRAFX-16] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname temper*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname temper*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .pce,.zip" >> ${consoles}

var=$(find /media/sdcard/ROMS/PCE/ -type f  -iname \*.zip -o -iname \*.pce  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/PCE/ -type f  -iname \*.zip -o -iname \*.pce  | head -n 1 | rev | cut -d"/" -f2-  | rev)
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
echo [TURBOGRAFX-CD] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname temper*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname temper*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .pce,.zip" >> ${consoles}

var=$(find /media/sdcard/ROMS/PCECD/ -type f  -iname \*.zip -o -iname \*.pce  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/PCECD/ -type f  -iname \*.zip -o -iname \*.pce  | head -n 1 | rev | cut -d"/" -f2-  | rev)
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
echo [ATARI 2600] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname stella*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname stella*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .bin,.zip" >> ${consoles}

var=$(find /media/sdcard/ROMS/2600/ -type f  -iname \*.zip -o -iname \*.bin	  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/2600/ -type f  -iname \*.zip -o -iname \*.bin  | head -n 1 | rev | cut -d"/" -f2-  | rev)
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
echo [ATARI 7800] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname prosystem*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname prosystem*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .a78,.rom,.zip" >> ${consoles}

var=$(find /media/sdcard/ROMS/7800/ -type f  -iname \*.a78 -o -iname \*.zip -o -iname \*.rom  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/7800/ -type f  -iname \*.a78 -o -iname \*.zip -o -iname \*.rom  | head -n 1 | rev | cut -d"/" -f2-  | rev)
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