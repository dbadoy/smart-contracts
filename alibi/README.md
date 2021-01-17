
This is the code that modified SimpleAssets.


Additional features
===
### Document's hash table
In ALIBI, employment contracts and retirement documents are stored on the blockchain. This is a table for this.
In addition, it also includes the user's signature value for the document.

> certhash
```
TABLE certhashd {
			uint64_t		certifinum;
			uint16_t		status;
			checksum256		certifihash;
			checksum256		retirementhash;
			std::vector<string> usersign;
			std::vector<string> targetsign;	

			auto primary_key()const{
				return certifinum;
			}
		};
    // 'status' indicates the document signing status
    // If the 'status' == 4,  This is the finished document.
    // The signature value is stored in vector<string>
```




### NFT's hash table
ALIBI issues career certificates using NFT. This is a table that stores the hash value of the issued NFT.

> nfthash
```
	TABLE nfthash {
			uint64_t		nft_id;
			checksum256		nft_hash;

			auto primary_key()const{
				return nft_id;
			}
		};
```

### Action
There is two additional action.
1. setcerthash - literaly, add in blockchain the document's hash value
2. setnfthash - same ...

> setcerthash
```
ACTION SimpleAssets::setcerthashd( name user, uint64_t certifinum, checksum256 certifihash, checksum256 retirementhash, string sign){
	//new certification input == certifinum >> 0 
	require_auth(user);
	string t = "NULL";
	const checksum256 hash_null = sha256(t.data(), t.size());

	const auto itr = certhashd.find(certifinum);
	if(itr == certhashd.end()){
			if(retirementhash == hash_null){
				certhashd.emplace( _self, [&]( auto& r) {
					r.certifinum = certifinum;	
					r.status = 1;
					r.certifihash = certifihash;
					r.retirementhash = hash_null;	
					r.usersign.push_back(sign);	
					r.targetsign.push_back("init");
					print(r.usersign[0]);
				});
				
			} else { check(false, "No exist certification. You cannot enter retire day hash value.");}
	}
	else{
		const auto itr = certhashd.find(certifinum);
			if(retirementhash == hash_null){
				certhashd.modify(itr, _self, [&]( auto& r) {
					if(r.status != 1) { check(false, "already exist certification"); }
					r.targetsign.pop_back();
					r.targetsign.push_back(sign);
					r.status = 2;
				});

			}
			else{
				certhashd.modify(itr, _self, [&]( auto& r) {
					if(r.status == 1) { check(false, "Both signatures have not been entered."); }
					if(r.status == 4) { check(false, "Completed certification"); }
					if(r.status == 3){
						r.targetsign.push_back(sign);
						r.status = 4;
					}	
					
					if(r.status == 2){
						r.retirementhash = retirementhash;
						r.usersign.push_back(sign);
						r.status = 3;
					}
		
				});
			}
			
	}
}

```

> setnfthash
```
ACTION SimpleAssets::setnfthash( name owner ,uint64_t nft_id, checksum256 nft_hash){
	require_auth(owner);

	sassets assets_f( _self, owner.value );
	const auto itr = assets_f.find(nft_id);
			if( itr == assets_f.end()){	
				string tmp = string("Not Eexist NFT!");
				check(false,  tmp);
			}
			else{
			
				// nfthasht == 'nfthas' table's multi index
				auto itr = nfthasht.find(nft_id);
				if( itr == nfthasht.end() ) {
					nfthasht.emplace( _self, [&]( auto& r ) {
						r.nft_id = nft_id;
						r.nft_hash = nft_hash;
					});
				}
				else{
					check(false, "already exist...");
				}
			}
}

```








Account
=== 
Jungle3 : alibicontract

Link : https://jungle3.bloks.io/account/alibicontrac


Reference
===
SimpleAssets

Link : https://github.com/CryptoLions/SimpleAssets
