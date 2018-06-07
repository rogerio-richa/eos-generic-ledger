#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

using namespace eosio;
using namespace std;

class bank : public contract {
    using contract::contract;

    public:
        bank( account_name self ) :
            contract(self),
            _statuses(_self, N(ledger)) {}

        // @abi action
        void test(name sender, string note) {
            
            require_auth(sender);

            auto iter = _statuses.find(sender);
            if(iter == _statuses.end()) {
                
                _statuses.emplace( sender, [&]( auto& row) {
                    row.sender = sender;
                    row.note = note;
                });
            }
            else _statuses.modify( iter, 0, [&]( auto& row) {
                
               row.status = status;
            });
        }

    private:

        // @abi table statz
        struct _status {
            
            name sender;
            string note;
            name primary_key() const {
                
                return sender;
            }
            
            EOSLIB_SERIALIZE(_status, (sender)(note))
        };

        multi_index<N(statz), _status> _statuses;
};

EOSIO_ABI( basics, (test) )
