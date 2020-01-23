const { Api, JsonRpc } = require('eosjs');
const { JsSignatureProvider } = require('eosjs/dist/eosjs-jssig');  // development only
const fetch = require('node-fetch'); //node only
const { TextDecoder, TextEncoder } = require('util'); //node only
//const privateKeys = [privateKey1];
//const privateKeys = ['EOS5LabEzJh4sStF5TyLGg718vs52dMyHhU978UUFvWKG9V2PuWPf'];
const signatureProvider = new JsSignatureProvider(["5KKxRW5DT54XjzApDrG9XsHqWNsJqVSEN43axeuX6AQ3o96PuEp"]);
const rpc = new JsonRpc('http://127.0.0.1:8888', { fetch }); //required to read blockchain state
const api = new Api({ rpc, signatureProvider, textDecoder: new TextDecoder(), textEncoder: new TextEncoder() }); //required to submit transactions


//HOW TO PUSH TRANSACTION
(async () => {
  await api.transact({
   actions: [{
     account: 'cryptomon',
     name: 'deletemon',
     authorization: [{
       actor: 'alice',
       permission: 'active',
     }],
     data: {
       acc: 'alice',
       cryptomon_index: 5,
     },
   }]
  }, {
   blocksBehind: 3,
   expireSeconds: 30,
  });
})();
