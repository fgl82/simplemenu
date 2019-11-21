#!/bin/sh

OPK_NAME='simplemenu.opk'

echo ${OPK_NAME}

# create default.gcw0.desktop
cat > default.gcw0.desktop <<EOF
[Desktop Entry]
Name=SimpleMenu
Comment=A sensible launcher
Exec=simplemenu.dge
Terminal=false
Type=Application
StartupNotify=false
Icon=smenu
Categories=applications;
EOF

# create opk
FLIST="simplemenu.dge"
FLIST="${FLIST} invoker.dge"
FLIST="${FLIST} usb_mode_on.sh"
FLIST="${FLIST} usb_mode_off.sh"
FLIST="${FLIST} resources"
FLIST="${FLIST} config"
FLIST="${FLIST} default.gcw0.desktop"
FLIST="${FLIST} smenu.png"

rm -f ${OPK_NAME}
mksquashfs ${FLIST} ${OPK_NAME} -all-root -no-xattrs -noappend -no-exports

cat default.gcw0.desktop
rm -f default.gcw0.desktop
