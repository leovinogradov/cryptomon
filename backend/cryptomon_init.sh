#!/bin/bash
eosio-cpp -I include -o cryptomon.wasm src/cryptomon.cpp
sleep 3
nodeos -e -p eosio --delete-all-blocks --force-all-checks --plugin eosio::producer_plugin --plugin eosio::chain_api_plugin --plugin eosio::http_plugin --plugin eosio::history_plugin --plugin eosio::history_api_plugin --filter-on="*" --access-control-allow-origin='*' --contracts-console --http-validate-host=false --verbose-http-errors >> nodeos.log 2>&1 &
sleep 3
cleos create account eosio cryptomon EOS5LabEzJh4sStF5TyLGg718vs52dMyHhU978UUFvWKG9V2PuWPf -p eosio@active
sleep 2
cleos create account eosio bob EOS5LabEzJh4sStF5TyLGg718vs52dMyHhU978UUFvWKG9V2PuWPf -p eosio@active
sleep 2
cleos create account eosio alice EOS5LabEzJh4sStF5TyLGg718vs52dMyHhU978UUFvWKG9V2PuWPf -p eosio@active
sleep 2
cleos create account eosio eosio.token EOS5LabEzJh4sStF5TyLGg718vs52dMyHhU978UUFvWKG9V2PuWPf -p eosio@active
sleep 2
cleos create account eosio issuer EOS5LabEzJh4sStF5TyLGg718vs52dMyHhU978UUFvWKG9V2PuWPf -p eosio@active
sleep 2
cleos set account permission --add-code cryptomon active
sleep 2
#cleos set account permission --add-code alice active
#sleep 2
#cleos set account permission --add-code bob active
#sleep 2
cleos set contract cryptomon /home/cameron/contracts/cryptomon cryptomon.wasm cryptomon_custom.abi
sleep 5
cleos set contract eosio.token /home/cameron/contracts/eosio.contracts/contracts/eosio.token --abi eosio.token.abi -p eosio.token@active
sleep 5
cleos push action eosio.token create '[ "issuer", "1000000000.0000 EOS"]' -p eosio.token@active
sleep 3
cleos push action eosio.token issue '[ "issuer", "200.0000 EOS", "memo" ]' -p issuer@active
sleep 3
cleos push action eosio.token transfer '[ "issuer", "alice", "100.0000 EOS", "m" ]' -p issuer@active
sleep 3
cleos push action cryptomon upsertplayer '{"acc": "alice", "s": "AliceWonder"}' -p alice@active
sleep 3
cleos push action cryptomon upsertplayer '{"acc": "bob", "s": "BobdaBuilder"}' -p bob@active
sleep 2
cleos push action cryptomon createmon '{"acc": "bob"}' -p bob@active
sleep 2
cleos push action cryptomon createmon '{"acc": "alice"}' -p alice@active
sleep 2
cleos transfer alice cryptomon "25.0000 EOS" "Transfer!" -p alice@active
sleep 2
cleos push action cryptomon inittrade '{"account_one": "alice", "account_two": "bob", "price": "10.0000 EOS", "swap": true, "duration": "10000"}' -p alice@active
