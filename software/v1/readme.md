
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

Test:
```bash
cat /dev/ttyACM0
```
