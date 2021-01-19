Adoysign
========
Adoysign is an electronic signature platform using blockchain (EOS).

Example of use
kor. https://docs.google.com/presentation/d/1ibGdVajbw3LHLZprPly4GD8wpUS5Sc2-yeCGpwIklNs/edit#slide=id.p1
eng. https://docs.google.com/presentation/d/147edyvw5h-shBYbKVD6iBOT4OBy931kwdm2Vc11kKxE/edit#slide=id.p1

Feature
-----
> 1. Decentralization of documents to be signed - Documents are stored in IPFS.

> 2. Multiple people can sign the document without any special configuration. - Share the document and just wait for signature.

> 3. All signing steps are stored on the blockchain and managed transparently. -  Only the hash value is stored, so others cannot see the actual data.

<br>

Smart contract 
=================
Adoysign's smart contract.
<br>

Account
----- 
Jungle3 : adoycontract

Link : https://jungle3.bloks.io/account/adoycontract
<br>

## Tables
The table is structured **signhash**, **dochash**, **Global**.<br>
**signhash** - This is the storage location of signCode's hash value.<br>
**dochash** - This is the storage location of document's hash value and signature data.<br>
**Global** - Automatic increment using sigleton.<br><br>

> signhash
```
struct [[eosio::table]] signhash {
		uint64_t signnum;
		checksum256 signhashvalue;
		bool use; // use aodysign -> doc -> sign

		uint64_t primary_key() const { return signnum; }
	};
	using sign_index = eosio::multi_index<"signs"_n, signhash>;
```
> dochash
```
struct [[eosio::table]] dochash {
		uint64_t docnum;
		checksum256 originhash; // document's hash value
		std::vector<signcodeinfo> signs;  // The storage vector of signature data for document

		uint64_t primary_key() const { return docnum; }
	};
	using doc_index = eosio::multi_index<"doc"_n, dochash>;


// vector <signer> structured like this,
struct signer {
  		name signer
  		uint64_t signnum
  		string signcode
	};

```
  
> Global  
```
struct [[eosio::table]] global {
		global() {}
		uint64_t signnum = 100000000000000;
		uint64_t docnum = 200000000000000;

		EOSLIB_SERIALIZE( global, ( signnum ) ( docnum ) )
	};
	typedef eosio::singleton< "global"_n, global > conf;
```
 
## Actions
> sethash
```
void sethash(name user, checksum256 signhash) {
		require_auth(user);
		sign_index tmpsign( get_self(), get_first_receiver().value);
		
		tmpsign.emplace( user, [&](auto& row) {
			row.signnum = getnum(sign_num);
			row.signhashvalue = signhash;
			row.use = false;
		});
	}
```

> setdoc
```
	void setdoc( name user, checksum256 originhash ) {
		require_auth(user);
		doc_index tmpdoc( get_self(), get_first_receiver().value);

		tmpdoc.emplace( user, [&](auto& row) {
			row.docnum = getnum(doc_num);
			row.originhash = originhash;
		});	
	}
```

> signdoc
```
void signdoc( name user, std::string sign_code, uint64_t sign_num, uint64_t doc_num, checksum256 my_doc_hash){
		require_auth(user);

		doc_index tmpdoc( get_self(), get_first_receiver().value);
		sign_index tmpsign( get_self(), get_first_receiver().value);

		auto sign_itr = tmpsign.find(sign_num);
		if( sign_itr == tmpsign.end()){ check(false, "No exist sign number !"); }

		if(sign_itr->use) { check(false, "already used sign code !"); }

		checksum256 entered_sign_code_hash = sha256(sign_code.data(), sign_code.size());
		if(sign_itr->signhashvalue != entered_sign_code_hash){ check(false, "Not matched sign code !"); };
		

		auto doc_itr = tmpdoc.find(doc_num);
		if( doc_itr == tmpdoc.end()){ check(false, "No exist document number !"); }
		tmpdoc.modify(doc_itr, user, [&](auto& row) {
			if(row.originhash != my_doc_hash){ check(false, "Different hash values. check your document"); }

			signcodeinfo temp;
			temp.signer = user;
			temp.signnum = sign_num;
			temp.signcode = sign_code;
 
			row.signs.push_back(temp);
		});
		tmpsign.modify(sign_itr, user, [&](auto& row) {
			row.use = true;
		});
	}
```

## Function
> getnum
```
enum num_type{ sign_num = 0, doc_num = 1 };

uint64_t getnum(num_type type){
		conf config(get_self(), get_self().value);

		global cstate = config.exists() ? config.get() : global{};
		uint64_t result = 0;
		
		switch( type ) {
			case sign_num:
				result = ++cstate.signnum;	
				break;
			case doc_num:
				result = ++cstate.docnum;
				break;
			default:
				check(false, "Wrong type. Value must be sign_num, cert_num");
		}

		config.set( cstate, _self);
		return result;
	}
```


