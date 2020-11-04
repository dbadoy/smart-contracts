Table info
==========
> table

	struct [[eosio::table]] doc {
			name key;
			uint64_t certificount;

			std::vector<userlist> users;
			
			uint64_t primary_key() const { return key.value; }
		};
		
> userlist
	
	struct userlist{
			std::string name;
			std::vector<userdata> data;

		};
		
> userdata

	struct userdata{
			uint64_t certifinum;
			checksum256 user_hash;
		};



Actions
=========
1. upsert
--------
<h2>void upsert(name user, std::string cstm, std::string tmpCertifi)<br><h2>
user의 계정에 cstm 유저와 그의 tmpCertifi를 입력 ( user 계정의 certificount를 참고하여 certifinum이 등록된다. )





