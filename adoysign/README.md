Adoysign
========
Adoysign is an electronic signature platform using blockchain (EOS).

Feature
-----
> 1. Decentralization of documents to be signed - Documents are stored in IPFS.

> 2. Multiple people can sign the document without any special configuration. - Share the document and just wait for signature.

> 3. All signing steps are stored on the blockchain and managed transparently. -  Only the hash value is stored, so others cannot see the actual data.

<br><br>

Smart contract 
=================
Adoysign's smart contract.
<br>

Account
----- 
Jungle3 : adoycontract

Link : https://jungle3.bloks.io/account/adoycontract
<br>

## Tables
The table is structured **signs**, **doc**, **Global**.<br>
**signs** - This is the storage location of signCode.<br>
**doc** - This is the storage location of document's hash value and signature data.<br>
**Global** - Automatic increment using sigleton.<br><br>

> signs
```
uint_64 signnum
checksum256 signhashvalue
bool use   // 'use' prevents reuse of signcode.
```
> doc
```
uint_64 docnum
checksum256 originhash // document's hash value
vector <signer> signs // The storage vector of signature data for document

// vector <signer> structured like this,
struct signer {
  name signer
  uint64_t signnum
  string signcode
};

```
  
> Global  
```
uint_64 signnum
uint_64 docnum  
// Auto increase
```
 
## Actions
> Sethash
```
void sethash(name user, checksum256 signhash)


```
