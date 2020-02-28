# Approach 1 would be to use the Naked library. However, lets try without external libraries first
import subprocess
import json

# account is account name, such as "alice"
def getplayer(account):
    try:
        c = ["node", "integration_readfcns.js", "getplayer", "--account"]
        c.append(account) # command is separated for readablity

        out = subprocess.check_output(c).decode("utf-8")
        return json.loads(out)
    except Exception as e:
        print(e)
        return -1;

# get all info about player and their cryptomons
def getallinfo(account):
    try:
        c = ["node", "integration_readfcns.js", "getallinfo", "--account"]
        c.append(account) # command is separated for readablity

        out = subprocess.check_output(c).decode("utf-8")
        return json.loads(out)
    except Exception as e:
        print(e)
        return -1;

# get cryptomon info by index
def getcryptomon(index):
    try:
        c = ["node", "integration_readfcns.js", "getcryptomon", "--index"]
        c.append(str(index)) # command is separated for readablity

        out = subprocess.check_output(c).decode("utf-8")
        return json.loads(out)
    except Exception as e:
        print(e)
        return -1;

# get trades offered to a Player by account name
def getofferredtrades(account: str):
    try:
        c = ["node", "integration_readfcns.js", "getofferedtrades", "--account"]
        c.append(account) # command is separated for readablity

        out = subprocess.check_output(c).decode("utf-8")
        return json.loads(out)
    except Exception as e:
        print(e)
        return -1;

# get cryptomon listings in the transact table, for now 10 listings will show
def getlistings():
    try:
        c = ["node", "integration_readfcns.js", "getlistings"]
        out = subprocess.check_output(c).decode("utf-8")
        return json.loads(out)
    except Exception as e:
        print(e)
        return -1;

# get all transact events including trades and listings of a Player by their account name
def getyourtransacts(account: str):
    try:
        c = ["node", "integration_readfcns.js", "getyourtransacts", "--account"]
        c.append(account) # command is separated for readablity
        out = subprocess.check_output(c).decode("utf-8")
        return json.loads(out)
    except Exception as e:
        print(e)
        return -1;

# get all entries in the transacts table
def gettransacts():
    try:
        c = ["node", "integration_readfcns.js", "gettransacts"]
        out = subprocess.check_output(c).decode("utf-8")
        return json.loads(out)
    except Exception as e:
        print(e)
        return -1;

# account: string
def createmon(account: str):
    try:
        c = ["node", "integration.js", "createmon", "--account"]
        c.append(account) # command is separated for readablity
        return subprocess.check_output(c).decode("utf-8")
    except Exception as e:
        print(e)
        return -1;

def deletemon(account: str, index: int):
    try:
        c = ["node", "integration.js", "deletemon", "--account"]
        c.append(account)
        c.append("--index")
        c.append(str(index))
        return subprocess.check_output(c).decode("utf-8")
    except Exception as e:
        print(e)
        return -1;

# List Cryptomon for sale
# asset = the price of the cryptomon in the format [asset type]
# delay = duration of sale
# index = index of Cryptomon
def listmon(account: str, asset: str, index: int):
    try:
        c = ["node", "integration.js", "listmon", "--account"]
        c.append(account)
        c.append("--asset " + asset)
        c.append("--index " + str(index))
        return subprocess.check_output(c).decode("utf-8")
    except Exception as e:
        print(e)
        return -1;

# Remove cryptomon from trade table
def delistmon(account, index):
    try:
        c = ["node", "integration.js", "delistmon"]
        c.append("--account"+account)
        c.append("--index"+str(index))
        return subprocess.check_output(c).decode("utf-8")
    except Exception as e:
        print(e)
        return -1;

def purchasemon(account, index):
    try:
        c = ["node", "integration.js", "purchasemon"]
        c.append("--account "+ account)
        c.append("--index " + str(index))
        return subprocess.check_output(c).decode("utf-8")
    except Exception as e:
        print(e)
        return -1;

def accepttrade(account, index):
    try:
        c = ["node", "integration.js", "accepttrade"]
        c.append("-account " + account)
        c.append("--index "+ str(index))
        return subprocess.check_output(c).decode("utf-8")
    except Exception as e:
        print(e)
        return -1;

def inittrade(account1: str, account2, price, swap, c1, c2):
    try:
        c = ["node", "integration.js", "inittrade"]
        c.append("--account1 "+account1)
        c.append("--account2 "+account2)
        c.append("--price "+ price)
        c.append("--swap "+ swap)
        c.append("--c1 " + str(c1))
        c.append("--c2 " + str(c2))
        return subprocess.check_output(c).decode("utf-8")
    except Exception as e:
        print(e)
        return -1;

def canceltrade(account, index):
    try:
        c = ["node", "integration.js", "canceltrade"]
        c.append("--account " +account)
        c.append("--index " + str(index))
        return subprocess.check_output(c).decode("utf-8")
    except Exception as e:
        print(e)
        return -1;


# set name of cryptomon
def setname(account: str, name: str, index: int):
    try:
        c = ["node", "integration.js", "setname"]
        c.append("--account " + account)
        c.append("--name " + name)
        c.append("--index " + index)
        return subprocess.check_output(c).decode("utf-8")
    except Exception as e:
        print(e)
        return -1;

# apply item in player inventory to cryptomon
def itemapply(account: str, item: int, index: int):
    try:
        c = ["node", "integration.js", "itemapply"]
        c.append("--account " + account)
        c.append("--item " + str(item))
        c.append("--index " + str(index))
        return subprocess.check_output(c).decode("utf-8")
    except Exception as e:
        print(e)
        return -1;

# create player for account
# name = name of player
def upsertplayer(account: str, name: str):
    try:
        c = ["node", "integration.js", "upsertplayer"]
        c.append("--account " + account)
        c.append("--s " + name)
        return subprocess.check_output(c).decode("utf-8")
    except Exception as e:
        print(e)
        return -1;

# transfer funds from account to contract for use in-game
def transfer(account: str, amount: str, memo = ""):
    try:
        c = ["node", "integration.js", "transfer"]
        c.append("--account " + account)
        c.append("--quantity " + amount)
        c.append("--memo" + memo)
        return subprocess.check_output(c).decode("utf-8")
    except Exception as e:
        print(e)
        return -1;

def test():
    try:
        return subprocess.check_output(["node", "integration.js", "createmon"])
    except:
        return -1;

if __name__ == "__main__":
    print()
    #print('TEST:')
    # print(test())
    res = getallinfo("zvnxqtokmcqs")
    #print(type(res))
    print(res)
