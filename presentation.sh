echo "cleos push action cryptomon upsertplayer '{"acc": "alice", "s": "Alice"}' -p alice@active"
echo "cleos push action cryptomon upsertplayer '{"acc": "bob", "s": "Bob"}' -p bob@active"
echo "cleos push action cryptomon createmon '{"acc": "bob"}' -p bob@active"
echo "cleos push action cryptomon createmon '{"acc": "alice"}' -p alice@active"
echo "cleos transfer alice cryptomon "25.0000 EOS" "Transfer!" -p alice@active"
echo "cleos push action cryptomon inittrade '{"account_one": "alice", "account_two": "bob", "price": "10.0000 EOS", "swap": true, "duration": "10000"}' -p alice@active"
cleos push action cryptomon upsertplayer '{"acc": "alice", "s": "Alice"}' -p alice@active
sleep 3
cleos push action cryptomon upsertplayer '{"acc": "bob", "s": "Bob"}' -p bob@active
sleep 2
cleos push action cryptomon createmon '{"acc": "bob"}' -p bob@active
sleep 2
cleos push action cryptomon createmon '{"acc": "alice"}' -p alice@active
sleep 2
cleos transfer alice cryptomon "25.0000 EOS" "Transfer!" -p alice@active
sleep 2
cleos push action cryptomon inittrade '{"account_one": "alice", "account_two": "bob", "price": "10.0000 EOS", "swap": true, "duration": "10000"}' -p alice@active
