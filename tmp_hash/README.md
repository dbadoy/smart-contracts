table
-------------
> struct
>   >struct [[eosio::table]] doc {
			name key;
			uint64_t certificount;

			std::vector<userlist> users;
			
			uint64_t primary_key() const { return key.value; }
		};

key(업체)의 계정에 여러 users(고객)에 대한 certificate data 보관<br>
users(고객)은 여러 certificate data를 가짐<br>

certificate data
-------------
certifinum, certificate_hash 으로 구성<br>
-> certifinum은 key 계정에서 카운트하며 할당 해주어서 색인에 사용<br>




