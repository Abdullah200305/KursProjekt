#!/usr/bin/env bash

echo "=============================="
echo " Building Tick Tock Bomb client"
echo "=============================="

mingw32-make -C client clean
mingw32-make -C client

if [ $? -ne 0 ]; then
    echo ""
    echo "Client build failed. Fix errors before running."
    read -p "Press Enter to exit..."
    exit 1
fi

echo ""
echo "=============================="
echo " Starting client"
echo "=============================="

cd client
./client.exe