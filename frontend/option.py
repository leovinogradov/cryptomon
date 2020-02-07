import pygame
class Option(object):

    def __init__(self):
        self.image = pygame.image.load("LionHeadClone.png")
        self.size = self.image.get_size()
        self.bigger_img = pygame.transform.scale(self.image, (int(self.size[0]*2), int(self.size[1]*2)))
        self.opX, self.opY = self.image.get_rect().size
        self.text = "sample text"
        self.action = None

    def __init__(self, image, text, action):
        self.image = image
        self.size = self.image.get_size()
        self.bigger_img = pygame.transform.scale(self.image, (int(self.size[0]*2), int(self.size[1]*2)))

        self.opX, self.opY = image.get_rect().size
        self.opBX, self.opBY = self.bigger_img.get_rect().size
        
        self.text = text
        self.action = action
        
    def draw(self, pyview, xOff, yOff):
        if (xOff == 0) & (yOff == 0):            
            fw, fh = pyview.font.size(self.text)
            surface = pyview.font.render(self.text, True, (0, 255, 0))
            pyview.screen.blit(self.bigger_img, ((pyview.width - self.opBX - xOff)//2,(pyview.height - self.opBY - yOff)//2))
            pyview.screen.blit(surface, ((pyview.width - fw - xOff) // 2, (pyview.height - fh - self.opBY - yOff) // 2))
        else:
            pyview.screen.blit(self.image, ((pyview.width - self.opX - xOff)//2,(pyview.height - self.opY - yOff)//2))
    
class OptionWheel(object):
    
    def __init__(self, pyview):
        self.pyview = pyview
        self.selected = False
        self.selection = 0
        self.options = []

    def append_option(self,image,text,action):
        self.options.append(Option(pygame.image.load(image),text,action))

    def select(self):
        #if self.selected == True:#turn off selection if selected
        #    return
        self.selected = True
        print(self.options[self.selection].action)
        #do nothing
        #act(self.options[self.selction].action)
        #act will perform the action interfaced by the act class

    def deselect(self):
        self.selected = False
        
    def draw(self):
        xOff = self.pyview.width*2//3
        yOff = self.pyview.height*1//3
        center_option = -1
        opNum = len(self.options)
        for i in range(0,opNum):
            amtXOff = self.selection-i
            amtYOff = amtXOff
            if amtYOff > 0 :
                amtYOff = -amtYOff
            if amtXOff != 0:
                if not self.selected:
                    self.options[i].draw(self.pyview,xOff*amtXOff,yOff*amtYOff)
            else:
                center_option = i
        if center_option >= 0:
            self.options[center_option].draw(self.pyview,0,0)
            
    def scroll_right(self):
        if self.selected == True:#turn off scrolling if selected
            return
        if(self.selection < len(self.options) - 1):
            self.selection += 1
    def scroll_left(self):
        if self.selected == True:#turn off scrolling if selected
            return
        if(self.selection > 0):
            self.selection -= 1

