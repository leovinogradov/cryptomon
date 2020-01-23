import requests
#url = "http://127.0.0.1:8888/v1/chain/get_account"

"""
url = "http://127.0.0.1:8888/v1/chain/get_table_rows"
payload = "{\"code\":\"cryptomon\",\"table\":\"players\",\"scope\":\"cryptomon\",\"index_position\":\"primary\",\"key_type\":\"name\", \"json\":\"true\", \"lower_bound\":\"alice\"}"
headers = {
    'accept': "application/json",
    'content-type': "application/json"
    }

response = requests.request("POST", url, data=payload, headers=headers)

print(response.text)


url = "http://127.0.0.1:8888/v1/chain/push_transactions"

payload = "[{\"actions\":[{\"account\":\"cryptomon\",\"name\":\"createmon\",\"authorization\":[{\"actor\":\"alice\",\"permission\":\"active\"}],\"data\":{\"acc\":\"alice\"}}]}]"
headers = {
    'accept': "application/json",
    'content-type': "application/json"
    }

response = requests.request("POST", url, data=payload, headers=headers)

print(response.text)
"""

url = "http://127.0.0.1:8888/v1/chain/get_block"
payload = "{\"block_num_or_id\":\"6658\"}"
headers = {
    'accept': "application/json",
    'content-type': "application/json"
    }

response = requests.request("POST", url, data=payload, headers=headers)

print(response.text)



url = "http://127.0.0.1:8888/v1/chain/get_info"

headers = {'accept': 'application/json'}

response = requests.request("POST", url, headers=headers)

print(response.text)


url = "http://127.0.0.1:8888/v1/chain/push_transactions"

payload = "[{\"expiration\":\"2020-01-23T12:50:30\",\"ref_block_num\":6658,\"ref_block_prefix\":2164473245,\"max_net_usage_words\":\"0\",\"max_cpu_usage_ms\":\"0\",\"delay_sec\":0,\"actions\":[{\"account\":\"cryptomon\",\"name\":\"createmon\",\"authorization\":[{\"actor\":\"alice\",\"permission\":\"active\"}],\"data\":{\"acc\":\"alice\"}}],\"transaction_extensions\":[]}]"
headers = {
    'accept': "application/json",
    'content-type': "application/json"
    }

response = requests.request("POST", url, data=payload, headers=headers)

print(response.text)
