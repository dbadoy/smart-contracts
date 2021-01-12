#include<eosio/eosio.hpp>
#include <eosio/crypto.hpp>
#include <eosio/singleton.hpp>

using namespace eosio;

class [[eosio::contract("adoysign")]] adoysign : public eosio::contract {
	
   public:

    adoysign(name receiver, name code, datastream<const char *> ds) : eosio::contract(receiver, code, ds) {}

    [[eosio::action]]
    void sethash(name user, checksum256 signhash) {
		//require_auth(deployAccount);
		require_auth(user);
		sign_index tmpsign( get_self(), get_first_receiver().value);
		
		
		tmpsign.emplace( user, [&](auto& row) {
			row.signnum = getnum(sign_num);
			row.signhashvalue = signhash;
			row.use = false;

			print("Here your signCode number : ", row.signnum);
		});
	}

	[[eosio::action]]
	void setdoc( name user, checksum256 originhash ) {
		//require_auth(deployAccount);
		require_auth(user);
		doc_index tmpdoc( get_self(), get_first_receiver().value);

		tmpdoc.emplace( user, [&](auto& row) {
			row.docnum = getnum(doc_num);
			row.originhash = originhash;

			print("Here your document number : ", row.docnum);
		});	
	}

	[[eosio::action]]
	void signdoc( name user, std::string sign_code, uint64_t sign_num, uint64_t doc_num, checksum256 my_doc_hash){
		//require_auth(deployAccount);
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

    private:
		struct signcodeinfo {
			name signer;
			uint64_t signnum;
			std::string signcode;	
		};

		struct [[eosio::table]] signhash {
			uint64_t signnum;
			checksum256 signhashvalue;
			bool use; // use aodysign -> doc -> sign

			uint64_t primary_key() const { return signnum; }
		};
		using sign_index = eosio::multi_index<"signs"_n, signhash>;



		struct [[eosio::table]] dochash {
			uint64_t docnum;
			checksum256 originhash;
			std::vector<signcodeinfo> signs; 

			uint64_t primary_key() const { return docnum; }
		};
		using doc_index = eosio::multi_index<"doc"_n, dochash>;

		struct [[eosio::table]] global {
			global() {}
			uint64_t signnum = 100000000000000;
			uint64_t docnum = 200000000000000;

			EOSLIB_SERIALIZE( global, ( signnum ) ( docnum ) )
		};
		typedef eosio::singleton< "global"_n, global > conf;


};
