import pygame
from option import *
from Action import *
class Main_Menu:
    def __init__(self, pyview):
        self.pyview = pyview
        pyview.background = pygame.image.load("Stage.jpg")
        self.opWheel = OptionWheel(pyview)
        self.opWheel.append_option("LionHeadElectric.png","My Mon",Action.GO_TO_INTERACT_MENU)
        self.opWheel.append_option("LionHeadGrass.png","Manage Mon",Action.GO_TO_SELECT_MON_MENU)
        self.opWheel.append_option("LionHeadNormal.png","Manage Friends",Action.GO_TO_FRIENDS_MENU)
        self.opWheel.append_option("LionHeadPoison.png","Exit App",Action.EXIT_APP)
    def draw(self):
        self.opWheel.draw()
    def right_button(self):
        self.opWheel.scroll_right()
    def down_button(self):
        self.opWheel.select()
    def left_button(self):
        self.opWheel.scroll_left()
        
class Select_Mon_Menu:
    def __init__(self, pyview):
        pyview.background = pygame.image.load("Stage.jpg")
        self.opWheel = OptionWheel(pyview)
        #Buy New Mon
        self.opWheel.append_option("LionHeadElectric.png","Buy Mon",Action.GO_TO_MARKET_MENU)
        #Iterate through list of options and if selected
        #Consult sub menu
        self.opWheel.append_option("LionHeadGrass.png","Mon",Action.OPEN_SELECT_MON_SUBMENU)
        self.opWheel.append_option("LionHeadNormal.png","Return to Main Menu",Action.GO_TO_MAIN_MENU)
    def draw(self):
        self.opWheel.draw()
    def right_button(self):
        self.opWheel.scroll_right()
    def down_button(self):
        self.opWheel.select()
    def left_button(self):
        self.opWheel.scroll_left()

