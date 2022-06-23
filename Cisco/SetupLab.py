from virl2_client import ClientLibrary

class NodeInfo:
    def __init__(self,lab):
        self.lab = lab
        self.curX = 0
        self.curY = 0
        self.nodeName = ""
        self.imageName = ""


    def SetupNode(self,nName,iName,x,y):
        self.nodeName = nName
        self.imageName = iName
        self.curY = y
        self.curX = x
    
    def Draw(self):
        self.node = self.lab.create_node(self.nodeName,self.imageName,self.curX,self.curY)
        self.node.config = "hostname " + self.nodeName + "\n"
        return self.node

class SetupLab:
    def __init__(self,labName):
        self.startX = 0
        self.startY = 0
        self.XAdd = 100
        self.YAdd = 100
        self.XCurr = 0
        self.YCurr = 0
        self.numNodes = 0
        self.client = ClientLibrary("https://192.168.250.35", "admin", "Cisco123!", ssl_verify=False)
        self.nodes = []
    
        self.lab = self.client.create_lab(labName)

    def AddNode(self,nName,iName):
        ni = NodeInfo(self.lab)
        self.nodes.append(ni)
        ni.SetupNode(nName,iName,self.XCurr,self.YCurr)
        return ni
    
    def CheckY(self):
        # if the Y value is below zero we need to offset all nodes
        if self.YCurr < 0:
            m_y = self.YCurr * -1
            self.YCurr = 0
            for i in self.nodes:
                i.curY += m_y

    def CheckX(self):
        # if the X value is below zero we need to offset all nodes
        if self.XCurr < 0:
            m_x = self.XCurr * -1
            self.XCurr = 0
            for i in self.nodes:
                i.curX += m_x

    def MoveUp(self,numPos):
        self.YCurr -= int(self.YAdd * numPos)
        self.CheckY()

    def MoveDown(self,numPos):
        self.YCurr += int(self.YAdd * numPos)

    def MoveLeft(self,numPos):
        self.XCurr -= int(self.XAdd * numPos)
        self.CheckX()

    def MoveRight(self,numPos):
        self.XCurr += int(self.XAdd * numPos)
