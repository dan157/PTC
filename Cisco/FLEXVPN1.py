from SetupLab import SetupLab

sl = SetupLab("FLEX-VPN-Lab")

#First Draw the Servers at the Data Centers
nsvr1 = sl.AddNode("Server1","server")
sl.MoveRight(2)
nsvr2 = sl.AddNode("Server2","server")
sl.MoveDown(1)
sl.MoveLeft(2)
#First Draw the Data Center routers
ndc1 = sl.AddNode("DC1", "iosv")
sl.MoveRight(2)
ndc2 = sl.AddNode("DC2", "iosv")
sl.MoveDown(1)
sl.MoveLeft(1)
nmpls = sl.AddNode("MPLS","iosv")
sl.MoveDown(1)
sl.MoveLeft(1)
nbr1 = sl.AddNode("BR1","iosv")
sl.MoveRight(2)
nbr2 = sl.AddNode("BR2","iosv")
sl.MoveDown(1)
sl.MoveLeft(2)
nh1 = sl.AddNode("User1","desktop")
sl.MoveRight(2)
nh2 = sl.AddNode("User2","desktop")

# Make the configurations
SVR1 = nsvr1.Draw()
SVR1_eth0 = SVR1.create_interface()
SVR1.config += "ifconfig eth0 up 10.99.35.100 netmask 255.255.255.0\n"
SVR1.config += "route add default gw 10.99.35.1 dev eth0\n"

SVR2 = nsvr2.Draw()
SVR2_eth0 = SVR2.create_interface()
SVR2.config += "ifconfig eth0 up 10.100.35.100 netmask 255.255.255.0\n"
SVR2.config += "route add default gw 10.100.35.1 dev eth0\n"

DC1 = ndc1.Draw()
DC1_gi0 = DC1.create_interface()
sl.lab.create_link(DC1_gi0,SVR1_eth0)
DC1_gi1 = DC1.create_interface()
DC1_gi2 = DC1.create_interface()
DC1.config += "ip domain-name testlab\n"
DC1.config += "int Gi0/0\n"
DC1.config += "  ip add 10.99.35.1 255.255.255.0\n"
DC1.config += "int Gi0/1\n"
DC1.config += "  ip add 172.16.31.1 255.255.255.252\n"
DC1.config += "int Gi0/2\n"
DC1.config += "  ip add 200.1.1.1 255.255.255.0\n"
DC1.config += "line con 0\n"
DC1.config += "  logging syn\n"
DC1.config += "no crypto isakmp default policy\n"
DC1.config += "no crypto ipsec profile default\n"
DC1.config += "no ip http server\n"
DC1.config += "no ip http secure-server\n"
DC1.config += "clock timezone HST -10\n"
DC1.config += "router eigrp 10\n"
DC1.config += "  network 200.1.1.1 0.0.0.0\n"
DC1.config += "  passive-interface default\n"
DC1.config += "  no passive-interface gi0/2\n"

DC2 = ndc2.Draw()
DC2_gi0 = DC2.create_interface()
sl.lab.create_link(DC2_gi0,SVR2_eth0)
DC2_gi1 = DC2.create_interface()
sl.lab.create_link(DC1_gi1,DC2_gi1)
DC2_gi2 = DC2.create_interface()
DC2.config += "ip domain-name testlab\n"
DC2.config += "int Gi0/0\n"
DC2.config += "  ip add 10.100.35.1 255.255.255.0\n"
DC2.config += "int Gi0/1\n"
DC2.config += "  ip add 172.16.31.2 255.255.255.252\n"
DC2.config += "int Gi0/2\n"
DC2.config += "  ip add 200.1.2.2 255.255.255.0\n"
DC2.config += "line con 0\n"
DC2.config += "  logging syn\n"
DC2.config += "no crypto isakmp default policy\n"
DC2.config += "no crypto ipsec profile default\n"
DC2.config += "no ip http server\n"
DC2.config += "no ip http secure-server\n"
DC2.config += "clock timezone HST -10\n"
DC2.config += "router eigrp 10\n"
DC2.config += "  network 200.1.2.2 0.0.0.0\n"
DC2.config += "  passive-interface default\n"
DC2.config += "  no passive-interface gi0/2\n"

BR1 = nbr1.Draw()
BR1_gi0 = BR1.create_interface()
BR1_gi1 = BR1.create_interface()
BR1.config += "ip domain-name branch25m.testlab\n"
BR1.config += "int Gi0/0\n"
BR1.config += "  ip add 200.1.3.3 255.255.255.0\n"
BR1.config += "int Gi0/1\n"
BR1.config += "  ip add 10.112.1.1 255.255.255.0\n"
BR1.config += "line con 0\n"
BR1.config += "  logging syn\n"
BR1.config += "no crypto isakmp default policy\n"
BR1.config += "no crypto ipsec profile default\n"
BR1.config += "no ip http server\n"
BR1.config += "no ip http secure-server\n"
BR1.config += "clock timezone HST -10\n"
BR1.config += "router eigrp 10\n"
BR1.config += "  network 200.1.3.3 0.0.0.0\n"
BR1.config += "  passive-interface default\n"
BR1.config += "  no passive-interface gi0/0\n"
BR1.config += "event manager applet FixNHRPBug authorization bypass\n"
BR1.config += " event syslog pattern \"%NHRP-5-NO_ROUTE: Not installing NHO\"\n"
BR1.config += " action 1.0 cli command \"enable\"\n"
BR1.config += " action 2.0 cli command \"clear ip nhrp shortcut interface tu200\"\n"
BR1.config += " action 4.0 cli command \"end\"\n"

BR2 = nbr2.Draw()
BR2_gi0 = BR2.create_interface()
BR2_gi1 = BR2.create_interface()
BR2.config += "ip domain-name branch10m.testlab\n"
BR2.config += "int Gi0/0\n"
BR2.config += "  ip add 200.1.4.4 255.255.255.0\n"
BR2.config += "int Gi0/1\n"
BR2.config += "  ip add 10.112.2.1 255.255.255.0\n"
BR2.config += "line con 0\n"
BR2.config += "  logging syn\n"
BR2.config += "no crypto isakmp default policy\n"
BR2.config += "no crypto ipsec profile default\n"
BR2.config += "no ip http server\n"
BR2.config += "no ip http secure-server\n"
BR2.config += "clock timezone HST -10\n"
BR2.config += "router eigrp 10\n"
BR2.config += "  network 200.1.4.4 0.0.0.0\n"
BR2.config += "  passive-interface default\n"
BR2.config += "  no passive-interface gi0/0\n"
BR2.config += "event manager applet FixNHRPBug authorization bypass\n"
BR2.config += " event syslog pattern \"%NHRP-5-NO_ROUTE: Not installing NHO\"\n"
BR2.config += " action 1.0 cli command \"enable\"\n"
BR2.config += " action 2.0 cli command \"clear ip nhrp shortcut interface tu200\"\n"
BR2.config += " action 4.0 cli command \"end\"\n"

MPLS1 = nmpls.Draw()
M1_gi0 = MPLS1.create_interface()
sl.lab.create_link(DC1_gi2,M1_gi0)
M1_gi1 = MPLS1.create_interface()
sl.lab.create_link(DC2_gi2,M1_gi1)
M1_gi2 = MPLS1.create_interface()
sl.lab.create_link(BR1_gi0,M1_gi2)
M1_gi3 = MPLS1.create_interface()
sl.lab.create_link(BR2_gi0,M1_gi3)
MPLS1.config += "ip domain-name testlab\n"
MPLS1.config += "ip host dc1.testlab 200.1.1.1\n"
MPLS1.config += "ip host dc2.testlab 200.1.2.2\n"
MPLS1.config += "ip host testlab ns ns1.testlab\n"
MPLS1.config += "ip host ns1.testlab 1.1.1.1\n"
MPLS1.config += "ip host br1.testlab 200.1.3.3\n"
MPLS1.config += "ip host br2.testlab 200.1.4.4\n"
MPLS1.config += "ip host server1.testlab 10.99.35.100\n"
MPLS1.config += "ip host server2.testlab 10.100.35.100\n"
MPLS1.config += "ip host user1.testlab 10.112.1.100\n"
MPLS1.config += "ip host user2.testlab 10.112.2.100\n"
MPLS1.config += "ip host ca1.testlab 192.0.2.1\n"
MPLS1.config += "int Gi0/0\n"
MPLS1.config += "  ip addr 200.1.1.10 255.255.255.0\n"
MPLS1.config += "int Gi0/1\n"
MPLS1.config += "  ip addr 200.1.2.10 255.255.255.0\n"
MPLS1.config += "int Gi0/2\n"
MPLS1.config += "  ip addr 200.1.3.10 255.255.255.0\n"
MPLS1.config += "int Gi0/3\n"
MPLS1.config += "  ip addr 200.1.4.10 255.255.255.0\n"
MPLS1.config += "int lo50\n"
MPLS1.config += "  ip addr 192.0.2.1 255.255.255.255\n"
MPLS1.config += "router eigrp 10\n"
MPLS1.config += "  passive-interface lo50\n"
MPLS1.config += "  network 192.0.2.1 0.0.0.0\n"
MPLS1.config += "  network 200.1.0.0 0.0.255.255\n"
MPLS1.config += "no ip http secure-server\n"
MPLS1.config += "ip dns server\n"
MPLS1.config += "ip dns primary testlab soa ns1.testlab admin 21600 900 7776000 86400\n"
MPLS1.config += "line con 0\n"
MPLS1.config += "  logging syn\n"
MPLS1.config += "clock timezone HST -10\n"
MPLS1.config += "crypto key gen rsa gen label ca1.testlab mod 2048 exportable\n"
MPLS1.config += "ip http server\n"
MPLS1.config += "crypto pki server ca1.testlab\n"
MPLS1.config += "  database url nvram:\n"
MPLS1.config += "  database level minimum\n"
MPLS1.config += "  issuer-name CN=ca1.testlab L=HNL C=US\n"
MPLS1.config += "  lifetime ca-certificate 365\n"
MPLS1.config += "  lifetime certificate 200\n"
MPLS1.config += "  lifetime crl 24\n"
MPLS1.config += "  cdp-url http://ca1.testlab/ca1cdp.testlab.crl\n"
MPLS1.config += "  grant auto\n"
MPLS1.config += "  auto-rollover 15\n"
MPLS1.config += "  hash sha512\n"
MPLS1.config += "crypto pki trustpoint ca1.testlab\n"
MPLS1.config += "  revocation-check crl\n"
MPLS1.config += "  rsakeypair ca1.testlab\n"

H1 = nh1.Draw()
H1_eth0 = H1.create_interface()
sl.lab.create_link(H1_eth0,BR1_gi1)
H1.config += "ifconfig eth0 up 10.112.1.100 netmask 255.255.255.0\n"
H1.config += "route add default gw 10.112.1.1 dev eth0\n"

H2 = nh2.Draw()
H2_eth0 = H2.create_interface()
sl.lab.create_link(H2_eth0,BR2_gi1)
H2.config += "ifconfig eth0 up 10.112.2.100 netmask 255.255.255.0\n"
H2.config += "route add default gw 10.112.2.1 dev eth0\n"
