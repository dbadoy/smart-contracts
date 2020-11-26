## Include:
```
#include <eosio/singleton.hpp>
```

## Table:
```
struct [[eosio::table]] global {
			global() {}
			uint64_t myid = 100000000000000;

			EOSLIB_SERIALIZE( global, ( myid ))
};
typedef eosio::singleton< "global"_n, global > conf; /// singleton
 ```
 
## Function:
 ```
 enum id_type { user_id };
 
 uint64_t getid( id_type type ) {

	conf config(get_self(), get_self().value);

	_global = config.exists() ? config.get() : global{};

	uint64_t result = 0;
	
	if( type == user_id){
    result = ++_global.myid; 
  }
  else{
		check(false, "Type error !" );
	}

	config.set( _cstate, _self );
	return result;
}

 ```
 
## Use:
 ```
 uint64_t myId = getid( user_id );

 ```

 
Reference
========
CryptoLions - SimpleAssets

https://github.com/CryptoLions/SimpleAssets

