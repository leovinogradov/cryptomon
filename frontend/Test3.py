import pygame
import os
from time import sleep
#import RPi.GPIO as GPIO #Comment this line back in if Physical Buttons are attatched
from Menus import *
from Action import *
from Mon import *
from node_integration import *

class PygameView(object):


    def __init__(self, width=480, height=320, fps=30):
        """Initialize GPIO pins
        """
        """#Comment this block back in if Physical Buttons are attatched
        self.button_map = (17,27,22)
        GPIO.setmode(GPIO.BCM)
        for k in self.button_map:
            GPIO.setup(k, GPIO.IN,pull_up_down=GPIO.PUD_DOWN)
        os.putenv('SDL_FBDEV','/dev/fb1')

        GPIO.add_event_detect(self.button_map[0], GPIO.FALLING, callback=self.right_button,bouncetime=200)
        GPIO.add_event_detect(self.button_map[1], GPIO.FALLING, callback=self.down_button,bouncetime=200)
        GPIO.add_event_detect(self.button_map[2], GPIO.FALLING, callback=self.left_button,bouncetime=200)
        """
        """Initialize pygame, window, background, font,...
        """
        pygame.init()
        pygame.mouse.set_visible(False)
        pygame.display.set_caption("Press ESC to quit")
        self.width = width
        self.height = height
        self.screen = pygame.display.set_mode((self.width, self.height), pygame.DOUBLEBUF)
        self.background = pygame.Surface(self.screen.get_size()).convert_alpha()
        self.clock = pygame.time.Clock()
        self.fps = fps
        self.playtime = 0.0
        self.font = pygame.font.SysFont('mono', 15, bold=True)

        dir_path = os.path.dirname(__file__)
        rel_path = "/assets/"
        self.path = os.path.join(dir_path+rel_path)

        self.my_index = "zvnxqtokmcqs"
        self.my_info_update()
        self.myFriends = ["ingkdmmngzgi"]#TODO add friends list

        self.primary_mon = None
        self.primary_food = None
        self.primary_friend = None
        self.primary_friend_mon = None

        self.menu = Main_Menu(self)

    def run(self):
        self.running = True
        while self.running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.running = False
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_ESCAPE:
                        self.running = False
                    elif event.key == pygame.K_RIGHT:
                        self.right_button(0)
                    elif event.key == pygame.K_DOWN:
                        self.down_button(0)
                    elif event.key == pygame.K_LEFT:
                        self.left_button(0)
                elif event.type == pygame.MOUSEBUTTONUP:
                    x, y = pygame.mouse.get_pos()
                    if x > self.width*2/3:
                        self.right_button(0)
                    elif x > self.width/3:
                        self.down_button(0)
                    else:
                        self.left_button(0)
            self.menu.draw()
            pygame.display.flip()
            self.screen.blit(self.background, (0, 0))

        pygame.quit()

    def right_button(self,callback_type):
        self.menu.right_button()
    def down_button(self,callback_type):
        self.menu.down_button()
    def left_button(self,callback_type):
        self.menu.left_button()
        
    def my_info_update(self):
        info = getallinfo(self.my_index)
        self.my_name = info['playerName']
        funds, tnt = info['funds'].split()
        self.funds = float(funds)
        self.my_mons = []
        cryptomons = info['cryptomons']
        for i in cryptomons:
            self.my_mons.append(Mon(self,i))
        self.myFood = info['inventory']

    def change_menu(self, action):
        if action == Action.GO_TO_MAIN_MENU:
            self.menu = Main_Menu(self)
        elif action == Action.GO_TO_SELECT_MON_MENU:
            self.menu = Select_Mon_Menu(self)
        elif action == Action.GO_TO_FOOD_MENU:
            self.menu = Food_Menu(self)
        elif action == Action.GO_TO_INTERACT_MENU:
            self.menu = Interact_Menu(self)
        elif action == Action.GO_TO_MARKET_MENU:
            self.menu = Market_Menu(self)
        elif action == Action.GO_TO_FRIENDS_MENU:
            self.menu = Friends_Menu(self)
        elif action == Action.GO_TO_TRADE_MENU:
            self.menu = Trade_Menu(self)
        elif action == Action.GO_TO_TRADE_FRIEND_MENU:
            self.menu = Trade_Friend_Menu(self)
        elif action == Action.GO_TO_PLAY_MENU:
            self.menu = Play_Menu(self)
        elif action == Action.EXIT_APP:
            self.running = False

    def draw_FPS(self):
        milliseconds = self.clock.tick(self.fps)
        self.playtime += milliseconds / 1000.0
        self.draw_text("FPS: {:6.3}{}PLAYTIME: {:6.3} SECONDS".format(
            self.clock.get_fps(), " "*5, self.playtime),0,0)

    def draw_text(self, text, x, y):
        surface = self.font.render(text, True, (0, 255, 0))
        self.screen.blit(surface, (x, y))

    def draw_text_center(self, text):
        fw, fh = self.font.size(text) # fw: font width,  fh: font height
        surface = self.font.render(text, True, (0, 255, 0))
        # // makes integer division in python3
        self.screen.blit(surface, ((self.width - fw) // 2, (self.height - fh) // 2))
####

if __name__ == '__main__':

    # call with width of window and fps
    PygameView(480, 320).run()
