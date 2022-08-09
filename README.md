# iptables-lite
A lightweight firewall for nuttx

## Prerequisites
The first step to get started with NuttX is to install a series of required tools. They can be found <a href="http://nuttx.incubator.apache.org/docs/latest/quickstart/install.html">here</a>.

## Initialize Configuration
The first step is to initialize NuttX configuration for a given board, based from a pre-existing configuration.
```
$ cd nuttx
$ ./tools/configure.sh -l sim:nsh
```
The `-l` tells use that we’re on Linux (macOS and Windows builds are possible). Use the `-h` argument to see all available options.

## Build NuttX
We can now build NuttX. To do so, you can simply run:
```
$ cd nuttx
$ make
```

## Run the Simulator
```
$ ./nuttx
login: admin
password: Administrator
```

## Run the Network Simulator
```
make distclean
./tools/configure.sh -l sim:tcpblaster
make
```
On recent Linux distributions, you need to give the nuttx program the capabilities (similar to permissions) to access the network:

```
sudo setcap cap_net_admin+ep ./nuttx
```
Bring Up the Network Interfaces
On Apache NuttX:
```
nsh> ifup eth0
```
Then, on Linux do this to set up the tap network interface and route that will let the Apache NuttX simulator access the network:
```
sudo ./tools/simhostroute.sh enp0s3 on
```
If you use ifconfig, you'll se that enp0se is the wireless interface. Note that it has an IP address on the local net. There may be other interfaces listed, you’ll need to pick the one that’s right for your system.

## Run the Netcat
```
netcat -l -p 31337
```

## Debugging with GDB
Initiate a nuttx process:
```
./nuttx
```
Collect the process PID:
```
ps aux | grep nuttx
```
Start GDB with this process id:
```
sudo gdb -p {pid}
```
Set breakpoints and continue to debugging:
```
(gdb) break file:lineno
(gdb) continue
```
Up interface eth0:
```
nsh > ifup eth0
```
Ping to linux:
```
nsh > ping -c 1 {ip_linux}
```

Got any trouble? Check the official NuttX quickstart guide:
http://nuttx.incubator.apache.org/docs/latest/quickstart/index.html