/** \file
 ************************************************************************************
 * <h2> IXMLTestUnicode.cpp - Incredible XML parser test example - wchar_t* version </h2>
 *
 * @version     V3.12
 * @author      Frank Vanden Berghen
 *
 * Copyright (c) 2013, Frank Vanden Berghen - All rights reserved.<br/>
 * See the file "AFPL-license.txt" about the licensing terms
 *
 * This file contains 11 examples that demonstrates how to use the
 * different functionnalities of the Incredible XML Parser in wchar_t* mode.
 *
 ***********************************************************************************/

#ifdef WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <stdio.h>
#include "IXMLParser.h"
ToIXMLStringTool tx,tx2;
const wchar_t *t2=L"<a><b>some text</b><b>other text    </a>";

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
    ITCXMLNode xMainNode=iDom.openFileHelper(L"TestFiles/PMMLModel.xml",L"PMML");

    // this prints "RANK For <you>":
    ITCXMLNode xNode=xMainNode.getChildNode(L"Header");
    printf("  Application Name is: '%S'\n", xNode.getChildNode(L"Application").getAttribute(L"name"));

    // this prints "Hello World!"
    printf("  Text inside Header tag is :'%S'\n", xNode.getText());

    // this prints "urn"
    printf("  Value of the first attribute of the 'PMML/Extension/Key' tag is :'%S'\n", xMainNode.getChildNodeByPath(L"Extension/Key").getAttributeValue());

    // this gets the number of "NumericPredictor" tags:
    xNode=xMainNode.getChildNode(L"RegressionModel").getChildNode(L"RegressionTable");
    int n=xNode.nChildNode(L"NumericPredictor");

    // this prints the "coefficient" value for all the "NumericPredictor" tags:
    int i,myIterator=0;
    for (i=0; i<n; i++)
    printf("    coeff %i=%S\n",i+1,xNode.getChildNode(L"NumericPredictor",&myIterator).getAttribute(L"coefficient"));

    // this create a file named "testUnicode.xml" based on the content of the first "Extension" tag of the XML file:
    IXMLRenderer().writeToFile(xMainNode.getChildNode(L"Extension"),L"TestFiles/testUnicode.xml");

    printf("  The content of the clear tag is:%S\n",xMainNode.getChildNode(L"html_page").getChildNode(L"a").getClear().sValue);
}    

///////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                           //
///    Example 2: memory management: when to use the 'stringDup' and the 'free' functions     //
//                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////
void example2()
{
    printf("EXAMPLE 2\n");
    IXMLDomParser iDom;
    ITCXMLNode xMainNode=iDom.openFileHelper(L"TestFiles/PMMLModel.xml",L"PMML");

    // compare these 4 lines ...
    wchar_t *t=IXMLStringDup(xMainNode.getAttribute(L"version"));       // get version number
    iDom.clear();                                           // free from memory the complete xml Tree
    printf("  PMML Version :%S\n\n",t);                           // print version number
    free(t);                                                  // free version number

// ... with the following 3 lines (currently commented, because of error):
//  const wchar_t *t=xMainNode.getAttribute(L"version");      // get version number (note that there is no 'stringDup')
//  udom.clear();                                    // free from memory the complete xml Tree AND the version number inside the 't' var
//  printf("PMML Version :%S\n",t);            // since the version number in 't' has been free'd this will not work
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
//  <?xml version="1.0"?>
//      <body color="#FFFFFF"> Hello "universe". </body>
//      <A><B><C>pick-a-boo</C></B></A>
// ... and we transform it into a standard C string that is printed on screen.
    IXMLNode xMainNode,xNode;
    xMainNode=IXMLNode::createXMLTopNode();
    xNode=xMainNode.addChild(L"xml",TRUE);
    xNode.addAttribute(L"version",L"1.0");
    xNode=xMainNode.addChild(L"body");
    xNode.addText(L"Hello \"univ\"!");
    xNode.deleteText();
    xNode.addText(L"Hello \"universe\"!");
    xNode.addAttribute(L"color",L"#wrongcolor");
    xNode.updateAttribute(L"#FFFFFF",NULL,L"color");
    xMainNode.addChild(L"A").addChild(L"B").addChild(L"C").addText(L"pick-a-boo");

    IXMLRenderer uRender;
    IXMLCStr t=uRender.getString(xMainNode);
    printf("  XMLString created from scratch:\n%S",t);

    // we delete some parts:
    xNode.deleteAttribute(L"color");
    xMainNode.getChildNode(L"A").deleteNodeContent();
    t=uRender.getString(xMainNode,false);
    printf("\n  With the \"A\" tag deleted and the \"color\" attribute deleted:\n   %S\n\n",t);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                            //
///    Example 4: by default, the XML parser is "forgiving" with respect to errors inside XML strings&files    //
//                                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void example4()
{
    printf("EXAMPLE 4\n");

// By default, the XML parser is "forgiving":
// (You can de-activate this behavior: see the header of xmlParser.cpp file)
    IXMLResults xe;
    ICXMLNode xMainNode=IXMLDomParser().parseStringNonT(t2,NULL,&xe);
    IXMLRenderer iRender;
    IXMLCStr t=iRender.getString(xMainNode,false);
    printf("  The following XML: %S\n    ...is parsed as: %S\n  with the following info: '%S'\n\n",
        t2,t?t:L"(null)",IXMLPullParser::getErrorMessage(xe.errorCode));
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
    xMainNode.getChildNode(L"b",1).deleteNodeContent();

    // To perform the same "delete" as above, we can also do:
    // xNode=xMainNode.getChildNode(L"a").getChildNode(L"b",1); xNode.deleteNodeContent(); xNode=XMLNode::emptyXMLNode;
    // If you forget the last part of the delete ("xNode=XMLNode::emptyXMLNode"), then the XMLNode will NOT be deleted:
    // As long as there exists a reference to an XMLNode, the smartPointer mechanism prevent the node to be deleted.

    // To perform the same "delete" as above, we can also do:
    // xNode=xMainNode.getChildNode(L"a").getChildNode(L"b",1); xNode.deleteNodeContent(true);
    // The "true" parameter will force the deletion, even if there still exists some references to the XMLNode.
    // This is however very dangerous because, after the delete, the xNode object is invalid and CANNOT be used anymore.
    // Unexpected results may appear if you still try to access the xNode object after its "forced" deletion.
    printf("  ...with the wrong node deleted: %S\n\n",IXMLRenderer().getString(xMainNode,false));
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
    IXMLNode xNode=IXMLDomParser().parseString(L"<c>hello</c>").deepCopy();

    xMainNode.addChild(xNode,0);
    IXMLRenderer uRender;
    printf("  We inserted a new node 'c' as the first tag inside 'a':\n       %S",uRender.getString(xMainNode,false));
    xMainNode.addChild(xNode,xMainNode.positionOfChildNode(L"b",1));
    printf("\n  We moved the node 'c' at the position of the second 'b' tag:\n       %S\n\n",uRender.getString(xMainNode,false));
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
    IXMLReaderFile xmlfile(L"TestFiles/PMMLModel.xml");
    IXMLPullParser pp(&xmlfile);
    pp.findPath(L"PMML/RegressionModel/RegressionTable");
    ITCXMLNode xMainNode;
    IXMLDomParser iDom;
    iDom.setRemoveClears(false);
    for(;;)
    {
        xMainNode=iDom.parseOneChild(&pp);
        if (xMainNode.nElement()==0) break;
        printf("    <%S %S='%S' %S='%S'/>\n",xMainNode.getName(),xMainNode.getAttributeName(0),xMainNode.getAttributeValue(0),
                                             xMainNode.getAttributeName(1),xMainNode.getAttributeValue(1));
    }
    pp.findPath(L"../html_page");
    xMainNode=iDom.parseOneChild(&pp);
    printf("  The content of the clear tag is:%S\n",xMainNode.getClear().sValue);
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
      "  To be able to include any binary data into an xml file, some Base64\n"
      "  conversion functions (binary data <--> ascii text) are provided:\n"
      "    original binary data   : %s\n"
      "    encoded as text        : %S\n",originalBinaryData,t);
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
    printf("  Processing XML file containing chinese,cyrilic and other extended characters.\n");
    ICXMLNode xMainNode=IXMLDomParser().parseFileNonT(L"TestFiles/utf8test.xml");
    IXMLRenderer().writeToFile(xMainNode,L"TestFiles/outputTestWideChar.xml");
    printf("  ... resulting multi-lingual file is 'outputTestWideChar.xml'.\n\n");
}

////////////////////////////////////////////////////////////
//                                                        //
///  Example 10: usage of the "getParentNode()" method    //
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
    xNode=xMainNode.getChildNode();     xNode=xNode.getParentNode();         t=(wchar_t*)    xNode.getName(); printf("   Ex1: Name of top node; '%S'\n",t?t:L"null");
    
    xMainNode=IXMLDomParser().parseString(t2).deepCopy();
    xMainNode=xMainNode.getChildNode(); xMainNode=xMainNode.getParentNode(); t=(wchar_t*)xMainNode.getName(); printf("   Ex2: Name of top node; '%S'\n",t?t:L"null");
}

//////////////////////////////////////////////////////////
//                                                      //
///   Example 11: usage of the ToXMLStringTool class    //
//                                                      //
//////////////////////////////////////////////////////////
void example11()
{
    printf("\nEXAMPLE 11\n");

    // For performance reason it's sometime better to use the old-style "fwprintf"
    // function to create a XML file directly without constructing first
    // a XMLNode structure. In such case, the ToXMLStringTool class comes in handy.

    const wchar_t *t3=L"Hello to the <\"World\">";
    printf("  ToXMLStringTool demo: Original String: %S\n"
           "                        Encoded in XML : %S\n",t3,ToIXMLStringTool().toXML(t3));

    // If you use several time (in different "fwprintf") the same ToXMLStringTool
    // object, then the memory allocation (needed to create the output
    // buffer) will be performed only once. This is very efficient, very fast.
    // Usually, I create a global instance of the ToXMLStringTool class named "tx" (see
    // line 42 of this file) and then I use "tx" everywhere. For example:
    const wchar_t *t4=L"I say 'pick-a-boo'!";
    printf("  Global ToXMLStringTool: %S\n",tx.toXML(t4));
    printf("  Global ToXMLStringTool: %S\n",tx.toXML(t3));

    // However you must be careful because sometime the output buffer might be
    // erased before being printed. The next example is not working:
    printf("  Error using ToXMLStringTool: %S\n"
           "                               %S\n",tx.toXML(t4),tx.toXML(t3));

    // However, this is working fine:
    printf("  Correct usage of ToXMLStringTool: %S\n"
           "                                    %S\n",tx.toXML(t4),tx2.toXML(t3));

    // Using the "ToXMLStringTool class" and the "fwprintf function" is THE most efficient
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
    IXMLReaderFile iReader(L"TestFiles/twitter.json");
    IJSONPullParser pp(&iReader);
    IXMLDomParser iDom;
    ITCXMLNode x=iDom.parse(&pp);
    printf("  Write back the JSON file as an XML File.\n");
    IXMLRenderer().writeToFile(x,L"TestFiles/twitterWideChar.xml");
    printf("  Search inside the JSON file using XPATH:\n"
           "   Content of 'results/[2]/metadata/recent_retweets' is '%S'\n\n",
           x.getElementByPath(L"results/[2]/metadata/recent_retweets"));
}

int main(int argc, char **argv)
{
    example1(); example2();  example3(); example4();  example5();  example6();
    example7(); example8();  example9(); example10(); example11(); example12();
    return 0;
}
