Table info
-------------
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
-------------
upsert
==============





