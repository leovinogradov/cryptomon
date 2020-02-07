from enum import Enum
class Action(Enum):
    GO_TO_MAIN_MENU         =  0
    GO_TO_SELECT_MON_MENU   =  1
    GO_TO_FOOD_MENU         =  2
    GO_TO_INTERACT_MENU     =  3
    GO_TO_MARKET_MENU       =  4
    GO_TO_FRIENDS_MENU      =  5
    GO_TO_TRADE_MENU        =  6
    GO_TO_PLAY_MENU         =  7
    
    OPEN_SELECT_MON_SUBMENU                 =  8
    OPEN_SELECT_MON_DISPOSE_SUBMENU         =  9
    OPEN_SELECT_MON_DISPOSE_SELL_SUBMENU    = 10
    OPEN_SELECT_MON_DISPOSE_RELEASE_SUBMENU = 11
    OPEN_FOOD_SUBMENU       = 12
    OPEN_INTERACT_SUBMENU   = 13
    OPEN_TRADE_SUBMENU      = 14
    OPEN_FRIENDS_SUBMENU    = 15
    
    INCREMENT_COUNTER       = 16
    DECREMENT_COUNTER       = 17

    DESELECT                = 18

    EXIT_APP                = 19
