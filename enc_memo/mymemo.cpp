#include<eosio/eosio.hpp>
#include <eosio/crypto.hpp>

using namespace eosio;

class [[eosio::contract("mymemo")]] mymemo : public eosio::contract {
	
   public:

    mymemo(name receiver, name code, datastream<const char *> ds) : eosio::contract(receiver, code, ds) {}

    [[eosio::action]]
    void pushmemo(name user, std::string memo, checksum256 memohash) {
	require_auth(user);
	memos_index tmpdoc( get_self(), get_first_receiver().value);
	//std::string temp_hash = "tmp_certification_data";	 
	auto it = tmpdoc.find(user.value);
	if( it == tmpdoc.end()){
	  	tmpdoc.emplace( user, [&](auto& row) {
		   	row.user = user;
			docBox tmp;
			tmp.memo = memo;
			tmp.memohash = memohash;

			row.memos.push_back(tmp);
		    });
    }
	else{
		tmpdoc.modify(it, user, [&](auto& row){
			docBox tmp;
			tmp.memo = memo;
			tmp.memohash = memohash;

			row.memos.push_back(tmp);
			});
		}
	}	
    private:
		struct docBox{
			std::string memo;
			checksum256 memohash;

		};

		struct [[eosio::table]] memo {
			name user;
			std::vector<docBox> memos;
			
			uint64_t primary_key() const { return user.value; }
		};
		using memos_index = eosio::multi_index<"mymemos"_n, memo>;


};
