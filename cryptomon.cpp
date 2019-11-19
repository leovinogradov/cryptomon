#include "cryptomon/cryptomon.hpp"

using eosio::contract;

class [[eosio::contract("cryptomon")]] cryptomon: public eosio::contract{
  public:
    cryptomon(eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds): contract(receiver, code, ds), cryptomon::_cryptomons(receiver, code.value), cryptomon::_player(receiver, code.value), cryptomon::_market(receiver, code.value)
    {}
    //applying an item to specified cryptomon for wanted effects
    [[eosio::action]]
    void upsertplayer(eosio::name acc, std::string name){
      require_auth(user);
      auto iterator = _player.find(acc.value);
      if(iterator == _player.end()){
        //Player is not in the table, create entry
        _player.emplace(acc, [&](auto &row){
          row.key = acc;
          row.playerName = name;
          row.has_cryptomon = false;
          //row.playerId = _player.available_primary_key();
          //cryptomon a = {10, 1, 1, 150, emotion::HAPPY, "NO NAME"};
          //row.monster = a;
        });
      }
      else{
        //Player is already in the table, modify state
        _player.modify(iterator, acc, [&](auto &row){
          row.playerName = name;
          //row.monster.name = name;
        });
      }
    }

    [[eosio::action]]
    void createmon(eosio::name acc){
      require_auth(acc);
      auto iterator = _player.find(acc.value);
      if(iterator != _player.end()){
        uint64_t c_index;
        if(!(_player.get(iterator).has_cryptomon)){

          _cryptomons.emplace(acc, [&](auto &row){ //creating cryptomon entry
            row.index = row.index+1;
            c_index = row.index;
            cryptomon a = {10, 1, 1, 150, 0, "NO NAME"};
            row.monster = a;
          });

          _player.modify(iterator, acc, [&](auto &row){ //
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
    void addfunds(eosio::name acc, eosio::asset deposit){
      require_auth(acc);
      if(acc == get_self()){
        eosio::print("This is an illegal operation");
      }
      auto iterator = _player.find(acc.value);
      check(iterator != _player.end(), "Player record does not exist!");
      if(iterator != _player.end()){
        _player.modify(iterator, acc, [&](auto &row){
          row.funds += deposit;
        });
      }
    }

    [[eosio::action]]
    void deleteplayer(eosio::name acc){
      require_auth(acc);
      auto player_iterator = _player.find(acc.value);
      auto marketplace_iterator = _market.find(acc.value);
      check(player_iterator != _player.end(), "Player does not exist");

      auto c_index = _player.get(player_iterator).cryptomon_index; //delete player
      _player.erase(player_iterator);

      auto cryptomons_iterator = _cryptomons.find(c_index); //find cryptomon associated with player
      _cryptomons.erase(cryptomons_iterator);

      if(marketplace_iterator != _market.end()){ //if there is an entry in market, delete it
        _market.erase(marketplace_iterator);
      }
    }

    [[eosio::action]]
    void deletemon(eosio::name acc){
        require_auth(acc);
        auto iterator = _player.find(acc.value);
        uint64_t c_index;
        if(iterator != _player.end()){
          if(_player.get(iterator).has_cryptomon){

            _player.modify(iterator, acc, [&](auto &row){
              row.has_cryptomon = false;
              c_index = row.cryptomon_index;
            });

            auto cryptomons_iterator = _cryptomons.find(c_index);
            _cryptomons.erase(cryptomons_iterator);
          }
        }
      }

    [[eosio::action]]
    void listmon(eosio::name acc, uint64_t price){
      require_auth(acc);
      auto player_iterator = _player.find(acc.value);
      auto market_iterator = _market.find(acc.value);
      if(player_iterator == _player.end()){
        eosio::print("No record exists of this player!")
      }
      else{
        if(market_iterator == _market.end()){
          //no entry in marketplace, create entry
          auto c_index = _player.get(player_iterator).cryptomon_index;
          _market.emplace(acc, [&](auto &row){
            row.key = acc;
            row.cryptomon_index = c_index;
            row.askingPrice = price;
          });
        }
        else{
          _market.modify(market_iterator, acc, [&](auto &row){
            row.askingPrice = price;
          });
        }
      }
    }

    [[eosio::action]]
    void delistmon(eosio::name acc){
      require_auth(acc);
      auto iterator = _market.find(acc.value);
      check(iterator != _market.end(), "Cryptomon listing does not exist!");
      _market.erase(iterator);
    }

    [[eosio::action]]
    void purchasemon(eosio::name buyer, eosio::name seller){
        require_auth(buyer);
        if(buyer == seller){
          eosio::print("NOT LEGAL");
        }
        buyer_iterator = _player.find(buyer.value);
        seller_iterator = _player.find(seller.value);
        market_iterator = _market.find(seller.value);
        check(market_iterator != _market.end(), "No record exists from seller!");
        check(buyer_iterator != _player.end(), "No record exists for buyer");
        check(seller_iterator != _player.end(), "No record exists for seller!");

        auto m_entry = _market.get(market_iterator);
        auto buyer_funds = _player.get(buyer_iterator);
        uint64_t cost = m_entry.askingPrice;

        check(buyer_funds.funds.amount() > cost, "Insufficient funds");
        auto seller_entry = _player.get(seller_iterator);
        uint64_t c_index = seller_entry.cryptomon_index;

        if(buyer_funds.funds.amount() > cost && buyer_iterator != player.end()){
          _player.modify(buyer_iterator, buyer, [&](auto &row){
            row.cryptomon_index = c_index;
            row.has_cryptomon = true;
          });

          _player.modify(seller_iterator, seller, [&](auto &row){
            row.has_cryptomon = false;
            row.cryptomon_index = 0; //the index of seller must be changed to indicate no cryptomon
          });
        }

        action{
          permission_level{seller, "active_n"},
          get_self(),
          "delistmon"_n,
          std::make_tuple(seller)
        }.send();

        action{
          permission_level{buyer, "active"_n}
          "eosio.token"_n,
          "transfer"_n,
          std::make_tuple(buyer, seller, buyer_iterator->funds, std::string("Funds transferred!"))
        }.send();
      }

    [[eosio::action]]
    void trademon(eosio::name account_one, eosio::name account_two){
        auto iterator_account_one = _player.find(account_one.value);
        auto iterator_account_two = _player.find(account_two.value);
        check(iterator_account_one != _player.end(), "No record exists for account one!");
        check(iterator_account_two != player.end(), "No record exists for account two!");

        if(iterator_account_one != _player.end() && iterator_account_two != player.end()){
          auto r1 = _player.get(account_one);
          c_index_one = r1.cryptomon_index;
          auto r2 = _player.get(account_two);
          c_index_two = r2.cryptomon_index;

          _player.modify(iterator_account_one, account_one, [&](auto &row){
            row.cryptomon_index = c_index_two;
          });

          _player.modify(iterator_account_two, account_two, [&] (auto &row){
            row.cryptomon_index = c_index_one;
          });
    }
  }

    [[eosio::action]]
    bool accepttrade(eosio::name account_two){
      return true;
    }

    [[eosio::action]]
    void setname(eosio::name acc, std::string s){
      require_auth(acc);
      auto iterator = _player.find(acc.value);
      auto c_index = _player.get(iterator).cryptomon_index;

      if(iterator != _player.end() && _player.get(iterator).has_cryptomon){
        auto cryptomons_iterator = _cryptomons.find(c_index);
        _cryptomons.modify(cryptomons_iterator, acc, [&](auto &row){
           row.monster.name = s;
        });
      }
    }

    [[eosio::action]]
    void interact(eosio::name acc){
      require_auth(acc);
      auto iterator = _player.find(acc.value);
      if(iterator != _player.end() && _player.get(iterator).has_cryptomon){
        auto c_index = _player.get(iterator).cryptomon_index;
        auto cryptomons_iterator = _cryptomons.find(c_index);

        _cryptomons.modify(cryptomons_iterator, acc, [&](auto &row){
          row.monster.HP += 5;
          row.monster.emotionalState = 1;
        });
      }
      else{
        eosio::print("Player record does not exist!");
      }
    }

    [[eosio::action]]
    void monversion(){
      eosio::print("VERSION 1.0");
    }

    //get current time
    uint32_t now() {
      return current_time_point().sec_since_epoch();
    }
  }

private:
  currency_symbol = "EOS"_n;
//look into vectors of defined user-type, for now will limit accounts to having one list_cryptomo
};
