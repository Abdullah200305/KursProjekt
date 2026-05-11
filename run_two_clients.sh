#!/usr/bin/env bash

echo "=============================="
echo " Building Tick Tock Bomb server"
echo "=============================="

mingw32-make -C Server clean
mingw32-make -C Server

if [ $? -ne 0 ]; then
    echo ""
    echo "Server build failed. Fix errors before running."
    read -p "Press Enter to exit..."
    exit 1
fi

echo ""
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
echo " Starting two clients"
echo "=============================="

(cd client && ./client.exe) &
sleep 1
(cd client && ./client.exe) &

echo ""
echo "Two clients started."
echo "Client 1: START -> HOST GAME"
echo "Client 2: START -> JOIN GAME"
echo "Then click in Host window to start match."