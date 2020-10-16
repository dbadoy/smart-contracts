#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
using namespace eosio;
using namespace std;

class [[eosio::contract("alibi")]] alibi : public eosio::contract {
   //friend void token::transfer( const name&    from,const name&    to,const asset&   quantity, const string&  memo );
      struct certificate{
         name user;
         name manager_name;
         string store_name;
         string start_day;
         string end_day;
         string retire_day;
         string working_day;
         vector<string> claim_contents;
         certificate(){ 
		      claim_contents.push_back("--initial--");	
         }
         EOSLIB_SERIALIZE( certificate, (user)(manager_name)(store_name)(start_day)(end_day)(retire_day)(working_day)(claim_contents) )
      };

   public: 
      alibi(name receiver, name code, datastream<const char*> ds) : contract(receiver, code, ds) {}

      [[eosio::action]]
      void upsert(name user, name manager_name, string store_name, string start_day, string end_day, string retire_day){
    	   require_auth(user);

         user_index me( get_self(), get_first_receiver().value);
         //auto iterator = me.find(user.value);
         auto itr = me.find(user.value);
         if(itr == me.end()){
            me.emplace(user,[&](auto& row){
                  if(row.my_certifi.empty()) { row.user = user; }
            });
         }
         auto failureMessage = "Failure to follow contract of the working days";

         if(retire_day == "0"){   // retire_day = 0 -> first input
               certificate tmpCer;
               tmpCer.user = user;
               tmpCer.manager_name = manager_name;
               tmpCer.store_name = store_name;
               tmpCer.start_day = start_day;
               tmpCer.end_day = end_day;
               tmpCer.retire_day = "0";
               tmpCer.working_day = "0";
               auto iterator = me.find(user.value);

               if(iterator != me.end()){
                     me.modify(iterator, user, [&](auto& row){
                        row.user = user; 
                        row.my_certifi.push_back(tmpCer);
               });
             }
         } else { 
               auto iterator = me.find(user.value);
               if(iterator != me.end()){
                 me.modify(iterator, user, [&]( auto& row ) { 
                     for(int i = 0; i < row.my_certifi.size(); i++){ 
                        if(row.my_certifi[i].store_name == store_name){
                            row.my_certifi[i].retire_day = retire_day;
                            row.my_certifi[i].working_day =  row.my_certifi[i].start_day + " ~ " + row.my_certifi[i].retire_day;
                            if(row.my_certifi[i].retire_day != row.my_certifi[i].end_day){
                                  row.my_certifi[i].claim_contents.push_back(failureMessage);
                            }                       
                        }       
                     }
                 });  
               }           
            }
         }

      [[eosio::action]]
      void editacl(name user, name target, int flag){
         require_auth(user);
         user_index me( get_self(), get_first_receiver().value);
         auto iterator = me.find(user.value);
         if( iterator != me.end()){
            me.modify(iterator, user, [&](auto& row){
               auto itr = find(row.my_accs.begin(), row.my_accs.end(), target);
                  if(flag == 0){                  
                     if(itr == row.my_accs.end()){
                        row.my_accs.push_back(target);
                     } else{
                        print("already exist user...");
                     }
                  } else if(flag ==1){
                     if(itr == row.my_accs.end()){
                        print("non-exist user...");
                     } else{
                        row.my_accs.erase(itr);
                     }
                  } else {
                     print("wrong flag...[ 0 : input / 1 : erase ]");
                  }
            });
         }      
      }
      [[eosio::action]]
      void searchacl(name user){
         require_auth(user);

         user_index me( get_self(), get_first_receiver().value);
         auto iterator = me.find(user.value);
         if( iterator != me.end()){
               me.modify(iterator, user, [&](auto& row){
                     for(int i =0; i < row.my_accs.size(); i ++){
                        print(row.my_accs[i]);
                     }
               });
         }
      }
   

      [[eosio::action]]
      void issuance(name searcher, name user, string store_name) {
         require_auth(searcher);
         auto permis_flag = 0;
         auto count = 0;

         user_index me( get_self(), get_first_receiver().value);
         auto iterator = me.require_find(user.value, "issusance");
         if( iterator != me.end()) {
            me.modify(iterator, user, [&](auto& row) {

                   auto itr = find(row.my_accs.begin(), row.my_accs.end(), searcher);
                   if( itr == row.my_accs.end() ){ permis_flag = 1;} 

                   if(permis_flag == 0){
                   for(int i = 0; i < row.my_certifi.size(); i++){ 
                        if(row.my_certifi[i].store_name == store_name){
                           print("Certification : ", name{user}, " worked in ", store_name);
                           print("startDay : ", row.my_certifi[i].start_day);
                           print("endDay : ", row.my_certifi[i].end_day);
                           print("retireDay : ", row.my_certifi[i].retire_day);
                           print("workingDay : ", row.my_certifi[i].working_day);
                           print("claims : ");
                           for(int j = 0; j < row.my_certifi[i].claim_contents.size(); j ++){
                              print(row.my_certifi[i].claim_contents[j]);
                              print("  ");
                           }
                        }else { count = count + 1;}
                     } 
                        if(count == row.my_certifi.size()){ print("No history"); }
                   } else { print("No auth");}
            });
         } 
      }




     [[eosio::action]]
     void claim(name claimer, name target, string store_name , string content) {
         require_auth( claimer );
         user_index target_user( get_self(), get_first_receiver().value);
         auto count = 0;
         auto iterator = target_user.find(target.value);
         if( iterator != target_user.end()) {
            target_user.modify(iterator, target, [&](auto& row){
                  for(int i = 0; i < row.my_certifi.size(); i++){
                        if(row.my_certifi[i].store_name == store_name){
                              if(claimer.value == row.my_certifi[i].user.value || claimer.value == row.my_certifi[i].manager_name.value){          
                                    row.my_certifi[i].claim_contents.push_back(content);
                              } else { print("No auth"); }                           
                        }else{ count = count + 1;}
                  }
                  if(count == row.my_certifi.size()) { print("No contract !");}
            });
         }
     }

   private:

      struct [[eosio::table]] userid {
         name user;
         vector<certificate> my_certifi;
         vector<name> my_accs;
         uint64_t primary_key() const { return user.value;}

         userid() { my_accs.push_back( user ); }

        EOSLIB_SERIALIZE( userid, (user) (my_certifi) (my_accs) )
      };

   using user_index = eosio::multi_index<"userlist"_n, userid>;
};
