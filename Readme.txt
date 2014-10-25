Incredible XMLParser v3.12
==========================

The library is composed by two files: "IXMLParser.cpp" and "IXMLParser.h".
These are the ONLY 2 files that you need when using the library inside your own projects.

All the functions of the library are documented inside the comments of the file "IXMLParser.h". 
These comments can be transformed in full-fledged HTML documentation using the DOXYGEN 
software: simply type: "doxygen doxy.cfg"

For more documentation, please refer either to the file "IXMLParser.h" or "Doc/html/index.html".

Change Log
----------

* V3.01: May 19, 2013: initial version.
* V3.02: May 23, 2013: Various bug fixes & improvements
* v3.03: May 24, 2013: Performed extensive testing on large documents and fixed some remaining small bugs.
* v3.04: May 28, 2013: changed the name from "Ultimate" to "Incredible" XMLParser.
* v3.05: May 30, 2013: 2 additions
   o added a JSON parser (see the IJSONPullParser class and the example12)
   o more advanced XPATH fonctionnality
* v3.06: June 12, 2013: 1 bug fix, 1 addition
   o FIX: compilation with new gcc.
   o added support for UTF32
* v3.07: June 26, 2013: 2 additions
   o added better support for "Max Memory Reached" error inside DOM parser
   o added better support to detect errors in JSON files
* v3.08: July 18, 2013: 2 additions
   o added advanced support for XPATH inside the Pull Parser (the DOM parser already supported advanced XPATH).
   o added primitive support for HTML: it's now possible to parse HTML documents!
* v3.09: September 17, 2013: 1 bug fix, 1 addition, 2 minor changes
   o FIX: rendering of gb2312-IXMLNode-tree to strings
   o added inside example 9 a code that shows how to hangle gb2312 xml files
   o changed the structure name "IXMLError" to "IXMLErrorInfo" to avoid collision with MSXML
   o changed the structure name "IXMLAttribute" to "IXMLAttr" to avoid collision with MSXML
* v3.10: December 20, 2013: 1 bug fix
   o FIX: parser sometime stopped parsing on very long tags
* v3.11: February 21, 2014: 4 additions
   o removed 64-bit compilation warnings
   o added "getAttribute(String)" method inside PullParser
   o various improvements to the "skipBranch()" method (e.g. support for the STRICT_PARSING option)
   o improved html support ("input" tag support)
* v3.12: June 9, 2014: 1 bug fix, 1 addition
   o fix compilation errors and memory-alignment error on WINCE+ARM platform: thanks to Marco Lizza for that!
   o improved XPATH support to search inside HTML files: We now ignore all "tbody" tags: You must modify all 
     your XPATH expressions in the following way: Replace all "/TBODY/" with "/".
