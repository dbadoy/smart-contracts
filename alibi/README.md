
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
ALIBI issues career certificates using NFT. When issued, it is a table that stores the hash value of the NFT.

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






Account
=== 
Jungle3 : alibicontract

Link : https://jungle3.bloks.io/account/alibicontrac


Reference
===
SimpleAssets

Link : https://github.com/CryptoLions/SimpleAssets
