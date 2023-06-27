# tlv_writer

Read the content from json fine and dump it to binary format (using TLV).

On the first line in json should contains a map between names of key and number. The following lines should contains the content. 
Each line should be the correct json (or started/ended with ',' separator).

The binary document at first contains a dump of map (first - size of map, name of first key, number of first key, ...).
After the map: number of key (not tlv!), value (tlv field)
Each tlv contains: type identifier(1 byte), length (optional, only for strings), content(1 byte for bool, 4 bytes for int and float)