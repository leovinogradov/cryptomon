#include <cryptomon.hpp>

using eosio::contract;

//class [[eosio::contract("cryptomon")]] cryptomon: public eosio::contract{
  //public:
    cryptomon::cryptomon(eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds): contract(receiver, code, ds), mons_table(receiver, code.value), player_table(receiver, code.value), transact_table(receiver, code.value)
    {}
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
          row.cryptomon_index = 0;
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
      auto iterator = player_table.find(acc.value);

      if(iterator != player_table.end()){
        uint64_t c_index;
        if(!(player_table.get(acc.value).has_cryptomon)){

          mons_table.emplace(acc, [&](auto &row){ //creating cryptomon entry
            row.key = mons_table.available_primary_key();
            c_index = row.key;
            row.HP = 15;
            row.happiness = 15;
            row.health = 15;
            row.hunger = 15;
            row.start = eosio::current_time_point();
            row.current = eosio::current_time_point();
            row.level = 1;
            row.mon_name = "SET NAME";
          });

          player_table.modify(iterator, acc, [&](auto &row){ //
              row.cryptomon_index = c_index;
              row.has_cryptomon = true;
          });

        }
        else{
          /*condition must be changed when player has ability to own numerous
          cryptomons; need players to have upper limit personal amount of cryptomons
          */
          eosio::print("Player already has a cryptomon");
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
      eosio::check(player_iterator != player_table.end(), "Player does not exist");

      auto c_index = player_table.get(acc.value).cryptomon_index; //delete player
      auto transact_iterator = transact_table.find(acc.value);
      player_table.erase(player_iterator);

      auto cryptomons_iterator = mons_table.find(c_index); //find cryptomon associated with player
      mons_table.erase(cryptomons_iterator);

      if(transact_iterator != transact_table.end()){ //if there is an entry in market, delete it
        transact_table.erase(transact_iterator);
      }
    }

    [[eosio::action]]
    void cryptomon::deletemon(eosio::name acc){
        //require_auth(acc);
        auto iterator = player_table.find(acc.value);
        uint64_t c_index;
        if(iterator != player_table.end()){
          if(player_table.get(acc.value).has_cryptomon){

            player_table.modify(iterator, acc, [&](auto &row){
              row.has_cryptomon = false;
              c_index = row.cryptomon_index;
            });

            auto cryptomons_iterator = mons_table.find(c_index);
            mons_table.erase(cryptomons_iterator);
          }
        }
      }

    [[eosio::action]]
    void cryptomon::listmon(eosio::name acc, eosio::asset price, uint64_t delay){
      //require_auth(acc);
      auto player_iterator = player_table.find(acc.value);
      auto trans_iterator = transact_table.find(acc.value);
      if(player_iterator == player_table.end()){
        eosio::print("No record exists of this player!");
      }
      else{
        if(trans_iterator == transact_table.end()){
          //no entry in marketplace, create entry
          auto c_index = player_table.get(acc.value).cryptomon_index;
          transact_table.emplace(acc, [&](auto &row){
            row.cryptomon_index = c_index;
            row.account_one = acc;
            row.price = price;
            row.swap = false;

          });
          eosio::transaction t{};
          t.actions.emplace_back(eosio::permission_level{get_self(), "active"_n}, get_self(), "delistmon"_n, std::make_tuple(c_index));
          t.delay_sec = delay;
          t.send(acc.value, acc);
        }
        else{
            transact_table.modify(trans_iterator, acc, [&](auto &row){
            row.price = price;
          });
        }
      }
    }

    [[eosio::action]]
    void cryptomon::delistmon(uint64_t c_index){
      //require_auth(acc);

      auto iterator = transact_table.find(c_index);
      eosio::check(iterator != transact_table.end(), "Cryptomon listing does not exist!");
      transact_table.erase(iterator);
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

        eosio::check(transact_entry.account_two.value == 0, "This is a trade, not a listing");
        eosio::check(acc != seller, "Cannot perform action with self!");
        eosio::check(transact_iterator != transact_table.end(), "No record exists in transact table!");
        eosio::check(buyer_iterator != player_table.end(), "No record exists for buyer");
        eosio::check(buyer.funds.amount > transact_entry.price.amount, "Insufficient funds");

        if(buyer.funds.amount > transact_entry.price.amount && buyer_iterator != player_table.end()){
          player_table.modify(buyer_iterator, acc, [&](auto &row){
            row.cryptomon_index = c_index;
            row.has_cryptomon = true;
            row.funds = row.funds - transact_entry.price;
          });

          player_table.modify(seller_iterator, seller, [&](auto &row){
            row.has_cryptomon = false;
            row.cryptomon_index = 0; //the index of seller must be changed to indicate no cryptomon
            if(row.funds.amount == 0){
              row.funds = transact_entry.price;
            }
            else{
              row.funds += transact_entry.price;
            }
          });
        }
        eosio::action(eosio::permission_level{acc, "active"_n}, get_self(), "delistmon"_n, std::make_tuple(c_index)).send();
        eosio::cancel_deferred(seller.value);
        //eosio::action(eosio::permission_level{acc, "active"_n}, "eosio.token"_n, "transfer"_n, std::make_tuple(acc, seller, buyer_iterator->funds, std::string("Transferring funds!"))).send();
      }

    [[eosio::action]]
    void cryptomon::trademon(eosio::name account_one, eosio::name account_two){
        auto iterator_account_one = player_table.find(account_one.value);
        auto iterator_account_two = player_table.find(account_two.value);
        eosio::check(iterator_account_one != player_table.end(), "No record exists for account one!");
        eosio::check(iterator_account_two != player_table.end(), "No record exists for account two!");

        if(iterator_account_one != player_table.end() && iterator_account_two != player_table.end()){
          auto r1 = player_table.get(account_one.value);
          uint64_t c_index_one = r1.cryptomon_index;
          auto r2 = player_table.get(account_two.value);
          uint64_t c_index_two = r2.cryptomon_index;

          player_table.modify(iterator_account_one, account_one, [&](auto &row){
            row.cryptomon_index = c_index_two;
          });

          player_table.modify(iterator_account_two, account_two, [&] (auto &row){
            row.cryptomon_index = c_index_one;
          });
    }
  }

    [[eosio::action]]
    void cryptomon::canceltrade(uint64_t cryptomon_index){
      auto trade_itr = transact_table.find(cryptomon_index);
      if(trade_itr != transact_table.end()){
        transact_table.erase(trade_itr);
      }
      else{
        eosio::print("No trade made with this cryptomon!");
      }
    }

    [[eosio::action]]
    void cryptomon::inittrade(eosio::name account_one, eosio::name account_two, eosio::asset price, bool swap, uint64_t duration) {
      //require_auth(acct);
      auto account_one_itr = player_table.find(account_one.value);
      auto account_two_itr = player_table.find(account_two.value);
      if(account_one_itr == player_table.end() || account_two_itr == player_table.end()){
        eosio::print("One of the accounts does not exist");
        return;
      }
      else{
        auto entry_account_one = player_table.get(account_one.value);
        auto entry_account_two = player_table.get(account_two.value);
        auto trade_itr = transact_table.find(entry_account_one.cryptomon_index);
        if(!entry_account_two.has_cryptomon){
          eosio::print("Cannot initiate trade with player who has no cryptomon!");
          return;
        }
        if(entry_account_one.funds.amount >= price.amount){
          if(trade_itr == transact_table.end()){
              transact_table.emplace(account_one, [&](auto &entry){ // creating new trade
                entry.account_one = account_one;
                entry.account_two = account_two;
                if(swap){
                  if(entry_account_one.has_cryptomon){
                    entry.swap = true;
                    entry.cryptomon_index = entry_account_one.cryptomon_index;
                  }
                  else{
                    eosio::print("Do not have a cryptomon to swap!");
                    return;
                  }
                }
                else{
                  entry.cryptomon_index = 0;
                }
                entry.price = price;
              });
              eosio::transaction t{}; //delete trade after duration period
              t.actions.emplace_back(eosio::permission_level{get_self(), "active"_n}, get_self(), "canceltrade"_n, std::make_tuple(entry_account_one.cryptomon_index));
              t.delay_sec = duration;
              t.send(account_one.value, account_one);
            }
          else{
            eosio::cancel_deferred(account_one.value);
            transact_table.modify(trade_itr, account_one, [&](auto &row){
              row.account_two = account_two;
              row.price = price;
              if(swap){
                if(entry_account_one.has_cryptomon){
                  row.swap = true;
                  row.cryptomon_index = entry_account_one.cryptomon_index;
                }
                else{
                  eosio::print("Do not have a cryptomon to swap!");
                  return;
                }
              }
              else{
                row.cryptomon_index = 0;
              }
            });
            eosio::transaction t{}; //delete trade after duration period
            t.actions.emplace_back(eosio::permission_level{get_self(), "active"_n}, get_self(), "canceltrade"_n, std::make_tuple(entry_account_one.cryptomon_index));
            t.delay_sec = duration;
            t.send(account_one.value, account_one);
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
      uint64_t c_index_acceptor = acceptor.cryptomon_index;
      uint64_t c_index_initiator = initiator.cryptomon_index;

      eosio::check((c_index_initiator == trade_instance.cryptomon_index), "Invalid cryptomon index"); // should never happen but good to check anyway
      eosio::print(trade_instance.swap);
      if(acceptor_iterator != player_table.end()){
        player_table.modify(acceptor_iterator, account, [&](auto &row){
          if(row.funds.amount == 0){
            row.funds = trade_instance.price;
          }
          else{
            row.funds += trade_instance.price;
          }
          if(trade_instance.swap){
            row.cryptomon_index = c_index_initiator;
            row.has_cryptomon = true;
          }
          else{
            row.cryptomon_index = 0;
            row.has_cryptomon = false;
          }
        });

        player_table.modify(initiator_iterator, trade_instance.account_one, [&](auto &row){
            row.cryptomon_index = c_index_acceptor;
            row.has_cryptomon = true;
            row.funds -= trade_instance.price;
        });
        transact_table.erase(trade_itr);
      }
    }

    [[eosio::action]]
    void cryptomon::setname(eosio::name acc, std::string s){
      //require_auth(acc);
      auto iterator = player_table.find(acc.value);
      auto c_index = player_table.get(acc.value).cryptomon_index;

      if(iterator != player_table.end() && player_table.get(acc.value).has_cryptomon){
        auto cryptomons_iterator = mons_table.find(c_index);
        mons_table.modify(cryptomons_iterator, acc, [&](auto &row){
           row.mon_name = s;
        });
      }
    }

    /*

    [[eosio::action]]
    void cryptomon::interact(eosio::name acc){
      //require_auth(acc);
      auto iterator = player_table.find(acc.value);
      if(iterator != player_table.end() && player_table.get(acc.value).has_cryptomon){
        auto c_index = player_table.get(acc.value).cryptomon_index;
        auto cryptomons_iterator = mons_table.find(c_index);

        mons_table.modify(cryptomons_iterator, acc, [&](auto &row){
          row.HP += 5;
          row.emotionalState = 1;
        });
      }
      else{
        eosio::print("Player record does not exist!");
      }
    }
    */
    //WHEN DEVICE BOOTS UP, call this function
    [[eosio::action]]
    void cryptomon::interact(eosio::name acc){
      auto player_itr = player_table.find(acc.value);
      eosio::check(player_itr != player_table.end(), "Player does not exist");
      eosio::check(player_table.get(acc.value).has_cryptomon, "Player does not have a cryptomon");
      auto cryptomon_index = player_table.get(acc.value).cryptomon_index;
      auto cryptomon_itr = mons_table.find(cryptomon_index);
      eosio::time_point t1 = mons_table.get(cryptomon_index).current;
      eosio::time_point t2 = eosio::current_time_point();
      eosio::microseconds duration = t2 - t1; //getting difference in time
      eosio::print(duration.to_seconds());
      if(duration > eosio::minutes(3)){ //arbitrary, can set time to any point to take away c stats
        mons_table.modify(cryptomon_itr, acc, [&](auto &row){
          row.health -= 2;
          row.hunger -= 2;
          row.happiness -= 2;
          row.current = eosio::current_time_point();
        });
      }
    }

    [[eosio::action]]
    void cryptomon::itemapply(eosio::name acc, uint8_t item){
      auto acc_itr = player_table.find(acc.value);
      eosio::check(acc_itr != player_table.end(), "Player not found");
      auto acc_entry = player_table.get(acc.value);
      eosio::check(acc_entry.has_cryptomon, "Player does not have a cryptomon");
      auto cryptomon_itr = mons_table.find(acc_entry.cryptomon_index);
      if(!acc_entry.inventory.empty()){
        int index = findItem(item, acc_entry.inventory);
        if(index > -1){
          mons_table.modify(cryptomon_itr, acc, [&](auto &row){
            switch(item){
              case 0: //mush
                row.health -= 1;
                row.happiness += 1;
                row.hunger += 6;
                break;
              case 1: //seed
                row.health += 4;
                row.happiness += 1;
                row.hunger += 1;
                break;
              case 2: //hay
                row.health += 2;
                row.happiness += 1;
                row.hunger += 3;
                break;
              case 3: //cake
                row.health -= 2;
                row.happiness += 5;
                row.hunger += 3;
                break;
              case 4: //fudge
                row.health -= 3;
                row.happiness += 3;
                row.hunger += 6;
                break;
              case 5: //medicine
                row.health += 10;
                row.happiness -= 4;
                row.hunger += 0;
                break;
              case 6: //catnip
                row.health -= 3;
                row.happiness += 10;
                row.hunger -= 1;
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
      eosio::check(player_itr != player_table.end(), "Player does not exist");
      player_table.modify(player_itr, acc, [&](auto &row){
          std::vector<uint8_t> &v = row.inventory;
          uint8_t a = 1; //random item gen
          v.push_back(a);
      });
    }

    [[eosio::action]]
    void cryptomon::itemdelete(eosio::name acc, uint8_t item){
      auto player_itr = player_table.find(acc.value);
      eosio::check(player_itr != player_table.end(), "Player does not exist");
      auto acc_entry = player_table.get(acc.value);
      if(!acc_entry.inventory.empty()){
        int index = findItem(item, acc_entry.inventory);
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

    int cryptomon::findItem(uint8_t item, const std::vector<uint8_t> &v){
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
