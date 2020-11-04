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
<h3>void upsert(name user, std::string cstm, std::string tmpCertifi)<h3><br>
user 테이블에 cstm(유저)의 tmpCertifi 정보를 입력 
1. cstm의 certifinum은 user에 certificount를 참고하여 지정된다.
2. user 테이블에 cstm이 존재하지 않을 경우, cstm을 등록해준 후 tmpCertifi를 추가한다.
3. user 테이블에 cstm이 존재할 경우, 입력 받은 tmpCertifi를 추가한다.  






