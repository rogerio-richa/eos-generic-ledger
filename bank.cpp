#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/singleton.hpp>

using namespace eosio;
using namespace std;

class bank : public contract {
    using contract::contract;

    public:
        bank( account_name self ) :
            contract(self),
            _ledger(_self, N(ledger)),
            _ledger_config(_self, N(ledger))
    {}

        // @abi action
        void submit(name sender, string note) {
            
            require_auth(sender);
            // add here a check to only allow a specific user to manipulate this ledger

            if (!_ledger_config.exists()) {
                
                print("here");
                _ledger_info info{};
                info.height = 0;

                _ledger_config.set(info, _self); // what is _self? TO DO - read doc

                _ledger.emplace( sender, [&] (auto& row) {

                    row.height = 0;
                    row.note = note;
                });
                
                return;
            }
            
            _ledger_info info{};
            info = _ledger_config.get();
            info.height++;
            _ledger_config.set(info, _self);
            
            uint64_t height = (info.height);
            print(height);

            _ledger.emplace( sender, [&](auto& row) {

                row.height = height;
                row.note = note;
            });
        }

    private:
    
        struct _ledger_info {

            uint64_t height;
        };
        singleton<N(ledgerconf), _ledger_info> _ledger_config;

    
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
