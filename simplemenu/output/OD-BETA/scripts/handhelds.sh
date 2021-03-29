consoles="/media/data/local/home/.simplemenu/section_groups/handhelds.ini"

echo "[CONSOLES]" > "${consoles}"
echo "consoleList = GAME BOY,GAME BOY COLOR,GAME BOY ADVANCE,GAME GEAR,NEO GEO POCKET,NEO GEO POCKET COLOR,ATARI LYNX,WONDERSWAN,GAME & WATCH" >> "${consoles}"

echo "" >> "${consoles}"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [GAME BOY] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname gambatte*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname gambatte*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .gb,.zip" >> ${consoles}
var=$(find /media/sdcard/ROMS/GB/ -type f  -iname \*.gb -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/sdcard/ROMS/GBGBC/GB/ -type f  -iname \*.gb -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/GB/ -type f  -iname \*.gb -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/GBGBC/GB/ -type f  -iname \*.gb -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> ${consoles}
fi
echo "scaling = 1" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [GAME BOY COLOR] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname gambatte*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname gambatte*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .gbc,.zip" >> ${consoles}
var=$(find /media/sdcard/ROMS/GBC/ -type f  -iname \*.gbc -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/sdcard/ROMS/GBGBC/GBC/ -type f  -iname \*.gbc -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/GBC/ -type f  -iname \*.gbc -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/GBGBC/GBC/ -type f  -iname \*.gbc -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> ${consoles}
fi
echo "scaling = 1" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [GAME BOY ADVANCE] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname regba*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname regba*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .gba,.zip" >> ${consoles}
var=$(find /media/sdcard/ROMS/GBA/ -type f  -iname \*.gba -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/GBA/ -type f  -iname \*.gba -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> "$consoles"
fi
echo "scaling = 1" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [GAME GEAR] >> "${consoles}"
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
echo "romExts = .bin,.gg,.zip" >> ${consoles}
var=$(find /media/sdcard/ROMS/GG/ -type f  -iname \*.gg -o -iname \*.bin -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/sdcard/ROMS/GGSMS/GG/ -type f  -iname \*.gg -o -iname \*.bin -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/GG/ -type f  -iname \*.gg -o -iname \*.bin -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/GGSMS/GG -type f  -iname \*.gg -o -iname \*.bin -o -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >>  "$consoles"
fi
echo "scaling = 1" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [NEO GEO POCKET] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname race*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname race*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .ngp,.ngf,.zip" >> ${consoles}
var=$(find /media/sdcard/ROMS/NGP/ -type f -iname \*.ngp -o -iname \*.zip -o -iname \*.ngf  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/NGP/ -type f -iname \*.ngp -o -iname \*.zip -o -iname \*.ngf  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/"  >> "$consoles"
fi
echo "scaling = 1" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [NEO GEO POCKET COLOR] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname race*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname race*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .zip,.ngc" >> ${consoles}
var=$(find /media/sdcard/ROMS/NGPC/ -type f -iname \*.zip -o -iname \*.ngc  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/NGPC/ -type f -iname \*.zip -o -iname \*.ngc  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> "$consoles"
fi
echo "scaling = 1" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [ATARI LYNX] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname handy*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname handy*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .lnx" >> ${consoles}

var=$(find /media/sdcard/ROMS/LYNX/ -type f -iname \*.lnx  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/LYNX/ -type f -iname \*.lnx   | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> ${consoles}
fi
echo "scaling = 1" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [WONDERSWAN] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname oswan*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname oswan*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .ws" >> ${consoles}

var=$(find /media/sdcard/ROMS/OSWAN/ -type f -iname \*.ws  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/sdcard/ROMS/OSWANC/ -type f -iname \*.wsc | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/OSWAN/ -type f -iname \*.ws | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/OSWANC/ -type f -iname \*.wsc   | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> ${consoles}
fi
echo "scaling = 1" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo "[GAME & WATCH]" >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname gw-libretro*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname gw-libretro*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .mgw" >> ${consoles}

var=$(find /media/sdcard/ROMS/GW/ -type f  -iname \*.mgw  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/GW/ -type f  -iname \*.mgw   | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> ${consoles}
fi
echo "scaling = 1" >> ${consoles}