# tlv_writer (test task)

Read the content from json fine and dump it to binary format (using TLV).

Get two parameters in command line: name of input file and name of output file. If it is skipped the default names are `input.json` and `output.bin`

On the first line in json should contains a map between names of key and number. The following lines should contains the content. 
Each line should be the correct json (or started/ended with ',' separator).

Example: 

```
{"key1":1, "key2":2, "key3":3} 
{"key1":"value1", "key2":42, "key3":true},
{"key1":"value2", "key2":43, "key3":false},
{"key1":"value3", "key2":44, "key3":true},
{"key1":"value4", "key2":45, "key3":false},
{"key1":"value5", "key2":46, "key3":true}
```

The binary document at first contains a dump of map (first - size of map, name of first key, number of first key, ...).
After the map: number of key (not tlv!), value (tlv field)
Each tlv contains: type identifier(1 byte), length (optional, only for strings), content(1 byte for bool, 4 bytes for int and float)
