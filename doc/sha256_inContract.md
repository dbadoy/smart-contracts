## Include:
```
#include <eosio/crypto.hpp>
```

## Use:
```
std::string temp = "my temp string.";

checksum256 my_hash = sha256(temp.data(), temp.size());
```

