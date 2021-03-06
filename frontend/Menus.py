import pygame
from option import *
from Action import *
from Mon import *
from node_integration import *

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

class Menu_W_Sub(Menu):
    def __init__(self, pyview):
        self.pyview = pyview
        self.opWheel = OptionWheel(pyview)
        self.inactive_wheel = []
        self.submenu = []
        self.amount = 0
    def right_button(self):
        super().right_button()
        if len(self.submenu)!=0:
            self.submenu[-1].right_button()
    def left_button(self):
        super().left_button()
        if len(self.submenu)!=0:
            self.submenu[-1].left_button()
    def activate_submenu(self,submenu):
        self.submenu.append(submenu)
        self.inactive_wheel.append(self.opWheel)
        self.opWheel = submenu.opWheel
    def deactivate_submenu(self):
        self.opWheel = self.inactive_wheel.pop()
        self.opWheel.selected = False
        self.submenu.pop()
    def dec(self,amt):
        if(amt <= self.amount):
            self.amount -= amt
            text = '{:.4f}'.format(self.amount)
            self.opWheel.options[1].text = text
    def inc(self,amt):
        total = self.amount + amt
        self.amount = total
        text = '{:.4f}'.format(self.amount)
        self.opWheel.options[1].text = text
    def inc_cap(self,amt):
        total = self.amount + amt
        if( self.pyview.funds >= total):
            self.amount = total
            text = '{:.4f}'.format(self.amount)
            self.opWheel.options[1].text = text


class Submenu:
    def __init__(self, menu):
        self.menu = menu
        self.opWheel = OptionThree(menu.pyview)
    def left_button(self):
        pass
    def down_button(self):
        pass
    def right_button(self):
        pass


class Main_Menu(Menu):
    def __init__(self, pyview):
        super().__init__(pyview)
        #Set Background Image
        #pyview.background = pygame.image.load(pyview.path+"Stage.png")
        # primary_mon = self.pyview.primary_mon
        # if primary_mon != None:
        #     self.opWheel.append_option(primary_mon.head_image,primary_mon.name,Action.GO_TO_INTERACT_MENU)
        self.opWheel.append_option(pyview.path+"LionHeadGrass.png","Manage Cryptomon",Action.GO_TO_SELECT_MON_MENU)
        self.opWheel.append_option(pyview.path+"LionHeadNormal.png","Manage Friends",Action.GO_TO_FRIENDS_MENU)
        self.opWheel.append_option(pyview.path+"LionHeadPoison.png","Exit App",Action.EXIT_APP)

class Select_Mon_Menu(Menu_W_Sub):
    def __init__(self, pyview):
        super().__init__(pyview)
        #Set Background Image
        #pyview.background = pygame.image.load(pyview.path+"Stage.jpg")
        #Set Wheel Contents
        self.opWheel.append_option(pyview.path+"LionHeadElectric.png","Buy Cryptomon",Action.GO_TO_MARKET_MENU)
        #List Cryptomon
        for i in self.pyview.my_mons:
            #self.opWheel.append_option(i.head_image,i.name,i)
            self.opWheel.append_option(i.head_image,str(i.index),i)
        self.opWheel.append_option(pyview.path+"LionHeadNormal.png","Return to Main Menu",Action.GO_TO_MAIN_MENU)

    def down_button(self):
        super().down_button()
        if len(self.submenu)!=0:
            self.submenu[-1].down_button()
        else:
            #update primary_mon
            selected_op = self.opWheel.get_selected_op()
            if(selected_op.mon!=None):
                self.pyview.primary_mon = selected_op.mon
                if(self.pyview.primary_mon.listed):#Check if mon is listed
                    self.activate_submenu(Select_Submenu_2(self))#listed
                else:
                    self.activate_submenu(Select_Submenu_1(self))#unlisted
    def deactivate_all_submenus(self):
        while len(self.submenu)!=0:
            self.opWheel = self.inactive_wheel.pop()
            self.opWheel.selected = False
            self.submenu.pop()

class Select_Submenu_1(Submenu):
    def __init__(self, menu):
        super().__init__(menu)
        primary_mon = self.menu.pyview.primary_mon
        # self.opWheel.append_option(primary_mon.head_image,"Dispose Menu",primary_mon)
        self.opWheel.append_option(self.menu.pyview.path+"BtnBlank.png","Market", centered=True)
        self.opWheel.append_option(primary_mon.head_image,"Interact",primary_mon)
        # self.opWheel.append_option(self.menu.pyview.path+"LionHeadElectric.png","Cancel")
        self.opWheel.append_option(self.menu.pyview.path+"BtnBlank.png","Cancel", centered=True)
    def left_button(self):
        self.menu.activate_submenu(Select_Submenu_1_1(self.menu))
    def down_button(self):
        # self.menu.pyview.change_menu(Action.GO_TO_INTERACT_MENU)
        self.menu.activate_submenu(Select_Submenu_1_Feed(self.menu))
    def right_button(self):
        self.menu.deactivate_submenu()

class Select_Submenu_1_Feed(Submenu):
    def __init__(self, menu):
        super().__init__(menu)
        primary_mon = self.menu.pyview.primary_mon
        numFoods = len(self.menu.pyview.myFood)
        # self.opWheel.append_option(self.menu.pyview.path+"LionHeadElectric.png","Change Food")
        self.opWheel.append_option(self.menu.pyview.path+"empty.png", f"Food remaining: {numFoods}", centered=True)
        self.opWheel.append_option(primary_mon.head_image, "Feed this mon", primary_mon)
        self.opWheel.append_option(self.menu.pyview.path+"BtnBlank.png", "Cancel Feed", centered=True)
    def left_button(self):
        # self.menu.pyview.change_menu(Action.GO_TO_FOOD_MENU)
        pass
    def down_button(self):
        #TODO: apply primary food to primary mon
        if (len(self.menu.pyview.myFood) <= 0):
            return
        itemapply(self.menu.pyview.my_index, self.menu.pyview.myFood.pop(), self.menu.pyview.primary_mon.index)
        numFoods = len(self.menu.pyview.myFood)
        self.opWheel.options[0].text = f"Food remaining: {numFoods}";
    def right_button(self):
        self.menu.deactivate_submenu()

class Select_Submenu_1_1(Submenu):#dispose menu
    def __init__(self, menu):
        super().__init__(menu)
        # self.opWheel.append_option(self.menu.pyview.path+"LionHeadElectric.png","Trade")
        self.opWheel.append_option(self.menu.pyview.path+"BtnBlank.png","Trade", centered=True)
        self.opWheel.append_option(self.menu.pyview.path+"BtnBlank.png","Sell", centered=True)
        # self.opWheel.append_option(self.menu.pyview.path+"LionHeadElectric.png","Release")
        self.opWheel.append_option(self.menu.pyview.path+"BtnBlank.png","Release", centered=True)
        self.opWheel.peeking = True # enables selected mon to peek
    def left_button(self):
        self.menu.pyview.change_menu(Action.GO_TO_TRADE_MENU)
    def down_button(self):
        self.menu.activate_submenu(Select_Submenu_1_1_1(self.menu))
    def right_button(self):
        self.menu.activate_submenu(Select_Submenu_1_1_2(self.menu))

class Select_Submenu_1_1_1(Submenu):#Sell Mon
    def __init__(self, menu):
        super().__init__(menu)
        self.opWheel.append_option(self.menu.pyview.path+"Down.png","Less")
        self.opWheel.append_option(self.menu.pyview.path+"LionHeadElectric.png","Confirm Price")
        self.opWheel.append_option(self.menu.pyview.path+"Up.png","More")
        self.opWheel.peeking = True#enables selected mon to peek
    def left_button(self):
        #decrease price
        self.menu.dec(.01)
    def down_button(self):
        self.menu.activate_submenu(Select_Submenu_1_1_1_1(self.menu))
    def right_button(self):
        #increase price
        self.menu.inc(.01)


class Select_Submenu_1_1_1_1(Submenu):#list mon
    def __init__(self, menu):
        super().__init__(menu)
        self.opWheel.append_option(self.menu.pyview.path+"LionHeadElectric.png","List Mon")
        self.opWheel.append_option(self.menu.pyview.path+"BtnBlank.png","Cancel", centered=True)
        self.opWheel.peeking = True#enables selected mon to peek
    def left_button(self):
        my_index    = self.menu.pyview.my_index
        primary_mon = self.menu.pyview.primary_mon
        err = listmon(my_index,'{:.4f}'.format(self.menu.amount) + " TNT",primary_mon.index)
        if(err == ''):
            primary_mon.enlist()
            self.menu.pyview.primary_mon = None
        self.menu.pyview.change_menu(Action.GO_TO_MAIN_MENU)
    def down_button(self):
        self.menu.deactivate_all_submenus()
    def right_button(self):
        pass


class Select_Submenu_1_1_2(Submenu):#Release Mon
    def __init__(self, menu):
        super().__init__(menu)
        self.opWheel.append_option(self.menu.pyview.path+"LionHeadElectric.png","Confirm Release")
        self.opWheel.append_option(self.menu.pyview.path+"BtnBlank.png","Cancel", centered=True)
        self.opWheel.peeking = True#enables selected mon to peek
    def left_button(self):
        my_index    = self.menu.pyview.my_index
        primary_mon = self.menu.pyview.primary_mon
        err = deletemon(self.menu.pyview.my_index,primary_mon.index)
        if(err == ''):
            self.menu.pyview.my_mons.remove(primary_mon)
            self.menu.pyview.primary_mon = None
        self.menu.pyview.change_menu(Action.GO_TO_MAIN_MENU)
    def down_button(self):
        self.menu.deactivate_all_submenus()
    def right_button(self):
        pass

class Select_Submenu_2(Submenu):#delist mon
    def __init__(self, menu):
        super().__init__(menu)
        self.opWheel.append_option(self.menu.pyview.path+"LionHeadElectric.png","Cancel Trade/Sale")
        self.opWheel.append_option(self.menu.pyview.path+"BtnBlank.png","Cancel", centered=True)
        self.opWheel.peeking = True#enables selected mon to peek
    def left_button(self):
        my_index    = self.menu.pyview.my_index
        primary_mon = self.menu.pyview.primary_mon
        err = canceltrade(my_index,primary_mon.index)
        if(err == ''):
            primary_mon.delist()
        else:
            my_trades = getyourtrades(my_index)
            for i in my_trades["trades"]:
                index_listed = i['cryptomon_index']
                if index_listed == primary_mon.index:
                    err = canceltrade(my_index,i['cryptomon_index2'])
                    if(err == ''):
                        primary_mon.delist()
        self.menu.pyview.change_menu(Action.GO_TO_MAIN_MENU)
    def down_button(self):
        self.menu.deactivate_submenu()
    def right_button(self):
        pass

class Food_Menu(Menu_W_Sub):
    def __init__(self, pyview):
        super().__init__(pyview)
        #Set Background Image
        #pyview.background = pygame.image.load(pyview.path+"Stage.jpg")
        #Set Wheel Contents
        #TODO:Display all food options
        self.opWheel.append_option(pyview.path+"LionHeadNormal.png","Return to Interact Menu",Action.GO_TO_INTERACT_MENU)
    def down_button(self):
        super().down_button()
        if len(self.submenu)!=0:
            self.submenu[-1].down_button()

class Food_Submenu(Submenu):
    def __init__(self, menu):
        super().__init__(menu)
        #TODO:Replace Default images with Food Images
        self.opWheel.append_option(self.menu.pyview.path+"Down.png","Less")
        self.opWheel.append_option(self.menu.pyview.path+"LionHeadElectric.png","Confirm")
        self.opWheel.append_option(self.menu.pyview.path+"Up.png","More")
        #TODO:Get amount of food
    def left_button(self):
        #Sub from this counter for food
        if(1 <= self.amount):
            self.amount -= 1
        self.opWheel.options[1].text = '{:d}'.format(self.amount)
    def down_button(self):
        self.menu.pyview.change_menu(Action.GO_TO_INTERACT_MENU)
    def right_button(self):
        #Add to this counter for food
        self.amount -= 1
        self.opWheel.options[1].text = '{:d}'.format(self.amount)

class Interact_Menu(Menu_W_Sub):
    def __init__(self, pyview):
        super().__init__(pyview)
        #Set Background Image
        #pyview.background = pygame.image.load(pyview.path+"Stage.jpg")
        #Set Wheel Contents
        self.opWheel.append_option(pyview.path+"LionHeadNormal.png","Play",Action.GO_TO_PLAY_MENU)
        self.opWheel.append_option(pyview.path+"LionHeadNormal.png","Clean Pen")
        self.opWheel.append_option(pyview.path+"LionHeadNormal.png","Feed Cryptomon")
        self.opWheel.append_option(pyview.path+"LionHeadNormal.png","Manage Cryptomon",Action.GO_TO_SELECT_MON_MENU)
    def down_button(self):
        super().down_button()
        if len(self.submenu)!=0:
            self.submenu[-1].down_button()
        else:
            selection = self.opWheel.selection
            if(selection == 1):
                #clean pen = Do Nothing
                self.opWheel.deselect()
            elif(selection == 2):
                self.activate_submenu(Interact_Submenu(self))


class Interact_Submenu(Submenu):
    def __init__(self, menu):
        super().__init__(menu)
        primary_mon = self.menu.pyview.primary_mon;
        # self.opWheel.append_option(self.menu.pyview.path+"LionHeadElectric.png","Change Food")
        self.opWheel.append_option(self.menu.pyview.path+"empty.png", "Food remaining: 5", centered=True)
        self.opWheel.append_option(primary_mon.head_image, "Feed this mon", primary_mon)
        self.opWheel.append_option(self.menu.pyview.path+"BtnBlank.png", "Cancel Feed", centered=True)
    def left_button(self):
        # self.menu.pyview.change_menu(Action.GO_TO_FOOD_MENU)
        pass
    def down_button(self):
        #TODO: apply primary food to primary mon
        # self.menu.deactivate_submenu()
        pass
    def right_button(self):
        self.menu.deactivate_submenu()

class Market_Menu(Menu_W_Sub):
    def __init__(self, pyview):
        super().__init__(pyview)
        #Set Background Image
        #pyview.background = pygame.image.load(pyview.path+"Stage.jpg")
        #Set Wheel Contents
        self.opWheel.append_option(pyview.path+"LionHeadNormal.png","Return to Manage Cryptomon",Action.GO_TO_SELECT_MON_MENU)

        listings = getlistings()
        print(listings)
        self.mons_listed = []
        self.selected_mon = None
        for i in listings:
            if i['account_one'] != pyview.my_index:#dont show my listings
                mon = Mon(pyview,getcryptomon(i['cryptomon_index2']))
                mon.name = i['price']
                self.mons_listed.append(mon)
                self.opWheel.append_option(mon.head_image,mon.name,mon)

        self.opWheel.append_option(pyview.path+"LionHeadNormal.png","Refresh Shop Page")
    def down_button(self):
        super().down_button()
        if len(self.submenu)!=0:
            self.submenu[-1].down_button()
        else:
            selection = self.opWheel.selection
            if(selection == len(self.opWheel.options)-1):
                #Refresh Page
                self.pyview.change_menu(Action.GO_TO_MARKET_MENU)
            elif(selection != 0):
                self.selected_mon = self.mons_listed[selection - 1]
                self.activate_submenu(Market_Submenu(self))


class Market_Submenu(Submenu):
    def __init__(self, menu):
        super().__init__(menu)
        #TODO: EXCHANGE MON PIC WITH SELECTED MON FROM SHOP
        self.opWheel.append_option(menu.selected_mon.head_image,"Buy Cryptomon", menu.selected_mon)
        self.opWheel.append_option(self.menu.pyview.path+"BtnBlank.png","Cancel", centered=True)
    def left_button(self):
        #Buy Mon
        my_index    = self.menu.pyview.my_index
        selected_mon = self.menu.selected_mon
        print(selected_mon.index)
        purchasemon(my_index,selected_mon.index)
        self.menu.pyview.change_menu(Action.GO_TO_MAIN_MENU)
    def down_button(self):
        self.menu.deactivate_submenu()
    def right_button(self):
        pass

class Friends_Menu(Menu_W_Sub):
    def __init__(self, pyview):
        super().__init__(pyview)
        #Set Background Image
        #pyview.background = pygame.image.load(pyview.path+"Stage.jpg")
        #Set Wheel Contents
        self.opWheel.append_option(pyview.path+"LionHeadNormal.png","New Friend")
        for i in self.pyview.myFriends:
            self.opWheel.append_option(pyview.path+"LionHeadNormal.png",i['player_name'])
        self.opWheel.append_option(pyview.path+"LionHeadNormal.png","Return to Main Menu",Action.GO_TO_MAIN_MENU)
    def down_button(self):
        super().down_button()
        if len(self.submenu)!=0:
            self.submenu[-1].down_button()
        else:
            selection = self.opWheel.selection
            if(selection == 0):
                pass
            elif(selection != len(self.opWheel.options) - 1):
                self.pyview.primary_friend = self.pyview.myFriends[selection - 1]
                self.activate_submenu(Friend_Submenu(self))

class Friend_Submenu(Submenu):
    def __init__(self, menu):
        super().__init__(menu)
        self.opWheel.append_option(self.menu.pyview.path+"LionHeadNormal.png","Delete Friend")
        self.opWheel.append_option(self.menu.pyview.path+"BtnBlank.png","Cancel", centered=True)
    def left_button(self):
        self.menu.pyview.myFriends.remove(self.menu.pyview.primary_friend)
        self.menu.pyview.change_menu(Action.GO_TO_MAIN_MENU)
    def down_button(self):
        self.menu.deactivate_submenu()
    def right_button(self):
        pass


class Trade_Menu(Menu_W_Sub):
    def __init__(self, pyview):
        super().__init__(pyview)
        #Set Background Image
        #pyview.background = pygame.image.load(pyview.path+"Stage.jpg")

        for i in pyview.myFriends:
            self.opWheel.append_option(pyview.path+"LionHeadNormal.png","Trade with " + i['player_name'])
        self.opWheel.append_option(pyview.path+"LionHeadNormal.png","Return to Main Menu",Action.GO_TO_MAIN_MENU)

        self.trades = []
        trade_dict = getofferredtrades(pyview.my_index)
        if(len(trade_dict['trades']) != 0):#TODO:check if any trade is incoming
            for i in trade_dict['trades']:
                if i['cryptomon_index2'] == pyview.primary_mon.index:
                    self.trades.append(i)
            if(len(self.trades) == 1):#if exactly one trade has been issued on said mon
                self.opWheel.selected = True
                self.activate_submenu(Trade_Submenu(self))


    def down_button(self):
        super().down_button()
        if len(self.submenu)!=0:
            self.submenu[-1].down_button()
        else:
            selection = self.opWheel.selection
            if(selection != len(self.opWheel.options) - 1):#if not last option
                self.pyview.primary_friend = self.pyview.myFriends[selection]
                self.pyview.change_menu(Action.GO_TO_TRADE_FRIEND_MENU)


class Trade_Submenu(Submenu):
    def __init__(self, menu):
        super().__init__(menu)
        their_index = self.menu.trades[0]['cryptomon_index']
        their_mon = Mon(self.menu.pyview,getcryptomon(their_index))
        self.opWheel.append_option(their_mon.head_image,"Decline Trade",their_mon)
        self.opWheel.append_option(their_mon.head_image,"Accept Trade",their_mon)
        self.opWheel.append_option(self.menu.pyview.path+"BtnBlank.png","Cancel", centered=True)
        self.opWheel.peeking = True#enables selected mon to peek
    def left_button(self):
        canceltrade(self.menu.pyview.my_index, self.menu.pyview.primary_mon.index)
        self.menu.deactivate_submenu()
    def down_button(self):
        accepttrade(self.menu.pyview.my_index,self.menu.pyview.primary_mon.index)
        self.menu.deactivate_submenu()
    def right_button(self):
        self.menu.deactivate_submenu()

class Trade_Friend_Menu(Menu_W_Sub):
    def __init__(self, pyview):
        super().__init__(pyview)
        #Set Background Image
        #pyview.background = pygame.image.load(pyview.path+"Stage.jpg")
        #Set Wheel Contents
        info = getallinfo(pyview.primary_friend['player_index'])
        self.friend_mons = []
        self.selected_friend_mon = None
        for i in info['cryptomons']:
            self.friend_mons.append(Mon(pyview,i))
        for i in self.friend_mons:
            self.opWheel.append_option(i.head_image,str(i.index),i)
        self.opWheel.append_option(pyview.path+"LionHeadNormal.png","Return to Trade Menu",Action.GO_TO_TRADE_MENU)
        self.opWheel.peeking = True#enables selected mon to peek
    def down_button(self):
        super().down_button()
        if len(self.submenu)!=0:
            self.submenu[-1].down_button()
        else:
            selection = self.opWheel.selection
            if(selection != len(self.opWheel.options) - 1):#if not last option
                self.selected_friend_mon = self.friend_mons[selection]
                self.activate_submenu(Trade_Friend_Submenu_1(self))

    def deactivate_all_submenus(self):
        while len(self.submenu)!=0:
            self.opWheel = self.inactive_wheel.pop()
            self.opWheel.selected = False
            self.submenu.pop()

class Trade_Friend_Submenu_1(Submenu):
    def __init__(self, menu):
        super().__init__(menu)
        mon = self.menu.selected_friend_mon
        self.opWheel.append_option(mon.head_image,"Buy Mon", mon)
        self.opWheel.append_option(mon.head_image,"Trade Mon", mon)
        self.opWheel.append_option(mon.head_image,"Trade + Buy Mon", mon)
        self.opWheel.peeking = True#enables selected mon to peek
    def left_button(self):
        self.menu.activate_submenu(Trade_Friend_Submenu_1_2(self.menu))
    def down_button(self):
        self.menu.activate_submenu(Trade_Friend_Submenu_1_1(self.menu))
    def right_button(self):
        self.menu.activate_submenu(Trade_Friend_Submenu_1_3(self.menu))

class Trade_Friend_Submenu_1_1(Submenu):#Trade
    def __init__(self, menu):
        super().__init__(menu)
        mon = self.menu.selected_friend_mon
        self.opWheel.append_option(mon.head_image,"Confirm", mon)
        self.opWheel.append_option(self.menu.pyview.path+"BtnBlank.png","Cancel", centered=True)
        self.opWheel.peeking = True#enables selected mon to peek
    def left_button(self):
        my_index      = self.menu.pyview.my_index
        their_index   = self.menu.pyview.primary_friend['player_index']
        #amount        = '{:.4f}'.format(self.menu.amount) + " TNT"
        amount        = '{:.4f}'.format( 0 ) + " TNT"
        primary_mon   = self.menu.pyview.primary_mon
        secondary_mon = self.menu.selected_friend_mon
        err = inittrade(my_index,their_index,amount,1,primary_mon.index,secondary_mon.index)
        if(err == ''):
            primary_mon.enlist()
            self.menu.pyview.primary_mon = None
        self.menu.pyview.change_menu(Action.GO_TO_MAIN_MENU)
    def down_button(self):
        self.menu.deactivate_all_submenus()
    def right_button(self):
        pass
class Trade_Friend_Submenu_1_2(Submenu):#Buy
    def __init__(self, menu):
        super().__init__(menu)
        mon = self.menu.selected_friend_mon
        self.opWheel.append_option(self.menu.pyview.path+"Down.png","Less")
        self.opWheel.append_option(mon.head_image,"Confirm Price", mon)
        self.opWheel.append_option(self.menu.pyview.path+"Up.png","More")
        #TODO:Get amount default
    def left_button(self):
        self.menu.dec(.01)
    def down_button(self):
        self.menu.activate_submenu(Trade_Friend_Submenu_1_2_1(self.menu))
    def right_button(self):
        self.menu.inc_cap(.01)
class Trade_Friend_Submenu_1_2_1(Submenu):
    def __init__(self, menu):
        super().__init__(menu)
        mon = self.menu.selected_friend_mon
        self.opWheel.append_option(mon.head_image,"Confirm", mon)
        self.opWheel.append_option(self.menu.pyview.path+"BtnBlank.png","Cancel", centered=True)
        self.opWheel.peeking = True#enables selected mon to peek
    def left_button(self):
        my_index      = self.menu.pyview.my_index
        their_index   = self.menu.pyview.primary_friend['player_index']
        amount        = '{:.4f}'.format(self.menu.amount) + " TNT"
        primary_mon   = ""
        secondary_mon = self.menu.selected_friend_mon
        err = inittrade(my_index,their_index,amount,1,primary_mon.index,secondary_mon.index)
        if(err == ''):
            primary_mon.enlist()
            self.menu.pyview.primary_mon = None
        self.menu.pyview.change_menu(Action.GO_TO_MAIN_MENU)
    def down_button(self):
        self.menu.deactivate_all_submenus()
    def right_button(self):
        pass
class Trade_Friend_Submenu_1_3(Submenu):#Trade + Buy
    def __init__(self, menu):
        super().__init__(menu)
        mon = self.menu.selected_friend_mon
        self.opWheel.append_option(self.menu.pyview.path+"Down.png","Less")
        self.opWheel.append_option(mon.head_image,"Confirm Price", mon)
        self.opWheel.append_option(self.menu.pyview.path+"Up.png","More")
        self.opWheel.peeking = True#enables selected mon to peek
        #TODO:Get amount default
    def left_button(self):
        self.menu.dec(.01)
    def down_button(self):
        self.menu.activate_submenu(Trade_Friend_Submenu_1_3_1(self.menu))
    def right_button(self):
        self.menu.inc_cap(.01)
class Trade_Friend_Submenu_1_3_1(Submenu):
    def __init__(self, menu):
        super().__init__(menu)
        mon = self.menu.selected_friend_mon
        self.opWheel.append_option(mon.head_image,"Confirm", mon)
        self.opWheel.append_option(self.menu.pyview.path+"BtnBlank.png","Cancel", centered=True)
        self.opWheel.peeking = True#enables selected mon to peek
    def left_button(self):
        my_index      = self.menu.pyview.my_index
        their_index   = self.menu.pyview.primary_friend['player_index']
        amount        = '{:.4f}'.format(self.menu.amount) + " TNT"
        primary_mon   = self.menu.pyview.primary_mon
        secondary_mon = self.menu.selected_friend_mon
        err = inittrade(my_index,their_index,amount,1,primary_mon.index,secondary_mon.index)
        if(err == ''):
            primary_mon.enlist()
            self.menu.pyview.primary_mon = None
        self.menu.pyview.change_menu(Action.GO_TO_MAIN_MENU)
    def down_button(self):
        self.menu.deactivate_all_submenus()
    def right_button(self):
        pass


class Play_Menu(Menu):
    def __init__(self, pyview):
        super().__init__(pyview)
        #Set Background Image
        #pyview.background = pygame.image.load(pyview.path+"Stage.jpg")
        #Set Wheel Contents
        self.opWheel.append_option(pyview.path+"LionHeadNormal.png","Stop Playing",Action.GO_TO_INTERACT_MENU)
