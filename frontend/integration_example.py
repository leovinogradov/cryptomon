import requests
#url = "http://127.0.0.1:8888/v1/chain/get_account"

url = "http://127.0.0.1:8888/v1/chain/get_table_rows"
payload = "{\"code\":\"cryptomon\",\"table\":\"players\",\"scope\":\"cryptomon\",\"index_position\":\"primary\",\"key_type\":\"name\", \"json\":\"true\", \"lower_bound\":\"alice\"}"
headers = {
    'accept': "application/json",
    'content-type': "application/json"
    }

response = requests.request("POST", url, data=payload, headers=headers)

print(response.text)
