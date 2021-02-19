# lpjobmonitor
Simple program for monitoring CUPS print jobs and GPIO support to turn the printer on and off (OpenWRT, RaspberryPi, etc.)


A simple program for automatic power on / off of the printer via GPIO or LED ports. In my case, instead of the LED, the MOC3041 chip is connected with the BT138 triac.
Every time specified in the configuration file, the program checks whether there are print jobs in the CUPS server. If so, then it executes the commands (in my version three, i.e. loads the USB driver, enables the printer's GPIO port, and changes permissions). After printing, it waits a time specified in the configuration file, and if there are no new print jobs, it turns off the printer (and in my case also the USB module).

System commands to be executed are given in the configuration file. Also waiting times are given in the configuration file. The file must be in / etc / lpjobmonitor-config, and unfortunately the number and sequence of lines are important.

The easiest way to compile the program is by running the cc command. In my case, the program runs on the OPENWRT system compiled in the MIPSEL architecture. Before building the program, you need to download the OpenWrt sources from github, and build the system (unfortunately CUPS also needs to be built "by hand"). After building the system, just issue the commands:

export PATH = $ PATH: /cross-compile/openwrt/staging_dir/toolchain-mipsel_24kc_gcc-7.5.0_musl/bin/

export STAGING_DIR = / cross-compile / openwrt / staging_dir / toolchain-mipsel_24kc_gcc-7.5.0_musl /

/cross-compile/openwrt/staging_dir/toolchain-mipsel_24kc_gcc-7.5.0_musl/bin/mipsel-openwrt-linux-gcc lpjobmonitor.c -o lpjobmonitor


If you are building the program directly on the platform it will be used on, just type:

cc lpjobmonitor.c -o lpjobmonitor

The output file must be placed in the bin, sbin directory with run permission. Then in rc.local or other startup script, depending on the system used, you can add the line:
lpjobmonitor &

Everything should work :)

If the program is helpful, I'd love to hear from you at <jaroslaw.strauchmann@gmail.com>
