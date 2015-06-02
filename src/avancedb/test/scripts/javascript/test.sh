mocha --version > /dev/null
if [ $? -eq 127 ]; then
    echo 'You should install node/npm/mocha before running this test'
    exit 1
fi

if [ ! -d "node_modules" ]; then
    npm install
fi

CONFIGURATION=$1
if [ "$CONFIGURATION" == "" ]; then
    CONFIGURATION=Debug
fi

pushd ../../../dist/${CONFIGURATION}/GNU-Linux-x86
./avancedb -p 15994 &
ADB_PID=$!
sleep 2
popd
mocha
sleep 2
kill $ADB_PID
