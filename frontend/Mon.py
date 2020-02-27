import pygame

path = "/home/pi/Desktop/Pygame/Cryptomon/cryptomon/assets/"

class Mon:
    def __init__(self,pyview,head_nature,body_nature = None,head = None,body = None):
        if body_nature == None :
            head_nature,body_nature,head,body = self.decode(head_nature)
        self.head_nature = head_nature #Ex "Electric"
        self.body_nature = body_nature #Ex "Normal"
        self.head = head #Ex: "Lion"
        self.body = body #Ex: "Bull"
        self.head_image_name = path + head + "Head" + head_nature + ".png"
        self.body_image_name = path + body + "Body" + body_nature + ".png"
        self.head_image = pygame.image.load(self.head_image_name)
        self.body_image = pygame.image.load(self.body_image_name)
        self.x_size, self.y_size = self.head_image.get_rect().size
        self.name = "Suzy"

    def decode(self,code):
        head_nature, body_nature = self.decode_nature(code)
        head = self.decode_head(code)
        body = self.decode_body(code)
        return head_nature,body_nature,head,body

    def decode_nature(self,code):
        #0bZZ XX XX XX
        h_code = code//64
        #0bXX ZZ XX XX
        b_code = (code % 64)//16
        #0bZZ
        if(h_code == 3):
            head = "Poison"
        elif(h_code == 2):
            head = "Electric"
        elif(h_code == 1):
            head = "Grass"
        else:
            head = "Normal"
        if(b_code == 3):
            return head , "Poison"
        elif(b_code == 2):
            return head , "Electric"
        elif(b_code == 1):
            return head , "Grass"
        else:
            return head , "Normal"

    def decode_head(self,code):
        #0bXX XX ZZ XX
        code = (code % 16)//4
        #0bZZ
        if(code == 3):
            return "Panda"
        elif(code == 2):
            return "Bull"
        elif(code == 1):
            return "Bear"
        else:
            return "Lion"

    def decode_body(self,code):
        #0bXX XX XX ZZ
        code = (code % 4)
        #0bZZ
        if(code == 3):
            return "Panda"
        elif(code == 2):
            return "Bull"
        elif(code == 1):
            return "Bear"
        else:
            return "Lion"
