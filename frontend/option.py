import pygame
from Action import *
class Option(object):
    def __init__(self, image, text, action=None, centered=False):
        self.image = image
        self.size = self.image.get_size()
        self.bigger_img = pygame.transform.scale(self.image, (int(self.size[0]*2), int(self.size[1]*2)))

        self.opX, self.opY = self.image.get_rect().size
        self.opBX, self.opBY = self.bigger_img.get_rect().size
        self.centered = centered

        self.text = text
        if(isinstance(action,Action)):
            self.action = action
            self.mon = None
            self.bigger_img_body = None
            self.opbXbody, self.opbYbody = None, None
            self.opBXbody, self.opBYbody = None, None
        else:
            self.mon = action
            self.action = None
            if(self.mon==None):
                self.bigger_img_body = None
                self.opbXbody, self.opbYbody = None, None
                self.opBXbody, self.opBYbody = None, None
            else:
                self.bigger_img_body = pygame.transform.scale(self.mon.body_image, (int(self.size[0]*2), int(self.size[1]*2)))
                self.opbXbody, self.opbYbody = self.mon.body_image.get_rect().size
                self.opBXbody, self.opBYbody = self.bigger_img_body.get_rect().size

    def draw(self, pyview, xOff, yOff):
        if (xOff == 0) and (yOff == 0):
            self.draw_scaled_body(pyview,xOff,yOff)
            self.draw_scaled_image(pyview, xOff, yOff)
            self.draw_scaled_caption(pyview, xOff, yOff)
        else:
            self.draw_unscaled_body(pyview,xOff,yOff)
            self.draw_unscaled_image(pyview, xOff, yOff)
            self.draw_unscaled_caption(pyview, xOff, yOff)

    def draw_scaled_body(self, pyview, xOff, yOff):
        if(self.mon != None):
            pyview.screen.blit(self.bigger_img_body, ((pyview.width - self.opBXbody - xOff)//2,(pyview.height - self.opBYbody//2 - yOff)//2))
    def draw_unscaled_body(self, pyview, xOff, yOff):
        if(self.mon != None):
            pyview.screen.blit(self.mon.body_image, ((pyview.width - self.opbXbody - xOff)//2,(pyview.height - self.opbYbody//2 - yOff)//2))

    def draw_scaled_image(self, pyview, xOff, yOff):
        pyview.screen.blit(self.bigger_img, ((pyview.width - self.opBX - xOff)//2,(pyview.height - self.opBY - yOff)//2))

    def draw_unscaled_image(self, pyview, xOff, yOff):
        pyview.screen.blit(self.image, ((pyview.width - self.opX - xOff)//2,(pyview.height - self.opY - yOff)//2))

    def draw_scaled_caption(self, pyview, xOff, yOff):
        fw, fh = pyview.font.size(self.text)
        surface = pyview.font.render(self.text, True, (0, 255, 0))
        pyview.screen.blit(surface, ((pyview.width - fw - xOff) // 2, (pyview.height - fh - self.opBY - yOff) // 2))

    def draw_unscaled_caption(self, pyview, xOff, yOff):
        fw, fh = pyview.font.size(self.text)
        surface = pyview.font.render(self.text, True, (0, 255, 0))
        offset = 0 if self.centered else self.opY
        pyview.screen.blit(surface, ((pyview.width - fw - xOff) // 2, (pyview.height - fh - offset - yOff) // 2))
        # pyview.screen.blit(surface, ((pyview.width - fw - xOff) // 2, (pyview.height - fh - yOff) // 2)) # no opY makes it centered

class OptionWheel(object):
    def __init__(self, pyview):
        self.pyview = pyview
        self.xOff = self.pyview.width*2//3
        self.yOff = self.pyview.height*1//3
        self.selected = False
        self.selection = 0
        self.options = []
        self.peeking = False

    def append_option(self,image_name,text,action=None,centered=False):
        if(isinstance(image_name,str)):
            self.options.append(Option(pygame.image.load(image_name),text,action,centered))
        else:
            self.append_option_existing(image_name,text,action,centered)

    def append_option_existing(self,image,text,action=None,centered=False):
        self.options.append(Option(image,text,action,centered))

    def select(self):
        self.selected = True
        return True

    def deselect(self):
        self.selected = False

    def draw(self):
        center_option = -1
        opNum = len(self.options)
        for i in range(0,opNum):
            amtXOff = self.selection-i
            amtYOff = amtXOff
            if amtYOff > 0 :
                amtYOff = -amtYOff
            if amtXOff == 1 or amtXOff == -1:
                if not self.selected:#dont draw if unselected during selection
                    self.options[i].draw(self.pyview,self.xOff*amtXOff,self.yOff*amtYOff)
            elif amtXOff == 0:
                center_option = i
        if center_option >= 0:#draw center option over all previous draws
            self.options[center_option].draw(self.pyview,0,0)
        if self.peeking == True:
            #display peeking head of primary_mon
            primary_mon = self.pyview.primary_mon
            if primary_mon != None:
                self.pyview.screen.blit(primary_mon.head_image,
                                        (int(self.pyview.width/2.5)-primary_mon.x_size,
                                         self.pyview.height-int(primary_mon.y_size/1.6)))

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
    def get_selected_op(self):
        return self.options[self.selection]
class OptionThree(OptionWheel):
    def __init__(self, pyview):
        self.pyview = pyview
        self.xOff = self.pyview.width*2//3
        self.yOff = -self.pyview.height*1//3
        self.selected = False
        self.selection = 1
        self.options = []
        self.peeking = False
    #def append_option(self,image,text,action=None):
    #    pass
    def select(self):
        pass
    def deselect(self):
        pass
    #def draw(self):
    #    pass
    def scroll_right(self):
        pass
    def scroll_left(self):
        pass

class OptionThree(OptionWheel):
    def __init__(self, pyview):
        self.pyview = pyview
        self.xOff = self.pyview.width*2//3
        self.yOff = -self.pyview.height*1//3
        self.selected = False
        self.selection = 1
        self.options = []
        self.peeking = False
    #def append_option(self,image,text,action=None):
    #    pass
    def select(self):
        pass
    def deselect(self):
        pass
    #def draw(self):
    #    pass
    def scroll_right(self):
        pass
    def scroll_left(self):
        pass
