# Description
Structure token contain offset, length and next element. I try to compress work "War and Peace" by Leo Tolstoy. As result compression: 2.7 MB to 1.4 MB

* Script launchnig:
~~~
./compressor --file=example.txt --out=example.compressed
~~~

## Encoding sequentence
* SB_SIZE - size of search buffer (dictionary - encoded data.)
* LA_SIZE - size of lookahead (The byte sequence from the coding position to the end of the input stream.)
* BIT_IO_BUFFER - size compressed file.
* MAX_BIT_BUFFER - size header buffer. (put SB_SIZE and LA_SIZE for decode.) 

## Sources
- [LZ77 Compression Algorithm](https://docs.microsoft.com/ru-RU/openspecs/windows_protocols/ms-wusp/fb98aa28-5cd7-407f-8869-a6cef1ff1ccb)
