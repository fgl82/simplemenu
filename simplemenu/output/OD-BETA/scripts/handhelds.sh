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
var=$(find /media/sdcard/ROMS/NGP/ -type f  -iname \*.ngp -o -iname \*.zip -o -iname \*.ngf  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/NGP/ -type f  -iname \*.ngp -o -iname \*.zip -o -iname \*.ngf  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/"  >> "$consoles"
fi
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
var=$(find /media/sdcard/ROMS/NGPC/ -type f  -iname \*.zip -o -iname \*.ngc  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/NGPC/ -type f  -iname \*.zip -o -iname \*.ngc  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> "$consoles"
fi
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#,WONDERSWAN,GAME & WATCH" >> "${consoles}"
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

var=$(find /media/sdcard/ROMS/LYNX/ -type f  -iname \*.lnx  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/sdcard/ROMS/LYNX/ -type f  -iname \*.lnx   | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/PS/ -type f  -iname \*.lnx   | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/PSX/ -type f  -iname \*.lnx   | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> ${consoles}
fi
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

var=$(find /media/sdcard/ROMS/7800/ -type f  -iname \*.zip -o -iname \*.bin -o -iname \*.rom  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/7800/ -type f  -iname \*.zip -o -iname \*.bin -o -iname \*.rom  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> ${consoles}
fi
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#,ATARI 7800