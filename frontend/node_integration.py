# Approach 1 would be to use the Naked library. However, lets try without external libraries first

import subprocess

# @Input account: string
def createmon(account):
    try:
        c = ["node", "integration.js", "createmon"]
        c.append("--account="+account) # command is separated for readablity
        return subprocess.check_output(c)
    except Exception as e:
        print(e)
        return -1;

def deletemon(account, index):
    try:
        c = ["node", "integration.js", "deletemon"]
        c.append("--account="+account)
        c.append("--index="+index)
        return subprocess.check_output(c)
    except Exception as e:
        print(e)
        return -1;

# List Cryptomon for sale
# asset = the price of the cryptomon in the format [asset type]
# delay = duration of sale
# index = index of Cryptomon
def listmon(account: string, asset: string, delay: number, index: number):
    try:
        c = ["node", "integration.js", "listmon"]
        c.append("--account="+account)
        c.append("--asset="+asset)
        c.append("--delay="+delay)
        c.append("--index="+index)
        return subprocess.check_output(c)
    except Exception as e:
        print(e)
        return -1;

# Remove cryptomon from trade table
def delistmon(account: string, index: number):
    try:
        c = ["node", "integration.js", "delistmon"]
        c.append("--account="+account)
        c.append("--index="+index)
        return subprocess.check_output(c)
    except Exception as e:
        print(e)
        return -1;

def purchasemon(account: string, index: number):
    try:
        c = ["node", "integration.js", "purchasemon"]
        c.append("--account="+account)
        c.append("--index="+index)
        return subprocess.check_output(c)
    except Exception as e:
        print(e)
        return -1;

def accepttrade(account: string, index: number):
    try:
        c = ["node", "integration.js", "accepttrade"]
        c.append("--account="+account)
        c.append("--index="+index)
        return subprocess.check_output(c)
    except Exception as e:
        print(e)
        return -1;

def inittrade(account1: string, account2: string, price: string, swap: boolean, duration: number, c1: number, c2: number):
    try:
        c = ["node", "integration.js", "inittrade"]
        c.append("--account1="+account1)
        c.append("--account2="+account2)
        c.append("--price="+price)
        c.append("--swap="+swap)
        c.append("--duration="+duration)
        c.append("--c1="+c1)
        c.append("--c2="+c2)
        return subprocess.check_output(c)
    except Exception as e:
        print(e)
        return -1;

def canceltrade(account: string, index: number):
    try:
        c = ["node", "integration.js", "canceltrade"]
        c.append("--account="+account)
        c.append("--index="+index)
        return subprocess.check_output(c)
    except Exception as e:
        print(e)
        return -1;


# set name of cryptomon
def setname(account: string, name: string, index: number):
    try:
        c = ["node", "integration.js", "setname"]
        c.append("--account="+account)
        c.append("--name="+name)
        c.append("--index="+index)
        return subprocess.check_output(c)
    except Exception as e:
        print(e)
        return -1;

# apply item in player inventory to cryptomon
def itemapply(account: string, item: number, index: number):
    try:
        c = ["node", "integration.js", "itemapply"]
        c.append("--account="+account)
        c.append("--item="+item)
        c.append("--index="+index)
        return subprocess.check_output(c)
    except Exception as e:
        print(e)
        return -1;

# create player for account
# name = name of player
def upsertplayer(account: string, name: string):
    try:
        c = ["node", "integration.js", "upsertplayer"]
        c.append("--account="+account)
        c.append("--s="+name)
        return subprocess.check_output(c)
    except Exception as e:
        print(e)
        return -1;

def test():
    try:
        return subprocess.check_output(["node", "integration.js", "createmon"])
    except:
        return -1;

if __name__ == "__main__":
    print('TEST:')
    # print(test())
    print(createmon("test"))
