#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

using namespace eosio;
using namespace std;

class bank : public contract {
    using contract::contract;

    public:
        bank( account_name self ) :
            contract(self),
            _ledger(_self, N(ledger)),
            _ledger_stats(_self, N(ledger))
    {}

        // @abi action
        void submit(name sender, string note) {
            
            require_auth(sender);
            
            // add here a check to only allow a specific user to manipulate this ledger

            auto iter = _ledger.find(0);
            if(iter == _ledger.end()) {
                
                _ledger.emplace( sender, [&] (auto& row) {
                    
                    row.height = 0;
                    row.note = note;
                });
                
                _ledger_stats.emplace( sender, [&](auto& row) {

                    row.id = N(ledger_depth);
                    row.height = 0;
                });
                
                return;
            }
            
            auto iter2 = _ledger_stats.find(N(ledger_depth));
            uint64_t height = (iter2->height);
            
            _ledger.emplace( sender, [&](auto& row) {

                row.height = ++height;
                row.note = note;
            });
            
            
            _ledger_stats.modify (iter2, sender, [&](auto& row) {
                
                row.height++;
            });
        }

    private:
    
        // @abi table ledgerstats
        struct _ledger_info {

            uint64_t id;
            uint64_t height;
            uint64_t primary_key() const {

                return id;
            }

            EOSLIB_SERIALIZE(_ledger_info, (id)(height))
        };
        multi_index<N(ledgerstats), _ledger_info> _ledger_stats;

    
        // @abi table ledger
        struct _ledger_entry {
            
            uint64_t height;
            string note;
            uint64_t primary_key() const {
                
                return height;
            }
            
            EOSLIB_SERIALIZE(_ledger_entry, (height)(note))
        };

        multi_index<N(ledger), _ledger_entry> _ledger;
};

EOSIO_ABI( bank, (submit) )
