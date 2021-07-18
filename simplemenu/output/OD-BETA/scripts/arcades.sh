consoles="/media/data/local/home/.simplemenu/section_groups/arcades.ini"

echo "[CONSOLES]" > "${consoles}"
echo "consoleList = MAME,FINALBURN ALPHA,DAPHNE" >> "${consoles}"

echo "" >> "${consoles}"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [MAME] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname mame4all*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname mame4all*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .zip" >> ${consoles}
var=$(find /media/sdcard/ROMS/MAME/ -type f -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/MAME/ -type f -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> ${consoles}
fi
echo "aliasFile = /media/data/local/home/.simplemenu/alias.txt" >> ${consoles}
echo "onlyFileNamesNoPathOrExtension = no" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [FINALBURN ALPHA] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname fba*.opk | head -n 1)
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
var=$(find /media/sdcard/ROMS/FBA/ -type f -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/FBA/ -type f -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> ${consoles}
fi
echo "aliasFile = /media/data/local//home/.simplemenu/alias.txt" >> ${consoles}
echo "onlyFileNamesNoPathOrExtension = no" >> ${consoles}
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "" >> "$consoles"

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
echo [DAPHNE] >> "${consoles}"
var=$(find /media/sdcard/APPS/ -type f -iname daphne*.opk | head -n 1)
if [ -z "$var" ]
then
	var=$(find /media/data/apps/ -type f -iname daphne*.opk | head -n 1)
fi
if [ -z "$var" ]
then
	var=none
fi
echo "execs = ""$var" >> ${consoles}
echo "romExts = .zip" >> ${consoles}
var=$(find /media/sdcard/ROMS/DAPHNE/ -type f -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
if [ -z "$var" ]
then
	var=$(find /media/data/ROMS/DAPHNE/ -type f -iname \*.zip  | head -n 1 | rev | cut -d"/" -f2-  | rev)
fi
if [ -z "$var" ]	
then
	var=none
	echo "romDirs = ""$var" >> ${consoles}
else
	echo "romDirs = ""$var""/" >> ${consoles}
fi
echo "aliasFile = /media/data/local/home/.simplemenu/alias.txt" >> ${consoles}
echo "onlyFileNamesNoPathOrExtension = no" >> ${consoles}
echo "scaling = 0" >> ${consoles}