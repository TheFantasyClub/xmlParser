/** \file
 ****************************************************************************
 * <h2> IXMTest.cpp - Incredible XML Parser - test examples - char* version </h2>
 *
 * @version     V3.12
 * @author      Frank Vanden Berghen
 *
 * Copyright (c) 2013, Frank Vanden Berghen - All rights reserved.<br/>
 * See the file "AFPL-license.txt" about the licensing terms
 *
 * This file contains 11 examples that demonstrates how to use the
 * different functionnalities of the Incredible XML Parser in char* mode.
 *
 ****************************************************************************
 */
#ifdef WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <stdio.h>
#include "IXMLParser.h"

void myfree(char *t); // {free(t);}
ToIXMLStringTool tx,tx2;
const char *t2="<a><b>some text</b><b>other text    </a>";

///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
///   Example 1: Basic operations to parse and collect data from a XML file      //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
void example1()
{
    printf("EXAMPLE 1\n");

    // this open and parse the XML file:
    IXMLDomParser iDom;
    iDom.setRemoveClears(false);
    ITCXMLNode xMainNode=iDom.openFileHelper("TestFiles/PMMLModel.xml","PMML");

    // this prints "RANK For <you>":
    ITCXMLNode xNode=xMainNode.getChildNode("Header");
    printf("  Application Name is: '%s' (note that &lt; has been replaced by '<')\n", xNode.getChildNode("Application").getAttribute("name"));

    // this prints "Hello World!"
    printf("  Text inside Header tag is :'%s'\n", xNode.getText());

    // this prints "urn"
    printf("  Value of the first attribute of the 'PMML/Extension/Key' tag is :'%s'\n", xMainNode.getChildNodeByPath("Extension/Key").getAttributeValue());

    // this gets the number of "NumericPredictor" tags:
    xNode=xMainNode.getChildNode("RegressionModel").getChildNode("RegressionTable");
    int n=xNode.nChildNode("NumericPredictor");

    // this prints the "coefficient" value for all the "NumericPredictor" tags:
    int i,myIterator=0;
    for (i=0; i<n; i++)
        printf("    coeff %i=%s\n",i+1,xNode.getChildNode("NumericPredictor",&myIterator).getAttribute("coefficient"));

    // this create a file named "test.xml" based on the content of the first "Extension" tag of the XML file:
    IXMLRenderer().writeToFile(xMainNode.getChildNode("Extension"),"TestFiles/test.xml","ISO-8859-1");

    printf("  The content of the clear tag is:%s\n",xMainNode.getChildNode("html_page").getChildNode("a").getClear().sValue);
}    

///////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                           //
///   Example 2: memory management: when to use the 'stringDup' and the 'free' functions     //
//                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////
void example2()
{
    printf("EXAMPLE 2\n");
    IXMLDomParser iDom;
    ITCXMLNode xMainNode=iDom.openFileHelper("TestFiles/PMMLModel.xml","PMML");

    // compare these 4 lines ...
    char *t=IXMLStringDup(xMainNode.getAttribute("version"));   // get version number
    iDom.clear();                                           // remove (free) from memory the complete xml Tree
    printf("  PMML Version :%s\n\n",t);                     // print version number
    myfree(t);                                              // free version number

    // ... with the following 3 lines (currently commented, because of error):
    //  const char *t=xMainNode.getAttribute("version"); // get version number (note that there is no 'stringDup')
    //  iDom.clear();                                    // free from memory the complete xml Tree AND the version number inside the 't' var
    //  printf("PMML Version :%s\n",t);                  // since the version number in 't' has been free'd, this will not work
}

/////////////////////////////////////////////////////////////////
//                                                             //
///   Example 3: constructing & updating a tree of XMLNode     //
//                                                             //
/////////////////////////////////////////////////////////////////
void example3()
{
    printf("EXAMPLE 3\n");

    // We create in memory from scratch the following XML structure:
    //   <?xml version="1.0"?>
    //   <body color="FFFFFF"> Hello universe. </body>
    //   <A><B><C>pick-a-boo</C></B></A>
    // ... and we transform it into a standard C string that is printed on screen.
    IXMLNode xMainNode,xNode;
    xMainNode=IXMLNode::createXMLTopNode();
    xNode=xMainNode.addChild("xml",TRUE);
    xNode.addAttribute("version","1.0");
    xNode=xMainNode.addChild("body");
    xNode.addText("Hello \"univ\"!");
    xNode.deleteText();
    xNode.addText("Hello \"universe\"!");
    xNode.addAttribute("color","#wrongcolor");
    xNode.updateAttribute("#FFFFFF",NULL,"color");
    xMainNode.addChild("A").addChild("B").addChild("C").addText("pick-a-boo");

    IXMLRenderer uRender;
    IXMLCStr t=uRender.getString(xMainNode);
    printf("  XMLString created from scratch:\n%s",t);

    // we delete some parts:
    xNode.deleteAttribute("color");
    xMainNode.getChildNode("A").deleteNodeContent();
    t=uRender.getString(xMainNode,false);
    printf("\n  With the \"A\" tag deleted and the \"color\" attribute deleted:\n   %s\n\n",t);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                            //
///   Example 4: by default, the XML parser is "forgiving" with respect to errors inside XML strings&files    //
//                                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void example4()
{
    printf("EXAMPLE 4\n");

    // By default, the XML parser is "forgiving":
    // (You can de-activate this behavior: see the header of the xmlParser.cpp file)
    IXMLResults xe;
    ICXMLNode xMainNode=IXMLDomParser().parseStringNonT(t2,NULL,&xe);
    IXMLRenderer iRender;
    IXMLCStr t=iRender.getString(xMainNode,false);
    printf("  The following XML: %s\n    ...is parsed as: %s\n  with the following info: '%s'\n\n",
        t2,t?t:"(null)",IXMLPullParser::getErrorMessage(xe.errorCode));
}

/////////////////////////////////////////////////////////////
//                                                         //
///   Example 5: deleting a part of the tree of XMLNode    //
//                                                         //
/////////////////////////////////////////////////////////////
void example5()
{
    printf("EXAMPLE 5\n");
    IXMLNode xMainNode=IXMLDomParser().parseString(t2).deepCopy();

    // this deletes the "<b>other text</b>" subtree part:
    xMainNode.getChildNode("b",1).deleteNodeContent();

    // To perform the same "delete" as above, we can also do:
    // xNode=xMainNode.getChildNode("a").getChildNode("b",1); xNode.deleteNodeContent(); xNode=XMLNode::emptyXMLNode;
    // If you forget the last part of the delete ("xNode=XMLNode::emptyXMLNode"), then the XMLNode will NOT be deleted:
    // In this case, as long as there exists a reference to the XMLNode, the smartPointer mechanism prevent the node to be deleted.

    // To perform the same "delete" as above, we can also do:
    // xNode=xMainNode.getChildNode("a").getChildNode("b",1); xNode.deleteNodeContent(true);
    // The "true" parameter will force the deletion, even if there still exists some references to the XMLNode.
    // This is however very dangerous because, after the delete, the xNode object is invalid and CANNOT be used anymore.
    // Unexpected results may appear if you still try to access the xNode object after its "forced" deletion.
    printf("  ...with the wrong node deleted: %s\n\n",IXMLRenderer().getString(xMainNode,false));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                             //
///  Example 6: inserting (and moving) a new XMLNode in the middle of an already existing XMLNode structure    //
//                                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void example6()
{
    printf("EXAMPLE 6\n");
    // This creates a XMLNode 'a' that is "<a><b>some text</b><b>other text</b></a>":
    IXMLNode xMainNode=IXMLDomParser().parseString(t2).deepCopy();

    // This creates a XMLNode 'c' that is "<c>hello</c>":
    IXMLNode xNode=IXMLDomParser().parseString("<c>hello</c>").deepCopy();

    xMainNode.addChild(xNode,0);
    IXMLRenderer uRender;
    printf("  We inserted a new node 'c' as the first tag inside 'a':\n       %s",uRender.getString(xMainNode,false));
    xMainNode.addChild(xNode,xMainNode.positionOfChildNode("b",1));
    printf("\n  We moved the node 'c' at the position of the second 'b' tag:\n       %s\n\n",uRender.getString(xMainNode,false));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                             //
///  Example 7: enumerate all the content of a xmlNode in the order in which they appear inside the XML        //
//                                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void example7()
{
    printf("EXAMPLE 7\n");
    printf("  Enumeration of the 'RegressionTable' tag inside the PMML file:\n");
    IXMLReaderFile xmlfile("TestFiles/PMMLModel.xml");
    IXMLPullParser pp(&xmlfile);
    pp.findPath("PMML/RegressionModel/RegressionTable");
    ITCXMLNode xMainNode;
    IXMLDomParser iDom;
    iDom.setRemoveClears(false);
    for(;;)
    {
        xMainNode=iDom.parseOneChild(&pp);
        if (xMainNode.nElement()==0) break;
        printf("    <%s %s='%s' %s='%s'/>\n",xMainNode.getName(),xMainNode.getAttributeName(0),xMainNode.getAttributeValue(0),
                                            xMainNode.getAttributeName(1),xMainNode.getAttributeValue(1));
    }
    pp.findPath("../html_page");
    xMainNode=iDom.parseOneChild(&pp);
    printf("  The content of the clear tag is:%s\n",xMainNode.getClear().sValue);
}

////////////////////////////////////////////////
//                                            //
///   Example 8: base 64 encoding/decoding    //
//                                            //
////////////////////////////////////////////////
void example8()
{
    printf("\nEXAMPLE 8\n");

    unsigned char *originalBinaryData=(unsigned char *)"this is binary data.";
    IXMLParserBase64Tool b64;
    IXMLCStr t=b64.encode(originalBinaryData,21);
    printf(
      "  To be able to include any binary data into an xml file, some Base64"
      "\n  conversion functions (binary data <--> ascii/utf8 text) are provided:\n"
      "    original binary data   : %s\n"
      "    encoded as text        : %s\n",originalBinaryData,t);
    printf("    decoded as binary again: %s\n\n",b64.decode(t));
}

//////////////////////////////////////////////////////////////////////
//                                                                  //
///   Example 9: demonstration of multi-lingual XML file parsing    //
//                                                                  //
//////////////////////////////////////////////////////////////////////
void example9()
{
    printf("EXAMPLE 9\n");
    {
        printf("  Processing UTF-8 XML file containing chinese,cyrilic and other extended characters.\n");
        ICXMLNode xMainNode=IXMLDomParser().parseFileNonT("TestFiles/utf8test.xml");
        IXMLRenderer().writeToFile(xMainNode,"TestFiles/outputTestUTF8.xml");
        printf("  ... resulting multi-lingual file is 'outputTestUTF8.xml'.\n");
    }
    {
        printf("  Processing GB2312 XML file containing chinese characters.\n");
        IXMLDomParser iDom;
        iDom.setAutoDetectSourceEncoding(true);
        // Alternatively, you can also write:
        // iDom.setAutoDetectSourceEncoding(false);
        // iDom.setCharEncoding(IXMLPullParser::char_encoding_GB2312,IXMLPullParser::char_encoding_GB2312);
        ITCXMLNode xMainNode=iDom.parseFile("TestFiles/gb32test.xml");
        IXMLRenderer iRender;
        iRender.setCharEncoding(iDom.getCharEncodingSource());
        iRender.writeToFile(xMainNode,"TestFiles/outputTestGB32.xml");
        printf("  ... resulting GB2312 Chinese file is 'outputTestGB32.xml'.\n\n");
    }
}

////////////////////////////////////////////////////////////
//                                                        //
///   Example 10: usage of the "getParentNode()" method   //
//                                                        //
////////////////////////////////////////////////////////////
void example10()
{
    printf("EXAMPLE 10\n");
    printf("  Two examples of usage of the \"getParentNode()\" method:\n");
    // In the following two examples, I create a tree of XMLNode based on the string
    // "<a><b>some text</b><b>other text</b></a>". After parsing this string
    // I get a XMLNode that represents the <a> tag. Thereafter I "go down" one
    // level, using getChildNode: I now have a XMLNode that represents the <b> tag.
    // Thereafter I "go up" one level, using getParentNode(): I now have once again
    // a XMLNode that represents the <a> tag. Thereafter, I print the name ('a') of
    // this last XMLNode. The first example below is working as intended (it prints 'a'
    // on the screen). However, the second example below prints "null" because when we
    // did "xMainNode=xMainNode.getChildNode()" we lost all references to the
    // top node and thus it's automatically "garbage collected" (free memory).
    IXMLCStr t;
    IXMLNode xNode,xMainNode;
    
    xMainNode=IXMLDomParser().parseString(t2).deepCopy();
    xNode=xMainNode.getChildNode();     xNode=xNode.getParentNode();         t=(char*)    xNode.getName(); printf("   Ex1: Name of top node; '%s'\n",t?t:"null");
    
    xMainNode=IXMLDomParser().parseString(t2).deepCopy();
    xMainNode=xMainNode.getChildNode(); xMainNode=xMainNode.getParentNode(); t=(char*)xMainNode.getName(); printf("   Ex2: Name of top node; '%s'\n",t?t:"null");
}

//////////////////////////////////////////////////////////
//                                                      //
///   Example 11: usage of the ToXMLStringTool class    //
//                                                      //
//////////////////////////////////////////////////////////
void example11()
{
    printf("\nEXAMPLE 11\n");
    // For performance reason it's sometime better to use the old-style "fprintf"
    // function to create a XML file directly without constructing first
    // a XMLNode structure. In such case, the ToXMLStringTool class comes in handy.

    const char *t3="Hello to the <\"World\">";
    printf("  ToXMLStringTool demo: Original String: %s\n"
           "                        Encoded in XML : %s\n",t3,ToIXMLStringTool().toXML(t3));

    // If you use several time (in different "fprintf") the same instance of
    // the ToXMLStringTool class, then the memory allocation (needed to create the output
    // buffer) will be performed only once. This is very efficient, very fast.
    // Usually, I create a global instance of the ToXMLStringTool class named "tx" (see
    // line 22 of this file) and then I use "tx" everywhere. For example:
    const char *t4="I say 'pick-a-boo'!";
    printf("  Global ToXMLStringTool: %s\n",tx.toXML(t4));
    printf("  Global ToXMLStringTool: %s\n",tx.toXML(t3));

    // However you must be careful because sometime the output buffer might be
    // erased before being printed. The next example is not working:
    printf("  Error using ToXMLStringTool: %s\n"
           "                               %s\n",tx.toXML(t4),tx.toXML(t3));

    // However, this is working fine:
    printf("  Correct usage of ToXMLStringTool: %s\n"
           "                                    %s\n",tx.toXML(t4),tx2.toXML(t3));

    // Using the "ToXMLStringTool class" and the "fprintf function" is THE most efficient
    // way to produce VERY large XML documents VERY fast.
}

//////////////////////////////////////////////////////////
//                                                      //
///   Example 12: JSON Parser                           //
//                                                      //
//////////////////////////////////////////////////////////
void example12()
{
    printf("\nEXAMPLE 12\n  Read and parse a JSON file.\n");
    IXMLReaderFile iReader("TestFiles/twitter.json");
    IJSONPullParser pp(&iReader);
    IXMLDomParser iDom;
    ITCXMLNode x=iDom.parse(&pp);
    printf("  Write back the JSON file as an XML File.\n");
    IXMLRenderer().writeToFile(x,"TestFiles/twitter.xml");
    printf("  Search inside the JSON file using XPATH:\n"
        "   Content of 'results/[2]/metadata/recent_retweets' is '%s'\n\n",
        x.getElementByPath("results/[2]/metadata/recent_retweets"));
}

int main(int argc, char **argv)
{
    example1(); example2();  example3(); example4();  example5();  example6();
    example7();  example8(); example9(); example10(); example11(); example12();
    return 0;
}

#ifdef _USE_XMLPARSER_DLL
    // We are using the DLL version of the XMLParser library.
    // NOTE: With visual studio .NET, you can always use the standard "free()" function: You don't
    //       need a special "DLL free" version.
    void myfree(char *t){freeIXMLString(t);}
#else
    // we are using the normal, classical version of the XMLParser library (directly from C++ sources)
    void myfree(char *t){free(t);}
#endif

