#pragma once
#include<eosio/transaction.hpp>
#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <string>
#include <vector>

using std::string;
using eosio::contract;

class [[eosio::contract("cryptomon")]] cryptomon: public eosio::contract {

  public:
    //using contract::contract;
    /*actions dealt with modifying persistant storage dealing with 2 multi-index tables
    _market and _player
    addPlayer() inserts object in _player assigning randomly generated cryptomon
    deletePlayer() removes object in _player also deleting cryptomon, removes entry in
    _market if found
    deleteCryptomon() deletes data found in cryptomon field, calls to random generation of
    cryptomon function
    listCryptomon() inserts object in _market identifying existing crytpomon for sale
    delistCryptomon() deletes object in _market, cryptomon no longer for sale
    purchaseCryptomon() if asset > askingPrice for cryptomon, cryptomon transfers to buyer
    */
    /*
    actions dealing with management of accounts
    */
    cryptomon(eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds);

    [[eosio::action]]
    void upsertplayer(eosio::name acc, std::string s);

    [[eosio::action]]
    void createmon(eosio::name acc);

    [[eosio::on_notify("eosio.token::transfer")]]
    void playerfund(eosio::name acc, eosio::name to, eosio::asset quantity, std::string memo);

    [[eosio::action]]
    void deleteplayer(eosio::name acc);

    [[eosio::action]]
    void deletemon(eosio::name acc);

    [[eosio::action]]
    void listmon(eosio::name acc, eosio::asset price, uint64_t delay);

    [[eosio::action]]
    void delistmon(uint64_t c_index);

    [[eosio::action]]
    void purchasemon(eosio::name acc, uint64_t c_index);

    [[eosio::action]]
    void trademon(eosio::name account_one, eosio::name account_two);

    [[eosio::action]]
    void accepttrade(eosio::name account, uint64_t cryptomon_index);
    /*
    actions dealt with modifying state of cryptomon
    */
    [[eosio::action]]
    void setname(eosio::name acc, std::string s);

    [[eosio::action]]
    void interact(eosio::name acc);

    [[eosio::action]]
    void itemapply(eosio::name acc, uint8_t item);

    [[eosio::action]]
    void itemacquire(eosio::name acc);

    [[eosio::action]]
    void itemdelete(eosio::name acc, uint8_t item);

    [[eosio::action]]
    void monversion();

    [[eosio::action]]
    void withdraw(eosio::name acc, eosio::asset amount);

    [[eosio::action]]
    void canceltrade(uint64_t cryptomon_index);

    [[eosio::action]]
    void inittrade(eosio::name account_one, eosio::name account_two, eosio::asset price, bool swap, uint64_t duration);

    uint32_t now();

    int findItem(uint8_t item, const std::vector<uint8_t> &v);

/*
    struct mon{
      mon(): HP(0), level(0), age(0), weight(0), emotionalState(0), mon_name(""){}
      mon(unsigned short int HP, unsigned short int level, unsigned int age, unsigned int weight, unsigned short int emotionalState, std::string s): HP(HP), level(level), age(age), weight(weight), emotionalState(emotionalState), mon_name(s)
      {}
      uint8_t HP;
      uint8_t level;
      uint32_t age;
      uint32_t weight;
      uint8_t emotionalState;
      std::string mon_name;
      //EOSIOLIB_SERIALIZE(mon, (HP)(level)(age)(weight)(emotionalState)(mon_name))
    };
*/

    struct [[eosio::table]] player{
      eosio::name key;
      std::vector<uint8_t> inventory;
      std::string playerName;
      eosio::asset funds;
      uint64_t cryptomon_index;
      bool has_cryptomon;
      //uint64_t playerId;
      //cryptomon monster;
      uint64_t primary_key() const {return key.value; }
      //uint64_t by_playerId const();
    };

    struct [[eosio::table]] transact {
      eosio::name account_one;
      eosio::name account_two;
      uint64_t cryptomon_index;
      bool swap;
      eosio::asset price;
      uint64_t primary_key() const { return cryptomon_index; }
    };


/*
    struct [[eosio::table]] marketplace{
      eosio::name key;
      uint64_t cryptomon_index;
      uint64_t askingPrice;
      uint64_t primary_key() const { return key.value; }
      //uint64_t playerId;
      //cryptomon monsterForSale;
      //uint64_t by_playerId const();

    };
*/
    struct [[eosio::table]] marketplace{
      uint64_t key;
      eosio::name seller;
      //uint64_t askingPrice;
      eosio::asset cost;
      uint64_t primary_key() const {return key; }
    };

    struct [[eosio::table]] cryptomons{
      uint64_t key;
      //mon monster;
      uint8_t HP;
      uint8_t level;
      uint8_t happiness;
      uint8_t health;
      uint8_t hunger;
      eosio::time_point start;
      eosio::time_point current;
      std::string mon_name;
      uint64_t primary_key() const {return key; }
    };

/*
    struct [[eosio::table]] inventory{
      uint64_t key;
      uint8_t item;
    };
    FOR BIT MANIPULATION
    int bits = 3; // 3 lower bits
    uint8_t mask = (1 << bits) - 1; // 7

*/
    //typedef eosio::multi_index<"_player"_n, player, eosio::indexed_by<"playerid"_n, eosio::const_mem_fun<player, uint64_t, &player::by_monsterName>>> player_data;
    //typedef eosio::multi_index<"_market"_n, marketplace, eosio::indexed_by<"playerid"_n, eosio::const_mem_fun<market, uint64_t, &market::by_playerId>>> market_data;

    typedef eosio::multi_index<"players"_n, player> p_data;
    //typedef eosio::multi_index<"market"_n, marketplace> m_data;
    typedef eosio::multi_index<"cryptomons"_n, cryptomons> c_data;
    typedef eosio::multi_index<"transacts"_n, transact> t_data;

    c_data mons_table;
    p_data player_table;
    //m_data market_table;
    t_data transact_table;

  private:
    const eosio::symbol currency_symbol = eosio::symbol("EOS", 4);
  };
