#pragma once
#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <string>

using std::string;
using eosio::contract;

class [eosio::contract("cryptomon")] cryptomon: public eosio::contract {

  public:
    using contract::contract;
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
    cryptomon(eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds): eosio::contract(receiver, code, ds),  _player(receiver, code.value), _market(receiver, code.value)
    {}

    /*
    actions dealing with management of accounts
    */
    [[eosio::action]]
    void upsertplayer(eosio::name acc);

    [[eosio::action]]
    void createmon(eosio::name acc);

    [[eosio::on_notify("eosio.token::transfer")]]
    void addfunds(eosio::name acc, eosio::asset funds);

    [[eosio::action]]
    void deleteplayer(eosio::name acc);

    [[eosio::action]]
    void deletemon(eosio::name acc);

    [[eosio::action]]
    void listmon(eosio::name acc, uint64_t price);

    [[eosio::action]]
    void delistmon(eosio::name acc);

    [[eosio::action]]
    void purchasemon(eosio::name buyer, eosio::name seller);

    [[eosio::action]]
    void trademon(eosio::name account_one, eosio::name account_two);

    [[eosio::action]]
    bool accepttrade(eosio::name account_two);
    /*
    actions dealt with modifying state of cryptomon
    */
    [[eosio::action]]
    void setname(eosio::name acc, std::string s);

    [[eosio::action]]
    void interact(eosio::name acc);

    [[eosio::action]]
    void monversion();

    struct [[eosio::table]] player{
      name key;
      std::string playerName;
      eosio::asset funds;
      uint64_t cryptomon_index;
      bool has_cryptomon;
      //uint64_t playerId;
      //cryptomon monster;
      uint64_t primary_key const() {return key.value; }
      //uint64_t by_playerId const();
    };

    struct [[eosio::table]] marketplace{
      name key;
      uint64_t cryptomon_index;
      uint64_t askingPrice;
      uint64_t primary_key const() { return key.value; }
      //uint64_t playerId;
      //cryptomon monsterForSale;
      //uint64_t by_playerId const();

    };

    struct [[eosio::table]] cryptomons{
      uint64_t index = 0;
      cryptomon monster;
      uint64_t primary_key const() {return index; }
    }

    struct cryptomon{
      cryptomon(unsigned short int HP, unsigned short int level, unsigned int age, unsigned int weight,
      unsigned short int emotionalState, std::string name): HP(HP), level(level), age(age), weight(weight), emotionalState(emotionalState), name(name)
      {}
      unsigned short int HP;
      unsigned short int level;
      unsigned int age;
      unsigned int weight;
      unsigned short int emotionalState;
      std::string name;
    };

/*
    enum class emotion: uint8_t{
      HAPPY,
      CALM,
      SAD,
      ANGRY,
      NEUTRAL,
      FEAR
    };
*/
/*
    struct item{
      unsigned short int HP_boost;
      emotion emotion_boost;
    };
*/
    //typedef eosio::multi_index<"_player"_n, player, eosio::indexed_by<"playerid"_n, eosio::const_mem_fun<player, uint64_t, &player::by_monsterName>>> player_data;
    //typedef eosio::multi_index<"_market"_n, marketplace, eosio::indexed_by<"playerid"_n, eosio::const_mem_fun<market, uint64_t, &market::by_playerId>>> market_data;

    typedef eosio::multi_index<"_player"_n, player> player_data;
    typedef eosio::multi_index<"_market"_n, marketplace> market_data;
    typedef eosio::multi_index<"_cryptomons"_n, cryptomons> cryptomons_data;
    cryptomons_data _cryptomons;
    player_data _player;
    market_data _market;

  private:
    const eosio::symbol currency_symbol;

  };
