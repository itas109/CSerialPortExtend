@ECHO OFF

echo "MinGW compile"

g++ main.cpp ../src/CSerialPortHotSwap.cpp -lpthread -I../src -o CSerialPortHotSwap-MinGW

pause