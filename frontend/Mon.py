import pygame

class Mon:
    def __init__(self,pyview,head_nature,body_nature = None,head = None,body = None, name = None):
        if body_nature == None :
            mon = head_nature
            self.name = mon['mon_name']
            self.head_nature,self.body_nature = self.decode_nature(mon['type'])
            self.head = self.decode_race(mon['head'])
            self.body = self.decode_race(mon['torso'])
            self.index = mon['key']
            self.happiness = mon['happiness']
            self.hunger = mon['hunger']
            self.date_aquired = ['start']
            self.date_last_interacted = ['current']
        else:
            self.name = name
            self.head_nature = head_nature
            self.body_nature = body_nature
            self.head = head
            self.body = body
            self.index = None
            self.happiness = None
            self.hunger = None
            self.date_aquired = None
            self.date_last_interacted = None
        self.head_image_name = pyview.path + self.head + "Head" + self.head_nature + ".png"
        self.body_image_name = pyview.path + self.body + "Body" + self.body_nature + ".png"
        self.head_image = pygame.image.load(self.head_image_name)
        self.body_image = pygame.image.load(self.body_image_name)
        self.x_size, self.y_size = self.head_image.get_rect().size

    def decode_nature(self,code):
        #0bZZ XX
        h_code = code//4
        #0bXX ZZ
        b_code = (code % 4)//4
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

    def decode_race(self,code):
        if(code == 3):
            return "Panda"
        elif(code == 2):
            return "Bull"
        elif(code == 1):
            return "Lion"
        else:
            return "Bear"
