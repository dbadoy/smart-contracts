#include<eosio/eosio.hpp>
#include <eosio/crypto.hpp>

using namespace eosio;

class [[eosio::contract("tmphash")]] tmphash : public eosio::contract {
	
   public:

    tmphash(name receiver, name code, datastream<const char *> ds) : eosio::contract(receiver, code, ds) {}

    [[eosio::action]]
    void upsert(name user, std::string cstm, std::string tmpCertifi) {
	require_auth(user);
	doc_index tmpdoc( get_self(), get_first_receiver().value);
	//std::string temp_hash = "tmp_certification_data";	 
    checksum256 hash = sha256(tmpCertifi.data(), tmpCertifi.size());

	int flags = 0;

	auto it = tmpdoc.find(user.value);
	if( it == tmpdoc.end()){
	  	tmpdoc.emplace( user, [&](auto& row) {
		   	row.key = user;
            row.certificount = 1;

			userlist tmp; tmp.name = cstm; 
			userdata ttmp; ttmp.certifinum = row.certificount;
			ttmp.user_hash = hash;
			tmp.data.push_back(ttmp);
			
			row.users.push_back(tmp);   
			flags ++;   
		    });
        }
	auto it2 = tmpdoc.find(user.value);
	if( it2 != tmpdoc.end() && flags == 0){
		int tmpFlags = 0;
		tmpdoc.modify(it2, user, [&](auto& row){
			if(row.users.empty()){
					++row.certificount; userdata ttmp; ttmp.certifinum = row.certificount; 
					ttmp.user_hash = hash;

					userlist tmp; tmp.name = cstm; tmp.data.push_back(ttmp);
					row.users.push_back(tmp);
					
				}
				 
			for(int i = 0; i < row.users.size(); i++){
				
				if(row.users[i].name == cstm){
					++row.certificount;  userdata ttmp; ttmp.certifinum = row.certificount; 
					ttmp.user_hash = hash;
					row.users[i].data.push_back(ttmp);
					tmpFlags = 1;
				}
			}
			if(tmpFlags==0){
					++row.certificount; userdata ttmp; ttmp.certifinum = row.certificount; 
					ttmp.user_hash = hash;

					userlist tmp; tmp.name = cstm; tmp.data.push_back(ttmp);
					row.users.push_back(tmp);
			}

		});
		}
	}

	[[eosio::action]]
	void checkinghash(name user, int certifinum,std::string certifi){
	require_auth(user);
	doc_index tmpdoc( get_self(), get_first_receiver().value);
	checksum256 hash = sha256(certifi.data(), certifi.size());

	int flag = 0;
	auto it = tmpdoc.find(user.value);
	if( it != tmpdoc.end()){
		tmpdoc.modify(it, user, [&](auto& row){
			for(int i = 0; i < row.users.size(); i ++){
				
				for(int j = 0; j < row.users[i].data.size(); j++){
					if(row.users[i].data[j].certifinum == certifinum){
						if(row.users[i].data[j].user_hash == hash){
							print("Correct certification!");
							flag = 1;
						}else{
							print("Another document!");
							flag = 1;
						}
					}
				}
			}
		});
		if( flag == 0){
			print("Non-exsist certification...");
		}
	}
	}
	

    private:
		struct userdata{
			uint64_t certifinum;
			checksum256 user_hash;
		};
		struct userlist{
			std::string name;
			std::vector<userdata> data;

		};
		struct [[eosio::table]] doc {
			name key;
			uint64_t certificount;

			std::vector<userlist> users;
			
			uint64_t primary_key() const { return key.value; }
		};
		using doc_index = eosio::multi_index<"hashdoc"_n, doc>;


};
