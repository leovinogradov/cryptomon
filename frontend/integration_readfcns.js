const { Api, JsonRpc, RpcError } = require('eosjs');
const fetch = require('node-fetch');           // node only; not needed in browsers
//const rpc = new JsonRpc('http://localhost:8888', { fetch });
const rpc = new JsonRpc('https://api.testnet.eos.io', { fetch });
const { TextEncoder, TextDecoder } = require('util');                   // node only; native TextEncoder/Decoder

require('yargs')
  .command('getplayer', "Retrieve player info from blockchain", {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      }
    },
      async function (argv) {
        const resp = await rpc.get_table_rows({
          json: true,
          code: 'mppvvumgroiw',
          scope: 'mppvvumgroiw',
          table: 'players',
          lower_bound: argv.account,
          limit: 1,
          reverse: false,
          show_payer: false,
        });
        console.log(JSON.stringify(resp.rows[0]));
  })
  .command('getcryptomon', "Retrieve Cryptomon info from blockchain", {
      index: {
        describe: 'the cryptomon associated with player',
        alias: 'i',
        type: 'number',
        demandOption: true,
      }
    },
      async function (argv) {
        const resp = await rpc.get_table_rows({
          json: true,
          code: 'mppvvumgroiw',
          scope: 'mppvvumgroiw',
          table: 'cryptomons',
          lower_bound: argv.index,
          limit: 1,
          reverse: false,
          show_payer: false,
        });
        console.log(JSON.stringify(resp.rows[0]));
  })
  .command('getallinfo', "Retrieve all info of Player and their Cryptomons from blockchain", {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      }
    },
      async function (argv) {
        const resp = await rpc.get_table_rows({
          json: true,
          code: 'mppvvumgroiw',
          scope: 'mppvvumgroiw',
          table: 'players',
          lower_bound: argv.account,
          limit: 1,
          reverse: false,
          show_payer: false,
        });
        resp.rows[0].cryptomons = new Array();
        var i;
        for(i = 0; i < resp.rows[0].cryptomon_indexes.length; i++){
          let resp_c = await rpc.get_table_rows({
            json: true,
            code: 'mppvvumgroiw',
            scope: 'mppvvumgroiw',
            table: 'cryptomons',
            lower_bound: resp.rows[0].cryptomon_indexes[i],
            limit: 1,
            reverse: false,
            show_payer: false,
          });
          resp.rows[0].cryptomons.push(resp_c.rows[0]);
        }
        console.log(JSON.stringify(resp.rows[0]));
  })
  .command('gettransacts', "Retrieve transact events in transacts table", {
    },
      async function (argv) {
        const resp = await rpc.get_table_rows({
          json: true,
          code: 'mppvvumgroiw',
          scope: 'mppvvumgroiw',
          table: 'transacts',
          lower_bound: "",
          reverse: false,
          show_payer: false,
        });
        console.log(JSON.stringify(resp.rows));
  })
  .command('getofferedtrades', "Retrieve all trades initiated with a Player", {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      }
    },
      async function (argv) {
        const temp = await rpc.get_table_rows({
          json: true,
          code: 'mppvvumgroiw',
          scope: 'mppvvumgroiw',
          table: 'players',
          lower_bound: argv.account,
          reverse: false,
          show_payer: false,
        });
        //resp.rows[0].cryptomons = new Array();
        resp = {};
        resp.trades = new Array();
        var i;
        for(i = 0; i < temp.rows[0].cryptomon_indexes.length; i++){
          let resp_c = await rpc.get_table_rows({
            json: true,
            code: 'mppvvumgroiw',
            scope: 'mppvvumgroiw',
            table: 'transacts',
            lower_bound: temp.rows[0].cryptomon_indexes[i],
            upper_bound: temp.rows[0].cryptomon_indexes[i],
            limit: 1,
            reverse: false,
            show_payer: false,
          });
          if(resp_c.rows.length > 0 && resp_c.rows[0].account_two != ""){
            resp.trades.push(resp_c.rows[0]);
          }
        }
        console.log(JSON.stringify(resp));
  })
  .command('getlistings', "Retrieve 10 listing(s) in the market", {
    },
      async function (argv) {
        const resp = await rpc.get_table_rows({
          json: true,
          code: 'mppvvumgroiw',
          scope: 'mppvvumgroiw',
          table: 'transacts',
          key_type: 'i64',
          index_position: 3,
          lower_bound: 0,
          upper_bound: 0,
          limit: 10,
          reverse: false,
          show_payer: false,
        });

        console.log(JSON.stringify(resp.rows));
  })
  .command('getyourtransacts', "Retrieve a Player's listing(s) and/or trade(s) in the market", {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      }
    },
      async function (argv) {
        const resp = await rpc.get_table_rows({
          json: true,
          code: 'mppvvumgroiw',
          scope: 'mppvvumgroiw',
          table: 'transacts',
          key_type: 'i64',
          index_position: 2,
          lower_bound: argv.account,
          upper_bound: argv.account,
          reverse: false,
          show_payer: false,
        });
        console.log(JSON.stringify(resp.rows));
  })
  .help()
  .alias('help', 'h')
  .argv;
