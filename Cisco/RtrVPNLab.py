import textwrap 
from pathlib import Path
from SetupLab import SetupLab

sl = SetupLab("RtrVPNLab")

#First Draw the routers
nr1 = sl.AddNode("R1","iosv")
sl.MoveDown(1)
ns1 = sl.AddNode("SW1","iosvl2")
sl.MoveDown(1)
sl.MoveLeft(0.5)
nr2 = sl.AddNode("R2","iosv")
sl.MoveRight(1)
nr3 = sl.AddNode("R3","iosv")

# Make the configurations
R1 = nr1.Draw()
R1.config += "int l0\n"
R1.config += "  ip add 1.1.1.1 255.255.255.0\n"
R1.config += "int Gi0/0\n"
R1.config += "  ip add 200.1.1.1 255.255.255.0\n"
R1.config += "ip route 200.1.2.0 255.255.255.0 200.1.1.10\n"
R1.config += "ip route 200.1.3.0 255.255.255.0 200.1.1.10\n"
R1.config += "line con 0\n"
R1.config += "  logging syn\n"

R2 = nr2.Draw()
R2.config += "int l0\n"
R2.config += "  ip add 2.2.2.2 255.255.255.0\n"
R2.config += "int Gi0/0\n"
R2.config += "  ip add 200.1.2.2 255.255.255.0\n"
R2.config += "ip route 200.1.1.0 255.255.255.0 200.1.2.10\n"
R2.config += "ip route 200.1.3.0 255.255.255.0 200.1.2.10\n"
R2.config += "line con 0\n"
R2.config += "  logging syn\n"

R3 = nr3.Draw()
R3.config += "int l0\n"
R3.config += "  ip add 3.3.3.3 255.255.255.0\n"
R3.config += "int Gi0/0\n"
R3.config += "  ip add 200.1.3.3 255.255.255.0\n"
R3.config += "ip route 200.1.1.0 255.255.255.0 200.1.3.10\n"
R3.config += "ip route 200.1.2.0 255.255.255.0 200.1.3.10\n"
R3.config += "line con 0\n"
R3.config += "  logging syn\n"

S1 = ns1.Draw()
S1.config += "int Gi0/0\n"
S1.config += "  no switchport\n"
S1.config += "  ip addr 200.1.1.10 255.255.255.0\n"
S1.config += "int Gi0/1\n"
S1.config += "  no switchport\n"
S1.config += "  ip addr 200.1.2.10 255.255.255.0\n"
S1.config += "int Gi0/2\n"
S1.config += "  no switchport\n"
S1.config += "  ip addr 200.1.3.10 255.255.255.0\n"
S1.config += "line con 0\n"
S1.config += "  logging syn\n"

# Now let's connect everything
R1_gi0 = R1.create_interface()
S1_gi0 = S1.create_interface()
sl.lab.create_link(R1_gi0,S1_gi0)

R2_gi0 = R2.create_interface()
S1_gi1 = S1.create_interface()
sl.lab.create_link(R2_gi0,S1_gi1)

R3_gi0 = R3.create_interface()
S1_gi2 = S1.create_interface()
sl.lab.create_link(R3_gi0,S1_gi2)
