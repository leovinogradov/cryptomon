const { Api, JsonRpc } = require('eosjs');
const { JsSignatureProvider } = require('eosjs/dist/eosjs-jssig');  // development only
const fetch = require('node-fetch'); //node only
const { TextDecoder, TextEncoder } = require('util'); //node only
//const signatureProvider = new JsSignatureProvider(["5KKxRW5DT54XjzApDrG9XsHqWNsJqVSEN43axeuX6AQ3o96PuEp"]);
//const signatureProvider = new JsSignatureProvider(["5JP6Vr9KB7bwJGG2QfjackhaWCw5eH1J1QnVdJqBAU23RcJVAbf"]);
const signatureProvider = new JsSignatureProvider(["5JmgLuvjNMBPJhwDKHpxVoJGFpvKufR7kPbY882U9y2uAuH1pJy"]);
//const rpc = new JsonRpc('http://127.0.0.1:8888', { fetch }); //required to read blockchain state
const rpc = new JsonRpc('https://api.testnet.eos.io', { fetch }); //required to read blockchain state
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
           account: 'mppvvumgroiw',
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
       account: 'mppvvumgroiw',
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
       account: 'mppvvumgroiw',
       name: 'listmon',
       authorization: [{
         actor: argv.account,
         permission: 'active',
       }],
       data: {
         acc: argv.account,
         price: argv.asset,
         cryptomon_index: argv.index
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
       account: 'mppvvumgroiw',
       name: 'purchasemon',
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
       account: 'mppvvumgroiw',
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
       account: 'mppvvumgroiw',
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
       account: 'mppvvumgroiw',
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
       account: 'mppvvumgroiw',
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
       account: 'mppvvumgroiw',
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
       account: 'mppvvumgroiw',
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
       account: 'mppvvumgroiw',
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
        type: 'string',
        demandOption: true,
    }
  },
  async function (argv) {
    await api.transact({
     actions: [{
       account: 'mppvvumgroiw',
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
       account: 'mppvvumgroiw',
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
       account: 'mppvvumgroiw',
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
       account: 'mppvvumgroiw',
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
  .command('transfer', 'Transfer tokens from account to smart-contract account', {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      },
      quantity: {
        describe: 'the quantity to transfer in the format X.XXXX TNT',
        alias: 'q',
        type: 'string',
        demandOption: true,
      },
      memo: {
        describe: "memo or message with transfer",
        alias: 'm',
        type: 'string',
        demandOption: false,
      }
  },
  async function (argv) {
    await api.transact({
     actions: [{
       account: 'eosio.token',
       name: 'transfer',
       authorization: [{
         actor: argv.account,
         permission: 'active',
       }],
       data: {
         from: argv.account,
         to: 'mppvvumgroiw',
         quantity: argv.quantity,
         memo: argv.memo
       },
     }]
    }, {
     blocksBehind: 3,
     expireSeconds: 30,
    });
  })
  .command('itembuy', 'Purchase item/food from the marketeplace with account', {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      },
      select: {
        describe: 'the select index of a food item',
        alias: 's',
        type: 'number',
        demandOption: true,
    }
  },
  async function (argv) {
    await api.transact({
     actions: [{
       account: 'mppvvumgroiw',
       name: 'itembuy',
       authorization: [{
         actor: argv.account,
         permission: 'active',
       }],
       data: {
         account: argv.account,
         select: argv.select,
       },
     }]
    }, {
     blocksBehind: 3,
     expireSeconds: 30,
    });
  })
  .command('itemapply', 'Apply a food item from inventory on a Cryptomon', {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      },
      item: {
        describe: 'the select index of a food item',
        alias: 'itm',
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
       account: 'mppvvumgroiw',
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
  .command('itemdelete', 'Delete item in inventory by specifying item type', {
      account: {
        describe: 'the account associated with player',
        alias: 'a',
        type: 'string',
        demandOption: true,
      },
      item: {
        describe: 'the item type',
        alias: 'itm',
        type: 'number',
        demandOption: true,
    }
  },
  async function (argv) {
    await api.transact({
     actions: [{
       account: 'mppvvumgroiw',
       name: 'itemdelete',
       authorization: [{
         actor: argv.account,
         permission: 'active',
       }],
       data: {
         acc: argv.account,
         item: argv.item
       },
     }]
    }, {
     blocksBehind: 3,
     expireSeconds: 30,
    });
  })
  .command('itemacquire', 'Acquire random item every 24 hours (WIP)', {
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
       account: 'mppvvumgroiw',
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
  .help()
  .alias('help', 'h')
  .argv;

//HOW TO PUSH TRANSACTION
/*
(async () => {
  await api.transact({
   actions: [{
     account: 'mppvvumgroiw',
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
