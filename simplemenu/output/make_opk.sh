#!/bin/sh

OPK_NAME='SimpleMenu.opk'

echo ${OPK_NAME}

# create default.gcw0.desktop
cat > default.gcw0.desktop <<EOF
[Desktop Entry]
Name=SimpleMenu
Comment=A simple launcher
Exec=simplemenu
Terminal=false
Type=Application
StartupNotify=false
Icon=simplemenu
Categories=applications;
EOF

# create opk
chmod 777 simplemenu
FLIST="simplemenu"
FLIST="${FLIST} invoker.dge"
FLIST="${FLIST} resources"
FLIST="${FLIST} scripts"
FLIST="${FLIST} config"
FLIST="${FLIST} apps"
FLIST="${FLIST} default.gcw0.desktop"
FLIST="${FLIST} simplemenu.png"

rm -f ${OPK_NAME}
mksquashfs ${FLIST} ${OPK_NAME} -all-root -no-xattrs -noappend -no-exports

cat default.gcw0.desktop
rm -f default.gcw0.desktop
