using eosio::contract;

struct [[eosio::table]] pendingTradesStruct {
  eosio::name account_one;
  eosio::name account_two;
  uint64_t cryptomon_index;
  uint64_t price;
  bool sold;
  uint64_t primary_key() const { return cryptomon_index; }
};

typedef eosio::multi_index<"pendingTrades"_n, pendingTradesStruct> pendingTrades;

[[eosio::action]]
void init_trade(eosio::name acct, eosio::name account_two, eosio::asset::amount price) {
  require_auth(acct);
  pendingTrades.emplace(acct, [&](auto &entry){ // creating new trade
    //  entry.key = entry.key+1;
    entry.account_one = acct;
    entry.account_two = account_two;
    entry.cryptomon_index = cryptomon_index;
    entry.price = price;
    entry.sold = false;
  });
}

void trade(eosio::name acct, uint64_t cryptomon_index) {
  require_auth(acct); // acct in this case is account of buyer

  auto trade_itr = pendingTrades.find(cryptomon_index);
  eosio::check(trade_itr != pendingTrades.end(), "No such trade exists");

  auto trade_instance = pendingTrades.get(cryptomon_index);
  eosio::check((!trade_instance.account_two || trade_instance.account_two == acct), "This trade is not for you");
  eosio::check(!trade_instance.sold, "This trade has already been completed");

  auto buyer_iterator = player_table.find(acct.value);
  auto seller_iterator = player_table.find(trade_instance.account_one.value);
  eosio::check(buyer_iterator != player_table.end(), "No record exists for buyer");
  eosio::check(seller_iterator != player_table.end(), "No record exists for seller!");

  auto buyer = player_table.get(acct.value);
  uint64_t cost = trade_instance.price;

  eosio::check(buyer.funds.amount > cost, "Insufficient funds");
  auto seller = player_table.get(trade_instance.account_one.value);
  uint64_t c_index = seller.cryptomon_index;
  eosio::check((c_index == trade_instance.cryptomon_index), "Invalid cryptomon index"); // should never happen but good to check anyway

  // Transfer funds
  // TODO: implement transfer funds

  // Swap cryptomons
  if(buyer.funds.amount > cost && buyer_iterator != player_table.end()){
    player_table.modify(buyer_iterator, buyer.key, [&](auto &row){
      row.cryptomon_index = c_index;
      row.has_cryptomon = true;
    });

    player_table.modify(seller_iterator, seller.key, [&](auto &row){
      row.has_cryptomon = false;
      row.cryptomon_index = buyer.cryptomon_index; // swap seller's and buyer's cryptomon
    });

    pendingTrades.modify(trade_itr, acct [&](auto &entry){
      entry.sold = true;
    });
  }
}
