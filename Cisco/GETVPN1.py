from SetupLab import SetupLab

sl = SetupLab("GET-VPN-Lab")

#First Draw the routers
nks1 = sl.AddNode("KS1", "iosv")
sl.MoveRight(2)
nks2 = sl.AddNode("KS2", "iosv")
sl.MoveDown(1)
sl.MoveLeft(1)
nmpls = sl.AddNode("MPLS","iosvl2")
sl.MoveDown(1)
sl.MoveLeft(1)
nr3 = sl.AddNode("R3","iosv")
sl.MoveRight(2)
nr4 = sl.AddNode("R4","iosv")
sl.MoveDown(1)
nh1 = sl.AddNode("User2","desktop")
sl.MoveLeft(2)
nh2 = sl.AddNode("User1","desktop")

# Make the configurations
KS1 = nks1.Draw()
KS1.config += "ip domain-name testlab\n"
KS1.config += "int Gi0/0\n"
KS1.config += "  ip add 200.1.1.1 255.255.255.0\n"
KS1.config += "ip route 200.1.2.0 255.255.255.0 200.1.1.10\n"
KS1.config += "ip route 200.1.3.0 255.255.255.0 200.1.1.10\n"
KS1.config += "ip route 200.1.4.0 255.255.255.0 200.1.1.10\n"
KS1.config += "line con 0\n"
KS1.config += "  logging syn\n"

KS2 = nks2.Draw()
KS2.config += "ip domain-name testlab\n"
KS2.config += "int Gi0/0\n"
KS2.config += "  ip add 200.1.2.2 255.255.255.0\n"
KS2.config += "ip route 200.1.1.0 255.255.255.0 200.1.2.10\n"
KS2.config += "ip route 200.1.3.0 255.255.255.0 200.1.2.10\n"
KS2.config += "ip route 200.1.4.0 255.255.255.0 200.1.2.10\n"
KS2.config += "line con 0\n"
KS2.config += "  logging syn\n"

R3 = nr3.Draw()
R3.config += "ip domain-name testlab\n"
R3.config += "int Gi0/0\n"
R3.config += "  ip add 200.1.3.3 255.255.255.0\n"
R3.config += "int Gi0/1\n"
R3.config += "  ip add 10.1.2.1 255.255.255.0\n"
R3.config += "ip route 200.1.1.0 255.255.255.0 200.1.3.10\n"
R3.config += "ip route 200.1.2.0 255.255.255.0 200.1.3.10\n"
R3.config += "ip route 200.1.4.0 255.255.255.0 200.1.3.10\n"
R3.config += "line con 0\n"
R3.config += "  logging syn\n"

R4 = nr4.Draw()
R4.config += "ip domain-name testlab\n"
R4.config += "int Gi0/0\n"
R4.config += "  ip add 200.1.4.4 255.255.255.0\n"
R4.config += "int Gi0/1\n"
R4.config += "  ip add 10.1.1.1 255.255.255.0\n"
R4.config += "ip route 200.1.1.0 255.255.255.0 200.1.4.10\n"
R4.config += "ip route 200.1.2.0 255.255.255.0 200.1.4.10\n"
R4.config += "ip route 200.1.3.0 255.255.255.0 200.1.4.10\n"
R4.config += "line con 0\n"
R4.config += "  logging syn\n"

S1 = nmpls.Draw()
S1.config += "ip domain-name testlab\n"
S1.config += "int Gi0/0\n"
S1.config += "  no switchport\n"
S1.config += "  ip addr 200.1.1.10 255.255.255.0\n"
S1.config += "int Gi0/1\n"
S1.config += "  no switchport\n"
S1.config += "  ip addr 200.1.2.10 255.255.255.0\n"
S1.config += "int Gi0/2\n"
S1.config += "  no switchport\n"
S1.config += "  ip addr 200.1.3.10 255.255.255.0\n"
S1.config += "int Gi0/3\n"
S1.config += "  no switchport\n"
S1.config += "  ip addr 200.1.4.10 255.255.255.0\n"
S1.config += "router eigrp 10\n"
S1.config += "  network 200.1.0.0 0.0.255.255\n"
S1.config += "line con 0\n"
S1.config += "  logging syn\n"

H1 = nh1.Draw()
H1.config += "ifconfig eth0 up 10.1.1.100 netmask 255.255.255.0\n"
H1.config += "route add default gw 10.1.1.1 dev eth0\n"

H2 = nh2.Draw()
H2.config += "ifconfig eth0 up 10.1.2.100 netmask 255.255.255.0\n"
H2.config += "route add default gw 10.1.2.1 dev eth0\n"

# Now let's connect everything
KS1_gi0 = KS1.create_interface()
S1_gi0 = S1.create_interface()
sl.lab.create_link(KS1_gi0,S1_gi0)

KS2_gi0 = KS2.create_interface()
S1_gi1 = S1.create_interface()
sl.lab.create_link(KS2_gi0,S1_gi1)

R3_gi0 = R3.create_interface()
S1_gi2 = S1.create_interface()
sl.lab.create_link(R3_gi0,S1_gi2)
R3_gi1 = R3.create_interface()
H2_eth0 = H2.create_interface()
sl.lab.create_link(R3_gi1,H2_eth0)

R4_gi0 = R4.create_interface()
S1_gi3 = S1.create_interface()
sl.lab.create_link(R4_gi0,S1_gi3)
R4_gi1 = R4.create_interface()
H1_eth0 = H1.create_interface()
sl.lab.create_link(R4_gi1,H1_eth0)
