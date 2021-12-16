import textwrap 
from pathlib import Path
from virl2_client import ClientLibrary

exec(compile(source=textwrap.dedent(open('SetupLab.py').read()), filename='SetupLab.py', mode='exec'))

client = ClientLibrary("https://192.168.250.35", "admin", "Cisco123!", ssl_verify=False)

sl = SetupLab("CCNA-Lab-1")

#First Draw the routers
nr1 = sl.AddNode("R1","iosv")
sl.MoveRight(1)
nfw1 = sl.AddNode("ASA1","asav")
sl.MoveRight(1)
ns1 = sl.AddNode("SW1","iosvl2")
sl.MoveDown(1)
nh1 = sl.AddNode("H1","desktop")


# Make the configurations
R1 = nr1.Draw()
FW1 = nfw1.Draw()
S1 = ns1.Draw()
H1 = nh1.Draw()

# Create the interfaces
R1_gi0 = R1.create_interface()
FW1_m0 = FW1.create_interface()
FW1_gi0 = FW1.create_interface()
FW1_gi1 = FW1.create_interface()
S1_gi0 = S1.create_interface()
S1_gi1 = S1.create_interface()
H1_eth0 = H1.create_interface()

# Link the devices to each other as needed
sl.lab.create_link(FW1_gi0,R1_gi0)
sl.lab.create_link(FW1_gi1,S1_gi0)
sl.lab.create_link(S1_gi1, H1_eth0)

# Knowing how to setup the IP address on a Linux system is not part of this lab, 
# so it will be pre-configured to assist the user
H1.config += "ifconfig eth0 up 192.168.1.50 netmask 255.255.255.0\n"
H1.config += "route add default gw 192.168.1.1 dev eth0\n"
