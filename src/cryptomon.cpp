#include <cryptomon.hpp>
#include "Random.h"

using eosio::contract;

    cryptomon::cryptomon(eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds): contract(receiver, code, ds), mons_table(receiver, code.value), player_table(receiver, code.value), transact_table(receiver, code.value)
    {
      //upsertplayer(receiver, "cryptomon");
      //createmon(receiver);
      //setname(receiver, "genesis", 0);
    }
    //applying an item to specified cryptomon for wanted effects
    [[eosio::action]]
    void cryptomon::upsertplayer(eosio::name acc, std::string s){
      //require_auth(acc);
      auto iterator = player_table.find(acc.value);
      if(iterator == player_table.end()){
        //Player is not in the table, create entry
        player_table.emplace(acc, [&](auto &row){
          row.key = acc;
          row.playerName = s;
          row.has_cryptomon = false;
          //row.cryptomon_indexes.push_back(0);
        });
      }
      else{
        //Player is already in the table, modify state
        player_table.modify(iterator, acc, [&](auto &row){
          row.playerName = s;
        });
      }
    }

    [[eosio::action]]
    void cryptomon::createmon(eosio::name acc){
      //require_auth(acc);
      eosblox::Random gen;
      gen.accumSeed(now());
      int type = gen.next()%16;
      auto iterator = player_table.find(acc.value);

      if(iterator != player_table.end()){
        uint64_t c_index;
        if(player_table.get(acc.value).cryptomon_indexes.size() < 5){

          mons_table.emplace(acc, [&](auto &row){ //creating cryptomon entry
            row.key = mons_table.available_primary_key();
            c_index = row.key;
            row.happiness = 15;
            row.health = 15;
            row.hunger = 15;
            row.head = 1;
            row.torso = 1;
            row.type = type;
            row.start = eosio::current_time_point();
            row.current = eosio::current_time_point();
            row.mon_name = "SET NAME";
          });

          player_table.modify(iterator, acc, [&](auto &row){ //
              //row.cryptomon_index = c_index;
              //row.cryptomon_indexes.erase(cryptomon_indexes.begin());
              row.cryptomon_indexes.emplace_back(c_index);
              row.has_cryptomon = true;
          });

        }
        else{
          /*condition must be changed when player has ability to own numerous
          cryptomons; need players to have upper limit personal amount of cryptomons
          */
          eosio::print("Player already has reached upper-limit of cryptomon inventory amount");
        }
      }
    }

    [[eosio::on_notify("eosio.token::transfer")]]
    void cryptomon::playerfund(eosio::name acc, eosio::name to, eosio::asset quantity, std::string memo){
      //require_auth(acc);
      if(acc == get_self() || to != get_self()){
        eosio::print("This is an illegal operation");
        return;
      }
      eosio::check(quantity.amount > 0, "Do not have any amount of tokens to deposit");
      eosio::check(quantity.symbol == currency_symbol, "Cannot accept this token!");
      p_data players(get_self(), get_self().value);
      auto iterator = players.find(acc.value);
      eosio::check(iterator != players.end(), "Player record does not exist!");
      if(iterator != players.end()){
        players.modify(iterator, acc, [&](auto &row){
          if(row.funds.amount == 0){
            row.funds = quantity;
          }
          else{
            row.funds += quantity;
          }
        });
      }
    }

    [[eosio::action]]
    void cryptomon::deleteplayer(eosio::name acc){
      //require_auth(acc);
      auto player_iterator = player_table.find(acc.value);
      auto player_entry = player_table.get(acc.value);
      eosio::check(player_iterator != player_table.end(), "Player does not exist");

      //auto c_index = player_table.get(acc.value).cryptomon_index; //delete player
      for(uint64_t c_index : player_table.get(acc.value).cryptomon_indexes){
        auto cryptomons_iterator = mons_table.find(c_index); //find cryptomon associated with player
        mons_table.erase(cryptomons_iterator);
      }

      auto transact_iterator = transact_table.find(acc.value);
      if(player_entry.funds.amount > 0){
        eosio::action(eosio::permission_level{get_self(), "active"_n}, "eosio.token"_n, "transfer"_n, std::make_tuple(get_self(), acc, player_entry.funds, std::string("Withdrawing all funds!"))).send();
      }
      player_table.erase(player_iterator);

      if(transact_iterator != transact_table.end()){ //if there is an entry in market, delete it
        transact_table.erase(transact_iterator);
      }
    }

    [[eosio::action]]
    void cryptomon::deletemon(eosio::name acc, uint64_t cryptomon_index){
        //require_auth(acc);
        auto iterator = player_table.find(acc.value);
        //uint64_t c_index;
        if(iterator != player_table.end()){
          if(player_table.get(acc.value).has_cryptomon){

            player_table.modify(iterator, acc, [&](auto &row){
              if(row.cryptomon_indexes.size() > 0){
                int location = findItem(cryptomon_index, row.cryptomon_indexes);
                if(location > -1){
                  row.cryptomon_indexes.erase(row.cryptomon_indexes.begin() + location);
                  auto transact_itr = transact_table.find(cryptomon_index);
                  if(transact_itr != transact_table.end()){
                    transact_table.erase(transact_itr);
                  }
                }
                else{
                  eosio::print("Cannot delete Cryptomon that you do not possess!");
                  return ;
                }
                row.has_cryptomon = (row.cryptomon_indexes.size() == 0)? false:true;
                //c_index = row.cryptomon_index;
                //row.has_cryptomon = false;
            }
            });

            auto cryptomons_iterator = mons_table.find(cryptomon_index);
            mons_table.erase(cryptomons_iterator);
          }
        }
      }

    [[eosio::action]]
    void cryptomon::listmon(eosio::name acc, eosio::asset price, uint64_t cryptomon_index){
      //require_auth(acc);
      auto player_iterator = player_table.find(acc.value);
      auto player_entry = player_table.get(acc.value);
      auto trans_iterator = transact_table.find(cryptomon_index);
      if(player_iterator == player_table.end()){
        eosio::print("No record exists of this player!");
      }
      else{
        if(trans_iterator == transact_table.end()){
          //no entry in marketplace, create entry
          //auto c_index = player_table.get(acc.value).cryptomon_index;
          const std::vector<uint64_t> &c = player_entry.cryptomon_indexes;
          int location = findItem<uint64_t>(cryptomon_index, c);
          eosio::print(cryptomon_index);

          if(location > -1){
            transact_table.emplace(acc, [&](auto &row){
              row.cryptomon_index2 = cryptomon_index;
              row.account_one = acc;
              row.price = price;
              row.swap = false;
            });
          }
          else{
            eosio::print("Cannot sell Cryptomon that you do not possess!");
            return;
          }
        }
        else{
            transact_table.modify(trans_iterator, acc, [&](auto &row){
            row.price = price;
          });
        }
      }
    }

    [[eosio::action]]
    void cryptomon::delistmon(eosio::name acc, uint64_t cryptomon_index){
      //require_auth(acc);

      auto trans_itr = transact_table.find(cryptomon_index);
      //eosio::check(iterator != transact_table.end(), "Cryptomon listing does not exist!");
      eosio::check(transact_table.get(cryptomon_index).account_one == acc, "Cannot remove sell event that you did not initiate!");
      if(trans_itr != transact_table.end() && acc == transact_table.get(cryptomon_index).account_one){
        transact_table.erase(trans_itr);
      }

    }

    [[eosio::action]]
    void cryptomon::purchasemon(eosio::name acc, uint64_t c_index){
        //require_auth(buyer);
        auto buyer_iterator = player_table.find(acc.value);
        // auto market_iterator = market_table.find(c_index);
        auto transact_iterator = transact_table.find(c_index);
        auto transact_entry = transact_table.get(c_index);
        eosio::name seller = transact_entry.account_one;
        auto seller_iterator = player_table.find(seller.value);
        auto buyer = player_table.get(acc.value);
        if(acc == seller){
          eosio::print("Cannot buy/sell with self");
          return;
        }

        if(buyer.cryptomon_indexes.size() >= 5){
          eosio::print("Cannot purchase cryptomon due to having maximum allowed amount of cryptomon (5)");
          return;
        }

        eosio::check(transact_entry.account_two.value == 0, "This is a trade, not a listing");
        eosio::check(acc != seller, "Cannot perform action with self!");
        eosio::check(transact_iterator != transact_table.end(), "No record exists in transact table!");
        eosio::check(buyer_iterator != player_table.end(), "No record exists for buyer");
        eosio::check(buyer.funds.amount > transact_entry.price.amount, "Insufficient funds");

        if(buyer.funds.amount > transact_entry.price.amount && buyer_iterator != player_table.end()){
          player_table.modify(buyer_iterator, acc, [&](auto &row){
            //row.cryptomon_index = c_index;
            row.cryptomon_indexes.emplace_back(c_index);
            row.has_cryptomon = true;
            row.funds = row.funds - transact_entry.price;
          });

          player_table.modify(seller_iterator, seller, [&](auto &row){
            //row.has_cryptomon = false;
            int location = findItem<uint64_t>(c_index, row.cryptomon_indexes);
            if(location > -1){
              row.cryptomon_indexes.erase(row.cryptomon_indexes.begin() + location);
              row.has_cryptomon = (row.cryptomon_indexes.size() == 0)? false: true;
            }
            if(row.funds.amount == 0){
              row.funds = transact_entry.price;
            }
            else{
              row.funds += transact_entry.price;
            }
          });
        }
        eosio::action(eosio::permission_level{acc, "active"_n}, get_self(), "delistmon"_n, std::make_tuple(transact_entry.account_one, c_index)).send();
        //eosio::action(eosio::permission_level{acc, "active"_n}, "eosio.token"_n, "transfer"_n, std::make_tuple(acc, seller, buyer_iterator->funds, std::string("Transferring funds!"))).send();
      }

    [[eosio::action]]
    void cryptomon::canceltrade(eosio::name account, uint64_t cryptomon_index){
      auto trade_itr = transact_table.find(cryptomon_index);
      auto trade_entry = transact_table.get(cryptomon_index);
      if(trade_itr != transact_table.end() && (account == trade_entry.account_one || account == trade_entry.account_two)){
        transact_table.erase(trade_itr);
      }
      else{
        eosio::print("No trade made with this cryptomon!");
      }
    }

    [[eosio::action]]
    void cryptomon::inittrade(eosio::name account_one, eosio::name account_two, eosio::asset price, bool swap, uint64_t c1 = 0, uint64_t c2 = 0) {
      //require_auth(account_one);
      auto account_one_itr = player_table.find(account_one.value);
      auto account_two_itr = player_table.find(account_two.value);
      if(account_one_itr == player_table.end() || account_two_itr == player_table.end()){
        eosio::print("One of the accounts does not exist");
        return;
      }
      else{
        auto entry_account_one = player_table.get(account_one.value);
        auto entry_account_two = player_table.get(account_two.value);
        auto trade_itr = transact_table.find(c2);
        //auto trade_itr = transact_table.find(entry_account_one.cryptomon_index);
        if(!entry_account_two.has_cryptomon || findItem<uint64_t>(c2, entry_account_two.cryptomon_indexes) == -1){
          eosio::print("Cannot initiate trade with player who does not have specified cryptomon!");
          return;
        }
        if(entry_account_one.funds.amount >= price.amount){
          if(trade_itr == transact_table.end()){
              transact_table.emplace(account_one, [&](auto &entry){ // creating new trade
                entry.account_one = account_one;
                entry.account_two = account_two;
                entry.cryptomon_index2 = c2;
                if(swap){
                  if(entry_account_one.has_cryptomon && findItem<uint64_t>(c1, entry_account_one.cryptomon_indexes) > -1){
                    entry.swap = true;
                    entry.cryptomon_index = c1;
                    //entry.cryptomon_index = entry_account_one.cryptomon_index;
                  }
                  else{
                    eosio::print("Do not have a cryptomon to swap!");
                    return;
                  }
                }
                else{
                  if(entry_account_one.cryptomon_indexes.size() >= 5){
                    eosio::print("Cannot offer tokens for cryptomon since the maximum amount of cryptomon has already been reached (5)");
                    return;
                  }
                  entry.cryptomon_index = 0;
                }
                entry.price = price;
              });
            }
          else{
            if(account_one == transact_table.get(c2).account_one){
              transact_table.modify(trade_itr, account_one, [&](auto &row){
                if(findItem<uint64_t>(c2, entry_account_two.cryptomon_indexes) > -1){
                  row.account_two = account_two;
                  row.price = price;
                  row.cryptomon_index2 = c2;
                }
                else{
                  eosio::print("Cannot initiate trade with player who does not have specified cryptomon!");
                  return;
                }
                if(swap){
                  if(entry_account_one.has_cryptomon && findItem<uint64_t>(c1, entry_account_one.cryptomon_indexes) > -1){
                    row.swap = true;
                    row.cryptomon_index = c1;
                    //row.cryptomon_index = entry_account_one.cryptomon_index;
                  }
                  else{
                    eosio::print("Do not have that cryptomon to swap!");
                    return;
                  }
                }
                else{
                  row.cryptomon_index = 0;
                }
              });
            }
          }
        }
          else{
            eosio::print("Lack adequate funds for offer amount!");
          }
        }
      }

    [[eosio::action]]
    void cryptomon::accepttrade(eosio::name account, uint64_t cryptomon_index){
      //require_auth(account); // acct in this case is account of buyer

      auto trade_itr = transact_table.find(cryptomon_index);
      eosio::check(trade_itr != transact_table.end(), "No such trade exists");

      auto trade_instance = transact_table.get(cryptomon_index);
      eosio::check((trade_instance.account_two.value != 0 && trade_instance.account_two == account), "This trade is not for you");
      //eosio::check(!trade_instance.sold, "This trade has already been completed"); //issue, if same cryptomon is to be traded again, .sold would be true

      auto acceptor_iterator = player_table.find(account.value);
      auto initiator_iterator = player_table.find(trade_instance.account_one.value);
      eosio::check(acceptor_iterator != player_table.end(), "No record exists for buyer");
      eosio::check(initiator_iterator != player_table.end(), "No record exists for seller!");

      auto acceptor = player_table.get(account.value);
      auto initiator = player_table.get(trade_instance.account_one.value);
      uint64_t c_index_acceptor = trade_instance.cryptomon_index2;
      uint64_t c_index_initiator = trade_instance.cryptomon_index;
      //uint64_t c_index_acceptor = acceptor.cryptomon_index;
      //uint64_t c_index_initiator = initiator.cryptomon_index;
      //eosio::check((c_index_initiator == trade_instance.cryptomon_index), "Invalid cryptomon index"); // should never happen but good to check anyway
      if(acceptor_iterator != player_table.end()){
        player_table.modify(acceptor_iterator, account, [&](auto &row){
          int location = findItem(c_index_acceptor, acceptor.cryptomon_indexes);
          row.cryptomon_indexes.erase(row.cryptomon_indexes.begin() + location);

          if(row.funds.amount == 0){
            row.funds = trade_instance.price;
          }
          else{
            row.funds += trade_instance.price;
          }
          if(trade_instance.swap){
            //row.cryptomon_index = c_index_initiator;
            row.cryptomon_indexes.emplace_back(c_index_initiator);
            row.has_cryptomon = true;
          }
          else{
            //row.cryptomon_index = 0;
            row.has_cryptomon = (row.cryptomon_indexes.size() <= 0)? 0: 1;
          }
        });

        player_table.modify(initiator_iterator, trade_instance.account_one, [&](auto &row){
            //row.cryptomon_index = c_index_acceptor;
            row.cryptomon_indexes.emplace_back(c_index_acceptor);
            row.has_cryptomon = true;
            row.funds -= trade_instance.price;
            if(trade_instance.swap){
              int location = findItem<uint64_t>(c_index_initiator, row.cryptomon_indexes);
              row.cryptomon_indexes.erase(row.cryptomon_indexes.begin() + location);
            }
        });
        transact_table.erase(trade_itr);
      }
    }

    [[eosio::action]]
    void cryptomon::setname(eosio::name acc, std::string s, uint64_t cryptomon_index){
      //require_auth(acc);
      auto iterator = player_table.find(acc.value);
      auto player_entry = player_table.get(acc.value);
      auto cryptomons_iterator = mons_table.find(cryptomon_index);
      int location = findItem<uint64_t>(cryptomon_index, player_entry.cryptomon_indexes);
      eosio::check(iterator != player_table.end(), "player does not exist");
      eosio::check(location > -1, "cannot find cryptomon");
      eosio::check(cryptomons_iterator != mons_table.end(), "cryptomon does not exist!");
      eosio::check(player_entry.has_cryptomon, "cannot change name of cryptomon that you do not have");
      //auto c_index = player_table.get(acc.value).cryptomon_index;

      if(iterator != player_table.end()){
        if(player_entry.has_cryptomon && location > -1){
          mons_table.modify(cryptomons_iterator, acc, [&](auto &row){
             row.mon_name = s;
          });
        }
        else{
          eosio::print("Cannot change cryptomon which does not belong to you!");
        }
      }
    }

    //WHEN DEVICE BOOTS UP, call this function
    [[eosio::action]]
    void cryptomon::interact(eosio::name acc, uint64_t cryptomon_index){
      auto player_itr = player_table.find(acc.value);
      auto player_entry = player_table.get(acc.value);
      eosio::check(player_itr != player_table.end(), "Player does not exist");
      eosio::check(player_table.get(acc.value).has_cryptomon, "Player does not have a cryptomon");
      //auto cryptomon_index = player_table.get(acc.value).cryptomon_index;
      if(findItem<uint64_t>(cryptomon_index, player_entry.cryptomon_indexes) == -1){
        eosio::print("Cannot interact with Cryptomon that you do not possess!");
        return ;
      }
      auto cryptomon_itr = mons_table.find(cryptomon_index);
      eosio::time_point t1 = mons_table.get(cryptomon_index).current;
      eosio::time_point t2 = eosio::current_time_point();
      eosio::microseconds duration = t2 - t1; //getting difference in time
      eosio::print(duration.to_seconds());
      if(duration > eosio::minutes(1440)){ //arbitrary, can set time to any point to take away c stats
        int multiplier = duration.to_seconds()/eosio::minutes(1440).to_seconds();
        mons_table.modify(cryptomon_itr, acc, [&](auto &row){
          row.health -= 1*multiplier;
          row.hunger -= 1*multiplier;
          row.happiness -= 1*multiplier;
          row.current = eosio::current_time_point();
        });
      }
    }

    [[eosio::action]]
    void cryptomon::itemapply(eosio::name acc, uint8_t item, uint64_t cryptomon_index){
      auto acc_itr = player_table.find(acc.value);
      eosio::check(acc_itr != player_table.end(), "Player not found");
      auto acc_entry = player_table.get(acc.value);
      eosio::check(acc_entry.has_cryptomon, "Player does not have a cryptomon");
      eosio::check(findItem<uint64_t>(cryptomon_index, acc_entry.cryptomon_indexes) > -1, "Cannot apply item to a Cryptomon you do not possess!");
      auto cryptomon_itr = mons_table.find(cryptomon_index);
      //auto cryptomon_itr = mons_table.find(acc_entry.cryptomon_index);
      if(!acc_entry.inventory.empty()){
        int index = findItem<uint8_t>(item, acc_entry.inventory);
        if(index > -1){
          mons_table.modify(cryptomon_itr, acc, [&](auto &row){
            switch(item){
              case 0: //mush
                row.health = (row.health > 0)? row.health-1 : 0;
                row.happiness = (row.happiness < 15)? row.happiness + 1 : 15;
                row.hunger = (row.hunger + 6 <= 15)? row.hunger + 6: 15;
                break;
              case 1: //seed
                row.health = (row.health + 4 <= 15)? row.health + 4 : 15;
                row.happiness = (row.happiness < 15)? row.happiness + 1 : 15;
                row.hunger = (row.hunger < 15)? row.hunger + 1: 15;
                break;
              case 2: //hay
                row.health = (row.health + 2 <= 15)? row.health + 2 : 15;
                row.happiness = (row.happiness < 15)? row.happiness + 1 : 15;
                row.hunger = (row.hunger + 3 <= 15)? row.hunger + 3: 15;
                break;
              case 3: //cake
                row.health = (row.health - 2 >= 0)? row.health - 2 : 0;
                row.happiness = (row.happiness + 5 <= 15)? row.happiness + 5 : 15;
                row.hunger = (row.hunger + 3 <= 15)? row.hunger + 3: 15;
                break;
              case 4: //fudge
                row.health = (row.health - 3 >= 0)? row.health - 3 : 0;
                row.happiness = (row.happiness + 3 <= 15)? row.happiness + 3 : 15;
                row.hunger = (row.hunger + 6 <= 15)? row.hunger + 6: 15;
                break;
              case 5: //medicine
                row.health = (row.health + 10 <= 15)? row.health + 10 : 0;
                row.happiness = (row.happiness - 4 >= 0)? row.happiness - 4 : 0;
                row.hunger += 0;
                break;
              case 6: //catnip
                row.health = (row.health - 3 >= 0)? row.health - 3 : 0;
                row.happiness = (row.happiness + 10 <= 15)? row.happiness + 10 : 15;
                row.hunger = (row.hunger > 0)? row.hunger - 1: 0;
                break;
              default:
                break;
            }
          });
          player_table.modify(acc_itr, acc, [&](auto &row){
              row.inventory.erase(row.inventory.begin() + index);
          });
        }
        else{
          eosio::print("This item does not exist in inventory");
        }
      }
      else{
        eosio::print("No item in inventory");
      }
    }

    [[eosio::action]]
    void cryptomon::itemacquire(eosio::name acc){
      auto player_itr = player_table.find(acc.value);
      auto player_entry = player_table.get(acc.value);
      eosblox::Random gen;
      gen.accumSeed(now());
      int item = gen.next()%7;
      eosio::check(player_itr != player_table.end(), "Player does not exist");
      eosio::check(player_entry.inventory.size() < 30, "cannot acquire item since maximum capacity of inventory has been reached");
      if(player_entry.inventory.size() >= 30){
        eosio::print("cannot acquire item since maximum capacity of inventory has been reached");
        return;
      }
      else{
        player_table.modify(player_itr, acc, [&](auto &row){
            std::vector<uint8_t> &v = row.inventory;
            uint8_t a = item;
            v.push_back(a);
        });
      }
      /*eosio::transaction t{};
      t.actions.emplace_back(eosio::permission_level{get_self(), "active"_n}, get_self(), "itemacquire"_n, std::make_tuple(acc));
      t.delay_sec = 86400;
      t.send(acc.value, acc);
      */
    }

    [[eosio::action]]
    void cryptomon::itemdelete(eosio::name acc, uint8_t item){
      auto player_itr = player_table.find(acc.value);
      eosio::check(player_itr != player_table.end(), "Player does not exist");
      auto acc_entry = player_table.get(acc.value);
      if(!acc_entry.inventory.empty()){
        int index = findItem<uint8_t>(item, acc_entry.inventory);
        if(index > -1){
          player_table.modify(player_itr, acc, [&](auto &row){
            std::vector<uint8_t> &v = row.inventory;
            v.erase(v.begin() + index);
          });
        }
        else{
          eosio::print("This item does not exist in inventory");
        }
      }
      else{
        eosio::print("No item in inventory");
      }
    }

    [[eosio::action]]
    void cryptomon::itembuy(eosio::name account, uint8_t select){
      auto player_itr = player_table.find(account.value);
      auto player_entry = player_table.get(account.value);
      eosio::asset cost(5000, currency_symbol);
      eosio::check(player_itr != player_table.end(), "Player does not exist");
      eosio::check(player_entry.inventory.size() < 30, "Cannot exceed maximum capacity for items (30)");
      eosio::check(select/6 <= 1, "select of greater than 6 does not exist");
      eosio::check(player_entry.funds >= cost, "Lack adequate funds for buying item");
      //eosio::check(amount <= player_entry.funds, "Do not have enough funds for the amount chosen!");
      if(player_itr != player_table.end()) {
        if(player_entry.inventory.size() < 30){
          if(select/6 <= 1){
            if(player_entry.funds >= cost){
              player_table.modify(player_itr, account, [&](auto &row){
                    row.inventory.push_back(select);
                    row.funds -= cost;
              });
            }
          }
        }
      }
    }

    template <typename T>
    int cryptomon::findItem(T item, const std::vector<T> &v){
      for(int i = 0; i < v.size(); i++){
        if (v[i] == item){
          return i;
        }
      }
      return -1;
    }

    [[eosio::action]]
    void cryptomon::monversion(){
      eosio::print("VERSION 1.0 Time: ");
      eosio::print(cryptomon::now());
    }

    [[eosio::action]]
    void cryptomon::withdraw(eosio::name acc, eosio::asset amount){
      auto player_itr = player_table.find(acc.value);
      eosio::check(player_itr != player_table.end(), "Player does not exist");
      auto player_entry = player_table.get(acc.value);
      if(amount.amount > player_entry.funds.amount){
        eosio::print("Cannot withdraw more than what is in account!");
        return;
      }
      else{
        player_table.modify(player_itr, acc, [&](auto &row){
          row.funds = row.funds - amount;
        });
        eosio::action(eosio::permission_level{get_self(), "active"_n}, "eosio.token"_n, "transfer"_n, std::make_tuple(get_self(), acc, amount, std::string("Withdrawing funds!"))).send();
      }

    }

    uint32_t cryptomon::now(){
      eosio::time_point t = eosio::current_time_point();
      eosio::microseconds curr = t.elapsed;
      return t.sec_since_epoch();
      //return curr._count;
      //return eosio::current_time_point();
    }

    /*
      WORKING WITH TIME:
      eosio::microseconds h = hours(uint);
      eosio::microseconds m = minutes(uint);
      eosio::microseconds s = seconds(uint);
      eosio::time_point t = eosio::current_time_point();

      if user puts time up for pending trade:
      uint32_t expiration = t.sec_since_epoch() + h.to_seconds() + m.to_seconds() + s.to_seconds();
      if(t.sec_since_epoch() or now() > expiration){
          trade is voided
      }
      NOTE: time point is time elapsed since january 1st, 1970
    */


//private:
//look into vectors of defined user-type, for now will limit accounts to having one list_cryptomo
//};
