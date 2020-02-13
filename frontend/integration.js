const { Api, JsonRpc } = require('eosjs');
const { JsSignatureProvider } = require('eosjs/dist/eosjs-jssig');  // development only
const fetch = require('node-fetch'); //node only
const { TextDecoder, TextEncoder } = require('util'); //node only
//const privateKeys = [privateKey1];
//const privateKeys = ['EOS5LabEzJh4sStF5TyLGg718vs52dMyHhU978UUFvWKG9V2PuWPf'];
const signatureProvider = new JsSignatureProvider(["5KKxRW5DT54XjzApDrG9XsHqWNsJqVSEN43axeuX6AQ3o96PuEp"]);
const rpc = new JsonRpc('http://127.0.0.1:8888', { fetch }); //required to read blockchain state
const api = new Api({ rpc, signatureProvider, textDecoder: new TextDecoder(), textEncoder: new TextEncoder() }); //required to submit transactions

require('yargs')
  .command('createmon', "Create a cryptomon for player", {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      }
    },
      async function (argv) {
        await api.transact({
         actions: [{
           account: 'cryptomon',
           name: 'createmon',
           authorization: [{
             actor: argv.account,
             permission: 'active',
           }],
           data: {
             acc: argv.account,
           },
         }]
        }, {
         blocksBehind: 3,
         expireSeconds: 30,
        });
  })
  .command('deletemon', 'Delete specified cryptomon of player', {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      },
      index: {
        describe: 'the index of the cryptomon',
        alias: 'i',
        type: 'number',
        demandOption: true,
    }
  },
  async function (argv) {
    await api.transact({
     actions: [{
       account: 'cryptomon',
       name: 'deletemon',
       authorization: [{
         actor: argv.account,
         permission: 'active',
       }],
       data: {
         acc: argv.account,
         cryptomon_index: argv.index
       },
     }]
    }, {
     blocksBehind: 3,
     expireSeconds: 30,
    });
  })
  .command('listmon', 'List players cryptomon for sale', {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      },
      asset: {
        describe: 'the price of the cryptomon in the format [asset type]',
        alias: 'p',
        type: 'string',
        demandOption: true,
      },
      delay: {
        describe: 'duration of sale',
        alias: 'd',
        type: 'number',
        demandOption: true,
      },
      index: {
        describe: 'the index of the cryptomon',
        alias: 'i',
        type: 'number',
        demandOption: true,
    }
  },
  async function (argv) {
    await api.transact({
     actions: [{
       account: 'cryptomon',
       name: 'listmon',
       authorization: [{
         actor: argv.account,
         permission: 'active',
       }],
       data: {
         acc: argv.account,
         price: argv.asset,
         delay: argv.delay,
         cryptomon_index: argv.index
       },
     }]
    }, {
     blocksBehind: 3,
     expireSeconds: 30,
    });
  })
  .command('delistmon', 'Remove cryptomon that is listed for sale', {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      },
      index: {
        describe: 'the index of the cryptomon',
        alias: 'i',
        type: 'number',
        demandOption: true,
    }
  },
  async function (argv) {
    await api.transact({
     actions: [{
       account: 'cryptomon',
       name: 'delistmon',
       authorization: [{
         actor: argv.account,
         permission: 'active',
       }],
       data: {
         acc: argv.account,
         c_index: argv.index
       },
     }]
    }, {
     blocksBehind: 3,
     expireSeconds: 30,
    });
  })
  .command('purchasemon', 'Purchase cryptomon that is for sale', {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      },
      index: {
        describe: 'the index of the cryptomon',
        alias: 'i',
        type: 'number',
        demandOption: true,
    }
  },
  async function (argv) {
    await api.transact({
     actions: [{
       account: 'cryptomon',
       name: 'purchasemon',
       authorization: [{
         actor: argv.account,
         permission: 'active',
       }],
       data: {
         acc: argv.account,
         cryptomon_index: argv.index
       },
     }]
    }, {
     blocksBehind: 3,
     expireSeconds: 30,
    });
  })
  .command('accepttrade', 'Accept a trade initiated by another player', {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      },
      index: {
        describe: 'the index of the cryptomon',
        alias: 'i',
        type: 'number',
        demandOption: true,
    }
  },
  async function (argv) {
    await api.transact({
     actions: [{
       account: 'cryptomon',
       name: 'accepttrade',
       authorization: [{
         actor: argv.account,
         permission: 'active',
       }],
       data: {
         account: argv.account,
         cryptomon_index: argv.index
       },
     }]
    }, {
     blocksBehind: 3,
     expireSeconds: 30,
    });
  })
  .command('setname', 'Set the name of players cryptomon', {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      },
      name: {
        describe: 'the name of the cryptomon',
        alias: 'n',
        type: 'string',
        demandOption: true,
      },
      index: {
        describe: 'the index of the cryptomon',
        alias: 'i',
        type: 'number',
        demandOption: true,
    }
  },
  async function (argv) {
    await api.transact({
     actions: [{
       account: 'cryptomon',
       name: 'setname',
       authorization: [{
         actor: argv.account,
         permission: 'active',
       }],
       data: {
         acc: argv.account,
         s: argv.name,
         cryptomon_index: argv.index
       },
     }]
    }, {
     blocksBehind: 3,
     expireSeconds: 30,
    });
  })
  .command('interact', 'Interacting with cryptomon', {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      },
      index: {
        describe: 'the index of the cryptomon',
        alias: 'i',
        type: 'number',
        demandOption: true,
    }
  },
  async function (argv) {
    await api.transact({
     actions: [{
       account: 'cryptomon',
       name: 'interact',
       authorization: [{
         actor: argv.account,
         permission: 'active',
       }],
       data: {
         acc: argv.account,
         cryptomon_index: argv.index
       },
     }]
    }, {
     blocksBehind: 3,
     expireSeconds: 30,
    });
  })
  .command('itemapply', 'Apply item in inventory of player to cryptomon', {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      },
      item: {
        describe: 'the item to apply',
        alias: 'm',
        type: 'number',
        demandOption: true,
      },
      index: {
        describe: 'the index of the cryptomon',
        alias: 'i',
        type: 'number',
        demandOption: true,
    }
  },
  async function (argv) {
    await api.transact({
     actions: [{
       account: 'cryptomon',
       name: 'itemapply',
       authorization: [{
         actor: argv.account,
         permission: 'active',
       }],
       data: {
         acc: argv.account,
         item: argv.item,
         cryptomon_index: argv.index
       },
     }]
    }, {
     blocksBehind: 3,
     expireSeconds: 30,
    });
  })
  .command('itemacquire', 'Acquire item', {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      }
  },
  async function (argv) {
    await api.transact({
     actions: [{
       account: 'cryptomon',
       name: 'itemacquire',
       authorization: [{
         actor: argv.account,
         permission: 'active',
       }],
       data: {
         acc: argv.account
       },
     }]
    }, {
     blocksBehind: 3,
     expireSeconds: 30,
    });
  })
  .command('itemdelete', 'Delete specified item from players inventory', {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      },
      index: {
        describe: 'the index of the item',
        alias: 'i',
        type: 'number',
        demandOption: true,
    }
  },
  async function (argv) {
    await api.transact({
     actions: [{
       account: 'cryptomon',
       name: 'itemdelete',
       authorization: [{
         actor: argv.account,
         permission: 'active',
       }],
       data: {
         acc: argv.account,
         item: argv.index
       },
     }]
    }, {
     blocksBehind: 3,
     expireSeconds: 30,
    });
  })
  .command('withdraw', 'Withdraw in-game funds to account', {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      },
      amount: {
        describe: 'the asset amount to withdraw',
        alias: 'a',
        type: 'string',
        demandOption: true,
    }
  },
  async function (argv) {
    await api.transact({
     actions: [{
       account: 'cryptomon',
       name: 'withdraw',
       authorization: [{
         actor: argv.account,
         permission: 'active',
       }],
       data: {
         acc: argv.account,
         amount: argv.amount
       },
     }]
    }, {
     blocksBehind: 3,
     expireSeconds: 30,
    });
  })
  .command('canceltrade', 'Cancel trade initiated by other player', {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      },
      index: {
        describe: 'the index of the cryptomon in trade consideration',
        alias: 'i',
        type: 'number',
        demandOption: true,
    }
  },
  async function (argv) {
    await api.transact({
     actions: [{
       account: 'cryptomon',
       name: 'canceltrade',
       authorization: [{
         actor: argv.account,
         permission: 'active',
       }],
       data: {
         account: argv.account,
         cryptomon_index: argv.index
       },
     }]
    }, {
     blocksBehind: 3,
     expireSeconds: 30,
    });
  })
  .command('inittrade', 'Initiate trade with other player', {
      account1: {
        describe: 'the account of initiator',
        alias: 'a1',
        type: 'string',
        demandOption: true,
      },
      account2: {
        describe: 'the account this trade is requested to',
        alias: 'a2',
        type: 'string',
        demandOption: true,
      },
      price: {
        describe: 'amount offered for cryptomon',
        alias: 'p',
        type: 'string',
        demandOption: true,
      },
      swap: {
        describe: 'false if no cryptomon is offered in trade, true otherwise',
        alias: 's',
        type: 'boolean',
        demandOption: true,
      },
      duration: {
        describe: 'duration of trade event',
        alias: 'd',
        type: 'number',
        demandOption: true
      },
      c1: {
        describe: 'cryptomon offered by initiator, if any',
        type: 'number',
        demandOption: true,
      },
      c2: {
        describe: 'cryptomon in trade consideration',
        type: 'number',
        demandOption: true,
      }
  },
  async function (argv) {
    await api.transact({
     actions: [{
       account: 'cryptomon',
       name: 'inittrade',
       authorization: [{
         actor: argv.account1,
         permission: 'active',
       }],
       data: {
         account_one: argv.account1,
         account_two: argv.account2,
         price: argv.price,
         swap: argv.swap,
         duration: argv.duration,
         c1: argv.c1,
         c2: argv.c2
       },
     }]
    }, {
     blocksBehind: 3,
     expireSeconds: 30,
    });
  })
  .command('upsertplayer', 'Create player for account', {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      },
      s: {
        describe: 'the name of the player',
        alias: 's',
        type: 'string',
        demandOption: true,
    }
  },
  async function (argv) {
    await api.transact({
     actions: [{
       account: 'cryptomon',
       name: 'upsertplayer',
       authorization: [{
         actor: argv.account,
         permission: 'active',
       }],
       data: {
         acc: argv.account,
         s: argv.s
       },
     }]
    }, {
     blocksBehind: 3,
     expireSeconds: 30,
    });
  })
  .command('deleteplayer', 'Delete specified cryptomon of player', {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      }
  },
  async function (argv) {
    await api.transact({
     actions: [{
       account: 'cryptomon',
       name: 'deleteplayer',
       authorization: [{
         actor: argv.account,
         permission: 'active',
       }],
       data: {
         acc: argv.account
       },
     }]
    }, {
     blocksBehind: 3,
     expireSeconds: 30,
    });
  })
  .help()
  .alias('help', 'h')
  .argv;

//HOW TO PUSH TRANSACTION
/*
(async () => {
  await api.transact({
   actions: [{
     account: 'cryptomon',
     name: 'createmon',
     authorization: [{
       actor: 'alice',
       permission: 'active',
     }],
     data: {
       acc: 'alice',
     },
   }]
  }, {
   blocksBehind: 3,
   expireSeconds: 30,
  });
})();
*/
