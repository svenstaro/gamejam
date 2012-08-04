TARGET=$1

GAME="game.love"
ROOTDIR=$(pwd)

if ! [ -f $ROOTDIR/$GAME ]; then
    echo "Game not found in $GAME - please run \"make build\" first."
    exit 1
fi

if ! [[ $TARGET =~ ^(linux|windows)_x(86|64)$ ]]; then
    echo "Invalid target: use one of (linux|windows)_x(86|64)"
    exit 1
fi

echo -n "Packaging for $TARGET... "

PKGDIR=pkg/$TARGET
LIBDIR=lib/$TARGET
BINARY=$(cd $LIBDIR && ls love*)
NAME=$(lua lib/get_title.lua)
ext=."${BINARY##*.}"
if [[ $ext == ".love" ]]; then ext=""; fi

# prepare directory
mkdir -p $PKGDIR

cp $LIBDIR/* $PKGDIR

cd $PKGDIR
cat $ROOTDIR/game.love >> "$BINARY"
mv "$BINARY" $NAME$ext

echo "DONE"
