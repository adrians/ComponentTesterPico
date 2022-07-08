
Initial setup:
```bash
sudo apt-get install build-essential cmake gcc-arm-none-eabi
cmake .
```

Build:
```bash
make
```

Deploy:
```bash
# Make a temporary folder
mkdir x

# Reboot the board in Bootloader-mode
# Check the name of the block-device
# Mount, deploy the file, unmount
sudo mount /dev/sdb1 x && sudo cp hello_usb.uf2 ./x && sudo umount x
```

Reindent the code:
```bash
indent -kr -nut -brf -l120 hello_usb.c
```

Test:
```bash
cat /dev/ttyACM0
```
