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


def test():
    try:
        return subprocess.check_output(["node", "integration.js", "createmon"])
    except:
        return -1;

if __name__ == "__main__":
    print('TEST:')
    # print(test())
    print(createmon("test"))
