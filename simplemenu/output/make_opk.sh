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
FLIST="apps"
FLIST="${FLIST} config"
FLIST="${FLIST} scripts"
FLIST="${FLIST} section_groups"
FLIST="${FLIST} themes"
FLIST="${FLIST} invoker.dge"
FLIST="${FLIST} simplemenu"
FLIST="${FLIST} simplemenu.png"
FLIST="${FLIST} default.gcw0.desktop"

rm -f ${OPK_NAME}
mksquashfs ${FLIST} ${OPK_NAME} -all-root -no-xattrs -noappend -no-exports

cat default.gcw0.desktop
rm -f default.gcw0.desktop
