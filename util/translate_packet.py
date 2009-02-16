import sys
import time
import array
 
class NovintFalcon():
    def __init__(self):
        self.LEDs = [0,0,0]
        self.Motors = [0,0,0,0]
        self.Buttons = [0,0,0,0]
        self.Axis = [-1,-1,-1]
        self.FalconDevice = None
        self.outputString = [0x3c, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x3e]
 
    def _parseButtons(self, input):
        button_byte = ord(input[13]) - 0x41
        for i in range(4):
            self.Buttons[i] = (button_byte & (0x1 << i)) != 0
 
    def _parsePositions(self, input):
        for i in range(3):
            start_pos = (i * 4) + 1
            self.Axis[i] = 0
            for c in range(start_pos, start_pos+4):
                print (ord(input[c]) - 0x41) #<< ((c-start_pos) * 4)
                self.Axis[i] |= (ord(input[c]) - 0x41) << ((c-start_pos) * 4)
            if self.Axis[i] >= 2**15:
                self.Axis[i] -= 2**16
 
    def printTracedPacket(self, input):
        self._parseButtons(input)
        self._parsePositions(input)
#        print "LEDs:"
#        print self.LEDs
        print "Motors:"
        print self.Axis
        print "Buttons:"
        print self.Buttons

def main(argv=None):
    h = NovintFalcon()
    #str = "3c 4f 49 4b 50 43 41 4b 50 4f 45 4b 50 45 41 3e"
    #str = "3c 4f 4e 41 50 45 43 44 50 41 50 45 50 45 41 3e"
    #str = "3c 50 4e 4e 41 49 47 4e 41 48 47 4d 41 45 41 3e"
    #str = "3c 4e 48 4d 50 42 46 47 50 4d 4b 47 50 45 41 3e"
    #str = "3c 47 4b 44 41 46 46 43 41 50 42 45 41 45 41 3e"
    #str = "3c 48 47 42 41 4a 46 4b 50 4c 50 4a 50 45 41 3e"

    #F: [0, 0, -100]
    #V: [4095, 3906, 4034]
    #str = "3c 50 50 50 41 43 45 50 41 43 4d 50 41 45 41 3e"

    #F: [0, 0, -50]
    #V: [2752, 2780, 2888]
    #str = "3c 41 4d 4b 41 4d 4e 4b 41 49 45 4c 41 45 41 3e"

    #F: [0, 0, -25]
    #V: [1311, 1453, 1526]
    #str = "3c 50 42 46 41 4e 4b 46 41 47 50 46 41 45 41 3e"
    
    #F: [0, 0, -12.5]
    #V: [661, 731, 766]
    #str = "3c 46 4a 43 41 4c 4e 43 41 4f 50 43 41 45 41 3e"

    #F: [0, -12.5, 0]
    #V: [-2900, 27, 929]
    #str = "3c 4d 4b 45 50 4c 42 41 41 42 4b 44 41 45 41 3e"

    #F: [-12.5, 0, 0]
    #V: [399, -3087, 1135]
    #str = "3c 50 49 42 41 42 50 44 50 50 47 45 41 45 41 3e"

    #F: [1.0, 0, 0]
    #V: [-65, 136, -159]
    #str = "3c 50 4c 50 50 49 49 41 41 42 47 50 50 45 41 3e"


    #F: [10.0, 0, 0]
    #V: [-679, 2529, -1862]
    #str = "3c 4a 46 4e 50 42 4f 4a 41 4b 4c 49 50 45 41 3e"

    #F: [0, 10.0, 0]
    #V: [-65, 136, -159]
    str = "3c 42 4c 4a 41 44 47 4e 50 46 47 4a 50 45 41 3e"
    a = [chr(int("0x"+x, 16)) for x in str.split(" ")]
    h.printTracedPacket(a)

    
 
if __name__ == "__main__":
    sys.exit(main())
 
