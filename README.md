# eos-generic-ledger

## Pre-requisites
* An available testnet with a loaded eos.bios contract


## How does it work
The eos-generic-ledger, dubbed bank for simplicity, is a simple contract that stores notes for any user who wishes to record a message. 

## To submit a note
```
cleos push action bank submit_note '["user","a note from user"]' -p user
```

## To query notes
To see a list of all notes, use the command below:
```
cleos get table bank ledger statz
```
