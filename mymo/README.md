Mymo
=======
#### Put a note on the EOS blockchain.

#### The memo is encrypted with the user's public key.



Jungle3
=====
Check here : mymocontract

https://jungle3.bloks.io/account/mymocontract

API - pushmemo
=========
### Request
> yourAccount, publickey, privatekey, yourmemo 

### Response
> Success or Error

#### In blockchain ( example )
```
azdkeleizk12	
[
 { "memo": "TO DECRYPT: eos-encrypt\n.-917881134...24512325.3617415603wFjIyT2EhmHvVs7V3FTo1RNZ3fbMd0fO+HTSs64cHsE=", 
   "memohash": "00b31d5cfbb961474d63581fe0355bd7724080aaa12109b9acbf779a1dfb10e9" },
 { "memo": "TO DECRYPT: eos-encrypt\n..331497171...24512326.3924552434r7XGwwQrHmb40X+hTl9a/Q==", 
   "memohash": "ffdb2dfff745c75fe328130f708cc452986ea2e8e7da71f850904c46472e912d" } 
]
```

API - getmemo
=========
## Request
> yourAccount, publickey, privatekey

## Response
![2](https://user-images.githubusercontent.com/72970043/100400927-55a6f000-309b-11eb-9d8a-e9747c449fba.PNG)




Smart contract
=============
> Table
> Action




