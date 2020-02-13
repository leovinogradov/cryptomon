const { Api, JsonRpc, RpcError } = require('eosjs');
const fetch = require('node-fetch');           // node only; not needed in browsers
const rpc = new JsonRpc('http://127.0.0.1:8888', { fetch });
const { TextEncoder, TextDecoder } = require('util');                   // node only; native TextEncoder/Decoder

async function getPlayer(){
const resp = await rpc.get_table_rows({
      json: true,                 // Get the response as json
      code: 'cryptomon',           // Contract that we target
      scope: 'cryptomon',           // Account that owns the data
      table: 'players',           // Table name
      lower_bound: 'alice',      // Table primary key value
      limit: 1,                  // Here we limit to 1 to get only the
      reverse: false,            // Optional: Get reversed data
      show_payer: false,         // Optional: Show ram payer
  });
  console.log(resp.rows);
}

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
          code: 'cryptomon',
          scope: 'cryptomon',
          table: 'players',
          lower_bound: argv.account,
          limit: 1,
          reverse: false,
          show_payer: false,
        });
        console.log(resp.rows[0]);
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
          code: 'cryptomon',
          scope: 'cryptomon',
          table: 'cryptomons',
          lower_bound: argv.index,
          limit: 1,
          reverse: false,
          show_payer: false,
        });
        console.log(resp.rows[0]);
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
          code: 'cryptomon',
          scope: 'cryptomon',
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
            code: 'cryptomon',
            scope: 'cryptomon',
            table: 'cryptomons',
            lower_bound: resp.rows[0].cryptomon_indexes[i],
            limit: 1,
            reverse: false,
            show_payer: false,
          });
          resp.rows[0].cryptomons.push(resp_c.rows[0]);
        }
          console.log(resp.rows[0]);

  })
  .help()
  .alias('help', 'h')
  .argv;
