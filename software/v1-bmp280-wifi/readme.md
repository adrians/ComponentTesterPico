
Initial setup:
```bash
sudo apt-get install build-essential cmake gcc-arm-none-eabi
cmake .
```

Don't forget to update the `WIFI_SSID`, `WIFI_PASSWORD` and the `srv_addr`.

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
sudo mount /dev/sdb1 x && sudo cp bmp280_i2c.uf2 ./x && sudo umount x
```
