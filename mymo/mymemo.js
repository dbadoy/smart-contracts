const eos_config = {
    expireInSeconds: 60,
    broadcast: true,
    debug: false,
    sign: true,
    httpEndpoint: 'https://jungle3.cryptolions.io:443',
    keyProvider: [
    ], 
    chainId: '2a02a0053e5a8cf73a56ba0fda11e4d92e0238a4a2aa74fccf46d5a910746840',
}; 

const router = require("express").Router();
const { encrypt, decrypt } = require("eos-encrypt");
const sha256 = require('sha256');
const crypto = require('crypto');
const bcrypt = require("bcryptjs");
const jwt = require("jsonwebtoken")
const eosjs_acc = require("eosjs-ecc");
const Eos = require('eosjs');
const { json } = require("body-parser");
const { tmpdir } = require("os");
const eos = Eos(eos_config);
const ecc = eosjs_acc;

router.post("/inputmemo", async(req,resp) => {

    const { yourAccount,publickey,privatekey,yourmemo  } = req.body;

    console.log(yourAccount, yourmemo);

    if(!(ecc.isValidPrivate(privatekey))) { return resp.status(400).json("err : unvalid private key")}


    eos_config.keyProvider.push(privatekey);
    const eos = Eos(eos_config);

    const enc_memo = encrypt(privatekey, publickey, yourmemo);

   
        hash_memo = sha256(yourmemo);

        eos.transaction({
            actions: [{
                account: "mymocontract",
                name: "pushmemo",
                data: {
                    user : yourAccount,
                    memo: enc_memo,
                    memohash : hash_memo
                },
                authorization: [{
                    actor: yourAccount,
                    permission: "active"
                }]
            }]
        })
        .then(res => { return resp.status(200).json(res)})
        .catch( err => { return resp.status(400).json("err : " + err)})
})


router.post('/getmemo', async(req, resp) => {

    const { publickey, privatekey, yourAccount } = req.body;

    if(!(ecc.isValidPrivate(privatekey))) { return res.status(400).json("err : unvalid private key")}


    let temp = []; let count = 0;
    eos.getTableRows({
        code:"mymocontract",
        scope: "mymocontract",
        table: 'mymemos',
        json: true,
    })
    .then(res => {
        let temp = {};
        for(let i = 0; i < res.rows.length; i ++){
            if(res.rows[i].user == yourAccount){
                temp = res.rows[i].memos;
                //return resp.status(200).json(res.rows[i].memos)
            }    
        }
        let Arr = new Array();
        let item = new Object();
        for(let j = 0; j < temp.length; j ++){
            item = {
                "memo" : decrypt(privatekey, publickey, temp[j].memo),
                "memohash" : temp[j].memohash
            }
            Arr.push(item);
            
        }
        let flag = 0;
        for(let k = 0; k < Arr.length; k ++){
            if(!(sha256(Arr[k].memo) == Arr[k].memohash)){
                console.log(sha256(Arr[k].memo) , "      ", Arr[k].memohash);
                console.log(Arr[k].memo)
                flag = flag -1;
            }
        }
        if( flag == 0 ){
            return resp.status(200).json(Arr);
        }else{
            console.log(flag);
            return resp.status(400).json("Integrity issues")
        }
       
    })
})


module.exports = router;
