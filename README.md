gw2_reverse
===========

*This repository is not maintained anymore, however it could well still work*

You can issue pull requests, and I will make sure to integrate them if they make sense, but I think you're better off forking this repository. I hope you'll find the content interesting enough!

## Interesting things to see

* Extracting all file formats (excluding the bxml ones) directly from the exe via an IDA script (yeah, that's quite nice for a reverser :D)
  - [misc/ida/parseANStructs.idc](https://github.com/ahom/gw2_reverse/blob/master/misc/ida/parseANStructs.idc)
  - The output is a working [010Editor](http://www.sweetscape.com/010editor/)'s template that is using the common types located here: [misc/templates](https://github.com/ahom/gw2_reverse/tree/master/misc/templates)

* Compression algorithm for data files
  - [src/gw2DatTools/compression/inflateDatFileBuffer.cpp](https://github.com/ahom/gw2_reverse/blob/master/src/gw2DatTools/compression/inflateDatFileBuffer.cpp#L203)

* Compression algorithm for texture files
  - [src/gw2DatTools/compression/inflateTextureFileBuffer.cpp](https://github.com/ahom/gw2_reverse/blob/master/src/gw2DatTools/compression/inflateTextureFileBuffer.cpp#L642)

## Author

* [Antoine Hom](https://github.com/ahom)
