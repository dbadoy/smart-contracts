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
upsert
------
### void upsert(name user, std::string cstm, std::string tmpCertifi)

cstm(유저)의 tmpCertifi입력<br>

	1. cstm의 certifinum은 user에 certificount를 참고하여 지정된다.
	2. user 테이블에 cstm이 존재하지 않을 경우, cstm을 등록해준 후 tmpCertifi를 추가한다.
	3. user 테이블에 cstm이 존재할 경우, 입력 받은 tmpCertifi를 추가한다.
	
<br>

checkinghash
------------
### void checkinghash(name user, int certifinum,std::string certifi)

user 테이블에 certifinum 을 찾아 certifi와 해시값 비교

	1. 해시값이 일치할 시, "Correct certification" 출력
	2. 해시값이 일치하지 않을 시, "Another certification" 출력
	3. certifinum이 존재하지 않을 시, "Non-exsist certification" 


<br><br>
Jungle3 - contract name
=======
### .
<br>

보완사항
========
	1. certification을 string으로 받고 있음  -> 실제로 넣어본 결과, certification 데이터를 JSON.parse , JSON.stringify 해준 후, toStiring('base64') 값을 넣으면 됨 ( nodejs - eosjs 환경 )
	2. 추가해야 할 액션
	

	
	

