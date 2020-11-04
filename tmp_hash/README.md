table
-------------
> struct

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



certificate data
-------------
certifinum, certificate_hash 으로 구성<br>
-> certifinum은 key 계정에서 카운트하며 할당 해주어서 색인에 사용<br>




