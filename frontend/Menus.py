import pygame
from option import *
from Action import *
class Menu:
    def __init__(self, pyview):
        self.pyview = pyview
        self.opWheel = OptionWheel(pyview)
    def draw(self):
        self.opWheel.draw()
    def right_button(self):
        self.opWheel.scroll_right()
    def down_button(self):
        self.opWheel.select()
        self.pyview.change_menu(self.opWheel.options[self.opWheel.selection].action)
    def left_button(self):
        self.opWheel.scroll_left()
        
class Main_Menu(Menu):
    def __init__(self, pyview):
        super().__init__(pyview)
        #Set Background Image
        pyview.background = pygame.image.load("Stage.jpg")
        #Set Wheel Contents
        #self.opWheel.append_option("LionHeadElectric.png","My Cryptomon",Action.GO_TO_INTERACT_MENU)
        self.opWheel.append_option("LionHeadGrass.png","Manage Cryptomon",Action.GO_TO_SELECT_MON_MENU)
        self.opWheel.append_option("LionHeadNormal.png","Manage Friends",Action.GO_TO_FRIENDS_MENU)
        self.opWheel.append_option("LionHeadPoison.png","Exit App",Action.EXIT_APP)
        
class Select_Mon_Menu(Menu):
    def __init__(self, pyview):
        self.pyview = pyview
        self.opWheel = OptionWheel(pyview)
        #Set Background Image
        pyview.background = pygame.image.load("Stage.jpg")
        #Set Wheel Contents
        self.opWheel.append_option("LionHeadElectric.png","Buy Cryptomon",Action.GO_TO_MARKET_MENU)
        #List Cryptomon
        #self.opWheel.append_option("LionHeadGrass.png","Cryptomon Name Here",Action.OPEN_SELECT_MON_SUBMENU)
            #self.opWheel.append_option("LionHeadNormal.png","Cryptomon Name Here",Action.GO_TO_INTERACT_MENU)
            #self.opWheel.append_option("LionHeadNormal.png","Deliver Cryptomon Name Here",Action.OPEN_SELECT_MON_DISPOSE_SUBMENU)
            #self.opWheel.append_option("LionHeadNormal.png","Cancel",100)
        self.opWheel.append_option("LionHeadNormal.png","Return to Main Menu",Action.GO_TO_MAIN_MENU)

class Food_Menu(Menu):
    def __init__(self, pyview):
        super().__init__(pyview)
        #Set Background Image
        pyview.background = pygame.image.load("Stage.jpg")
        #Set Wheel Contents
        #Display all food options
            #self.opWheel.append_option("LionHeadNormal.png","Cryptomon Name Here",Action.GO_TO_INTERACT_MENU)
            #self.opWheel.append_option("LionHeadNormal.png","Buy",100)
            #self.opWheel.append_option("LionHeadNormal.png","Sell",100)
        self.opWheel.append_option("LionHeadNormal.png","Return to Cryptomon Name Here",Action.GO_TO_INTERACT_MENU)

class Interact_Menu(Menu):
    def __init__(self, pyview):
        super().__init__(pyview)
        #Set Background Image
        pyview.background = pygame.image.load("Stage.jpg")
        #Set Wheel Contents
        self.opWheel.append_option("LionHeadNormal.png","Play",Action.GO_TO_PLAY_MENU)
        self.opWheel.append_option("LionHeadNormal.png","Clean Pen",100)
        self.opWheel.append_option("LionHeadNormal.png","Feed Cryptomon",100)
            #self.opWheel.append_option("LionHeadNormal.png","Select Feed",100)
            #self.opWheel.append_option("LionHeadNormal.png","Cancel",100)
        self.opWheel.append_option("LionHeadNormal.png","Manage Cryptomon",Action.GO_TO_INTERACT_MENU)

class Market_Menu(Menu):
    def __init__(self, pyview):
        super().__init__(pyview)
        #Set Background Image
        pyview.background = pygame.image.load("Stage.jpg")
        #Set Wheel Contents
        self.opWheel.append_option("LionHeadNormal.png","Return to Manage Cryptomon",Action.GO_TO_SELECT_MON_MENU)
        #Display Page From Market
            #self.opWheel.append_option("LionHeadNormal.png","Buy Cryptomon",100)
            #self.opWheel.append_option("LionHeadNormal.png","Cancel",100)
        self.opWheel.append_option("LionHeadNormal.png","Refresh Shop Page",100)

class Friends_Menu(Menu):
    def __init__(self, pyview):
        super().__init__(pyview)
        #Set Background Image
        pyview.background = pygame.image.load("Stage.jpg")
        #Set Wheel Contents
        self.opWheel.append_option("LionHeadNormal.png","New Friend",100)
        #If Player Has Friend(s)
            #self.opWheel.append_option("LionHeadNormal.png","Delete Friend",100)
            #self.opWheel.append_option("LionHeadNormal.png","Cancel",100)
        self.opWheel.append_option("LionHeadNormal.png","Return to Main Menu",Action.GO_TO_MAIN_MENU)

class Trade_Menu(Menu):
    def __init__(self, pyview):
        super().__init__(pyview)
        #Set Background Image
        pyview.background = pygame.image.load("Stage.jpg")
        #Set Wheel Contents
        """check if trade deal has been completed
        Step 1: Mon A offered by party 1 to friend
        Step 2: Mon B offered by party 2 for Mon A
        Option apears to party 1 to select or deny trade"""
        #Show list of mons offered by friends to you, if any
        #Show list of friends, if any
        self.opWheel.append_option("LionHeadNormal.png","Return to Main Menu",Action.GO_TO_MAIN_MENU)

        
class Play_Menu(Menu):
    def __init__(self, pyview):
        super().__init__(pyview)
        #Set Background Image
        pyview.background = pygame.image.load("Stage.jpg")
        #Set Wheel Contents
        self.opWheel.append_option("LionHeadNormal.png","Stop Playing",Action.GO_TO_INTERACT_MENU)


