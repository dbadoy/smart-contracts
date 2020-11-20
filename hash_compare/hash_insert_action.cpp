using namespace std;

ACTION SimpleAssets::setcerthashd( name user, uint64_t certifinum, checksum256 certifihash, checksum256 retirementhash, string sign){
	//new certification input == certifinum >> 0 
	require_auth(user);
	string t = "NULL";
	const checksum256 hash_null = sha256(t.data(), t.size());


	const auto itr = certhashd.find(certifinum);
	if(itr == certhashd.end()){
			if(retirementhash == hash_null){
				certhashd.emplace( _self, [&]( auto& r) {
					r.certifinum = certifinum;	
					r.status = 1;
					r.certifihash = certifihash;
					r.retirementhash = hash_null;	
					r.usersign.push_back(sign);	
					r.targetsign.push_back("init");
					print(r.usersign[0]);
				});
				
			} else { check(false, "No exist certification. You cannot enter retire day hash value.");}
	}
	else{
		const auto itr = certhashd.find(certifinum);
			if(retirementhash == hash_null){
				certhashd.modify(itr, _self, [&]( auto& r) {
					if(r.status != 1) { check(false, "already exist certification"); }
					r.targetsign.pop_back();
					r.targetsign.push_back(sign);
					r.status = 2;
				});

			}
			else{
				certhashd.modify(itr, _self, [&]( auto& r) {
					if(r.status == 1) { check(false, "Both signatures have not been entered."); }
					if(r.status == 4) { check(false, "Completed certification"); }
					if(r.status == 3){
						r.targetsign.push_back(sign);
						r.status = 4;
					}	
					
					if(r.status == 2){
						r.retirementhash = retirementhash;
						r.usersign.push_back(sign);
						r.status = 3;
					}
		
				});
			}
			
	}
}
