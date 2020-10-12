Congratulations on finding an easter egg: Kevin has implemented a small program for finding computer serial devices. The usage is as follows

```bash
sudo apt-get install ros-${your-ros-version}-serial
sudo cp lsserial /usr/bin
```

Then you can use the command `lsserial`  to check serial ports on your computer.

```bash
(base) ➜  ~ lsserial
No serial port available.
(base) ➜  ~ lsserial
(/dev/ttyUSB0,  USB2.0-Serial , USB VID:PID=1a86:7523 )
(base) ➜  ~ 

```

