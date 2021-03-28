#!/bin/sh

OPK_NAME="SimpleMenu-${1}.opk"

echo ${OPK_NAME}

if [ "$1" = "OD" ] || [ "$1" = "PG2" ] || [ "$1" = "OD-BETA" ]; then
    var="gcw0";
else
    var="retrofw";
fi

if [ "$1" = "OD-BETA" ]; then
    user_name="od";
else
    user_name="root";
fi

cd /home/bittboy/git/simplemenu/simplemenu/

if [ "$1" = "OD" ]; then
    make clean
    make PLATFORM=OD
elif [ "$1" = "OD-BETA" ]; then
    make clean
    make PLATFORM=OD-BETA
elif [ "$1" = "PG2" ]; then
    make clean
    make PLATFORM=NPG
elif [ "$1" = "RFW" ]; then
    make clean
    make PLATFORM=RG-300
fi

cd /home/bittboy/git/invoker/invoker/

if [ "$1" = "OD" ]; then
    make clean
    make PLATFORM=OD
elif [ "$1" = "OD-BETA" ]; then
    make clean
    make PLATFORM=OD-BETA
elif [ "$1" = "PG2" ]; then
    make clean
    make PLATFORM=NPG
else
    make clean
    make PLATFORM=RG-300
fi

cp invoker.dge /home/bittboy/git/simplemenu/simplemenu/output

if [ "$1" = "OD" ] || [ "$1" = "PG2" ] || [ "$1" = "OD-BETA" ]; then
    var="gcw0"
fi

if [ "$1" = "RFW" ]]; then
    var="retrofw"
fi

cd /home/bittboy/git/simplemenu/simplemenu/output

cat>default.${var}.desktop<<EOF
[Desktop Entry]
Name=SimpleMenu
Comment=A simple launcher
Exec=simplemenu
Terminal=true
Type=Application
StartupNotify=true
Icon=simplemenu
Categories=applications;
EOF

#copy the right config
cp $1/config-$2/* $1/config

# create opk
FLIST="$1/apps"
FLIST="${FLIST} $1/config"
FLIST="${FLIST} $1/games"
FLIST="${FLIST} $1/scripts"
FLIST="${FLIST} $1/section_groups"
FLIST="${FLIST} $1/themes"
FLIST="${FLIST} resources"
FLIST="${FLIST} invoker.dge"
FLIST="${FLIST} simplemenu"
FLIST="${FLIST} usb.png"
FLIST="${FLIST} simplemenu.png"
if [ "$1" = 'OD' ] || [ "$1" = 'PG2' ] || [ "$1" = "OD-BETA" ]; then
    FLIST="${FLIST} default.gcw0.desktop"
else
    FLIST="${FLIST} default.retrofw.desktop"
fi

rm -f ${OPK_NAME} > log.txt
mksquashfs ${FLIST} ${OPK_NAME} -all-root -no-xattrs -noappend -no-exports >> log.txt

if [ "$1" = 'OD' ] || [ "$1" = 'PG2' ] || [ "$1" = 'OD-BETA' ]; then
    cat default.gcw0.desktop >> log.txt
    rm -f default.gcw0.desktop >> log.txt
    while true; do
        read -p "Transfer?" yn
        case $yn in
            [Yy]* ) scp SimpleMenu-${1}.opk $user_name@10.1.1.2:/media/sdcard/apps/SimpleMenu-${1}.opk; break;;
            [Nn]* ) exit;;
            * ) echo "Please answer yes or no.";;
        esac
    done
else
    cat default.retrofw.desktop >> log.txt
    rm -f default.retrofw.desktop >> log.txt
fi
