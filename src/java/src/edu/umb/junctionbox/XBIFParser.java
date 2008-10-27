/*
 * Copyright 1999-2002,2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package edu.umb.junctionbox;

import java.io.PrintWriter;
import java.lang.StringBuilder;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;
import java.nio.DoubleBuffer;

import org.xml.sax.Attributes;
import org.xml.sax.Parser;
import org.xml.sax.SAXException;
import org.xml.sax.SAXNotRecognizedException;
import org.xml.sax.SAXNotSupportedException;
import org.xml.sax.SAXParseException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.DefaultHandler;
import org.xml.sax.helpers.ParserAdapter;
import org.xml.sax.helpers.ParserFactory;
import org.xml.sax.helpers.XMLReaderFactory;

/**
 * A simple SAX parser for XBIF files
 *
 */
public class XBIFParser
    extends DefaultHandler {

    //
    // Constants
    //

    // feature ids

    /** Namespaces feature id (http://xml.org/sax/features/namespaces). */
    protected static final String NAMESPACES_FEATURE_ID = "http://xml.org/sax/features/namespaces";

    /** Namespace prefixes feature id (http://xml.org/sax/features/namespace-prefixes). */
    protected static final String NAMESPACE_PREFIXES_FEATURE_ID = "http://xml.org/sax/features/namespace-prefixes";

    /** Validation feature id (http://xml.org/sax/features/validation). */
    protected static final String VALIDATION_FEATURE_ID = "http://xml.org/sax/features/validation";

    /** Schema validation feature id (http://apache.org/xml/features/validation/schema). */
    protected static final String SCHEMA_VALIDATION_FEATURE_ID = "http://apache.org/xml/features/validation/schema";

    /** Schema full checking feature id (http://apache.org/xml/features/validation/schema-full-checking). */
    protected static final String SCHEMA_FULL_CHECKING_FEATURE_ID = "http://apache.org/xml/features/validation/schema-full-checking";

    /** Validate schema annotations feature id (http://apache.org/xml/features/validate-annotations) */
    protected static final String VALIDATE_ANNOTATIONS_ID = "http://apache.org/xml/features/validate-annotations";

    /** Dynamic validation feature id (http://apache.org/xml/features/validation/dynamic). */
    protected static final String DYNAMIC_VALIDATION_FEATURE_ID = "http://apache.org/xml/features/validation/dynamic";

    /** XInclude feature id (http://apache.org/xml/features/xinclude). */
    protected static final String XINCLUDE_FEATURE_ID = "http://apache.org/xml/features/xinclude";

    /** XInclude fixup base URIs feature id (http://apache.org/xml/features/xinclude/fixup-base-uris). */
    protected static final String XINCLUDE_FIXUP_BASE_URIS_FEATURE_ID = "http://apache.org/xml/features/xinclude/fixup-base-uris";

    /** XInclude fixup language feature id (http://apache.org/xml/features/xinclude/fixup-language). */
    protected static final String XINCLUDE_FIXUP_LANGUAGE_FEATURE_ID = "http://apache.org/xml/features/xinclude/fixup-language";

    // default settings

    /** Default parser name. */
    protected static final String DEFAULT_PARSER_NAME = "org.apache.xerces.parsers.SAXParser";

    /** Default repetition (1). */
    protected static final int DEFAULT_REPETITION = 1;

    /** Default namespaces support (true). */
    protected static final boolean DEFAULT_NAMESPACES = true;

    /** Default namespace prefixes (false). */
    protected static final boolean DEFAULT_NAMESPACE_PREFIXES = false;

    /** Default validation support (false). */
    protected static final boolean DEFAULT_VALIDATION = false;

    /** Default Schema validation support (false). */
    protected static final boolean DEFAULT_SCHEMA_VALIDATION = false;

    /** Default Schema full checking support (false). */
    protected static final boolean DEFAULT_SCHEMA_FULL_CHECKING = false;

    /** Default validate schema annotations (false). */
    protected static final boolean DEFAULT_VALIDATE_ANNOTATIONS = false;

    /** Default dynamic validation support (false). */
    protected static final boolean DEFAULT_DYNAMIC_VALIDATION = false;

    /** Default XInclude processing support (false). */
    protected static final boolean DEFAULT_XINCLUDE = false;

    /** Default XInclude fixup base URIs support (true). */
    protected static final boolean DEFAULT_XINCLUDE_FIXUP_BASE_URIS = true;

    /** Default XInclude fixup language support (true). */
    protected static final boolean DEFAULT_XINCLUDE_FIXUP_LANGUAGE = true;

    /** Default memory usage report (false). */
    protected static final boolean DEFAULT_MEMORY_USAGE = false;

    /** Default "tagginess" report (false). */
    protected static final boolean DEFAULT_TAGGINESS = false;

    protected static final boolean DEFAULT_APPLY_LOG_TRANSFORM = true;

    //
    // Data
    //

    /** Number of elements. */
    protected long fElements;
    /** Number of attributes. */
    protected long fAttributes;
    /** Number of characters. */
    protected long fCharacters;
    /** Number of ignorable whitespace characters. */
    protected long fIgnorableWhitespace;
    /** Number of characters of tags. */
    protected long fTagCharacters;
    /** Number of other content characters for the "tagginess" calculation. */
    protected long fOtherCharacters;
    /** Number variables model is over*/
    protected long fNumVbls;
    /** Number of observations*/
    protected long fNumObs;
    /** Maximum number of values in any variable*/
    protected long mNumVals;
    /** Maximum number of parents any variable*/
    protected long mNumPar;
    /** Maximum number of parents of current variable*/
    protected long cNumPar;

    /**Keep Track of what state the parser is in:
     0 = Outside of any tags we care about
     1 = Inside of a Variable tag
     2 = Inside of a Probability tag
     3 = Inside of a Values tag
     4 = Inside of a Given tag
     5 = Inside of a Table tag
     6 = Inside of an Observation tag
     7 = Inside of a For tag
     8 = Inside of a Name tag*/
    protected int parserState = 0;
    protected StringBuilder cbuf;
    protected Variable cVariable;
    protected Observation cObs;
    protected Table cTable;
    protected HashMap<String,Variable> variables;
    protected HashMap<String,Observation> obs;
    protected HashMap<String,Table> tables;

    //
    // Constructors
    //

    /** Default constructor. */
    public XBIFParser() {
    } // <init>()

    //
    // Public methods
    //

    /** Prints the results. */
    public void printResults(PrintWriter out, String uri, long time,
                             long memory, boolean tagginess,
                             int repetition,
                             HashMap<String,Observation> obs) {

        // filename.xml: 631 ms (4 elems, 0 attrs, 78 spaces, 0 chars)
        out.print("\n");
        out.print(uri);
        out.print(": ");
        if (repetition == 1) {
            out.print(time);
        }
        else {
            out.print(time);
            out.print('/');
            out.print(repetition);
            out.print('=');
            out.print(time/repetition);
        }
        out.print(" ms");
        if (memory != Long.MIN_VALUE) {
            out.print(", ");
            out.print(memory);
            out.print(" bytes");
        }
        out.print(" (");
        out.print(fElements);
        out.print(" elems, ");
        out.print(fAttributes);
        out.print(" attrs, ");
        out.print(fIgnorableWhitespace);
        out.print(" spaces, ");
        out.print(fCharacters);
        out.print(" chars, ");
        out.print(mNumPar);
        out.print(" Maximum Parents of any vbl, ");
        out.print(mNumVals);
        out.print(" Maximum Values for any vbl, ");
        out.print(fNumObs);
        out.print(" Total Observations, ");
        out.print(fNumVbls);
        out.print("  Variables )");
        if (tagginess) {
            out.print(' ');
            long totalCharacters = fTagCharacters + fOtherCharacters
                                 + fCharacters + fIgnorableWhitespace;
            long tagValue = fTagCharacters * 100 / totalCharacters;
            out.print(tagValue);
            out.print("% tagginess");
        }
        out.println();
        out.print("Observations: {");
        for(Observation o : obs.values()){
            out.print(" ("+o.variable+" : "+o.value+" ) ");
        }
        out.print(" } ");
        out.println();
        out.flush();

    } // printResults(PrintWriter,String,long)

    //
    // ContentHandler methods
    //

    /** Start document. */
    public void startDocument() throws SAXException {

        fElements            = 0;
        fAttributes          = 0;
        fCharacters          = 0;
        fIgnorableWhitespace = 0;
        fTagCharacters       = 0;
        fNumVbls             = 0;
        fNumObs              = 0;
        mNumPar              = 0;
        mNumVals             = 0;
        cbuf                 = new StringBuilder();
        variables            = new HashMap<String,Variable>();
        tables               = new HashMap<String,Table>();
        obs                  = new HashMap<String,Observation>();

    } // startDocument()

    /** Start element. */
    public void startElement(String uri, String local, String raw,
                             Attributes attrs) throws SAXException {

        fElements++;
        fTagCharacters++; // open angle bracket
        fTagCharacters += raw.length();
        if (attrs != null) {
            int attrCount = attrs.getLength();
            fAttributes += attrCount;
            for (int i = 0; i < attrCount; i++) {
                fTagCharacters++; // space
                fTagCharacters += attrs.getQName(i).length();
                fTagCharacters++; // '='
                fTagCharacters++; // open quote
                fOtherCharacters += attrs.getValue(i).length();
                fTagCharacters++; // close quote
            }
        }
        fTagCharacters++; // close angle bracket

        if(parserState==0){
            if(local.equalsIgnoreCase("Variable")){
                fNumVbls++;
                parserState = 1;//Assume validated
                //Start a new Variable Object
                cVariable = new Variable();
            }
            else if(local.equalsIgnoreCase("Probability")){
                parserState = 2;//Assume validated
                cTable = new Table();
                cNumPar = 0;
                //Start a new CPT Object
            }
        }
        else if(parserState==1){

            if(local.equalsIgnoreCase("Values")){
                parserState = 3;

            }else if(local.equalsIgnoreCase("Name")){
                parserState = 8;
            }
            else if(local.equalsIgnoreCase("Observed")){
                fNumObs++;
                parserState = 5;
                //Create an observation object for the current variable
                cObs = new Observation();
                cObs.variable = cVariable.name;
            }
        }
        else if(parserState==2){
            if(local.equalsIgnoreCase("Given")){
                parserState = 4;
                cNumPar++;
            }
            if(local.equalsIgnoreCase("Table")){
                parserState = 6;
                //
            }
            if(local.equalsIgnoreCase("For")){
                parserState = 7;
                //
            }
        }

        //switch on parser state
    } // startElement(String,String,StringAttributes)

public void endElement(String uri,
                       String local,
                       String qName)
    throws SAXException{
        if(parserState!=0){
            if(local.equalsIgnoreCase("Variable")){
                parserState = 0;//Assume validated
                //Finish new Variable Object
                variables.put(cVariable.name, cVariable);
            }
            else if(local.equalsIgnoreCase("Observed")){
                parserState = 1;//Assume validated
                cObs.value = Integer.parseInt(cbuf.toString().trim());
                obs.put(cObs.variable,cObs);
                cbuf.setLength(0);
            }else if(local.equalsIgnoreCase("Name")){
                parserState = 1;//Assume validated
                cVariable.name = cbuf.toString().trim();
                cbuf.setLength(0);
            }
            else if(local.equalsIgnoreCase("Probability")){
                parserState = 0;//Assume validated
                if(cNumPar > mNumPar)
                    mNumPar = cNumPar;
                //Finish  new CPT Object
                tables.put(cTable.forVbl,cTable);
            }
            else if(local.equalsIgnoreCase("Values")){
                //Handle values text
                //System.out.println(cbuf.toString());
                String valString = cbuf.toString().trim();
                int cNumVals =Integer.parseInt( valString);
                cVariable.numVals = cNumVals;
                if(cNumVals > mNumVals)
                    mNumVals = cNumVals;
                cbuf.setLength(0);
                parserState = 1;//Assume validated
            }
           else if(local.equalsIgnoreCase("Given")){
               //Add cbuf.toString() to parents array
               cTable.given.add(cbuf.toString().trim());
               cbuf.setLength(0);
               parserState = 2;//Assume validated
           }
           else if(local.equalsIgnoreCase("Table")){
               //Magically make a probability table
                //System.out.println(cbuf.toString());
               cTable.table = parseTable(cbuf.toString().trim());
               cbuf.setLength(0);
               parserState = 2;//Assume validated
           } else if(local.equalsIgnoreCase("For")){
               //System.out.println(cbuf.toString());
               cTable.forVbl = cbuf.toString().trim();
               cbuf.setLength(0);
               parserState = 2;//Assume validated
           }
           else{
               System.out.println(local);
               cbuf.setLength(0);
           }
        }
}// endElement(String,String,StringAttributes)

    /** Characters. */
    public void characters(char ch[], int start, int length)
        throws SAXException {
        fCharacters += length;
        if( parserState != 0){
            if(parserState != 6)
                cbuf.append(ch,start,length);
            else{//We remove newlines from tables to simplify parsing
                for(int i = start; i < start + length; i++){
                    if(ch[i] == '\n')
                        cbuf.append(' ');
                    else
                        cbuf.append(ch[i]);
                }
            }
        }

    } // characters(char[],int,int);

    /** Ignorable whitespace. */
    public void ignorableWhitespace(char ch[], int start, int length)
        throws SAXException {

        fIgnorableWhitespace += length;

    } // ignorableWhitespace(char[],int,int);

    /** Processing instruction. */
    public void processingInstruction(String target, String data)
        throws SAXException {
        fTagCharacters += 2; // "<?"
        fTagCharacters += target.length();
        if (data != null && data.length() > 0) {
            fTagCharacters++; // space
            fOtherCharacters += data.length();
        }
        fTagCharacters += 2; // "?>"
    } // processingInstruction(String,String)

    //
    // ErrorHandler methods
    //

    /** Warning. */
    public void warning(SAXParseException ex) throws SAXException {
        printError("Warning", ex);
    } // warning(SAXParseException)

    /** Error. */
    public void error(SAXParseException ex) throws SAXException {
        printError("Error", ex);
    } // error(SAXParseException)

    /** Fatal error. */
    public void fatalError(SAXParseException ex) throws SAXException {
        printError("Fatal Error", ex);
        //throw ex;
    } // fatalError(SAXParseException)

    //
    // Protected methods
    //

    /** Prints the error message. */
    protected void printError(String type, SAXParseException ex) {

        System.err.print("[");
        System.err.print(type);
        System.err.print("] ");
        if (ex== null) {
            System.out.println("!!!");
        }
        String systemId = ex.getSystemId();
        if (systemId != null) {
            int index = systemId.lastIndexOf('/');
            if (index != -1)
                systemId = systemId.substring(index + 1);
            System.err.print(systemId);
        }
        System.err.print(':');
        System.err.print(ex.getLineNumber());
        System.err.print(':');
        System.err.print(ex.getColumnNumber());
        System.err.print(": ");
        System.err.print(ex.getMessage());
        System.err.println();
        System.err.flush();

    } // printError(String,SAXParseException)

    int repetition = DEFAULT_REPETITION;
    boolean namespaces = DEFAULT_NAMESPACES;
    boolean namespacePrefixes = DEFAULT_NAMESPACE_PREFIXES;
    boolean validation = DEFAULT_VALIDATION;
    boolean schemaValidation = DEFAULT_SCHEMA_VALIDATION;
    boolean schemaFullChecking = DEFAULT_SCHEMA_FULL_CHECKING;
    boolean validateAnnotations = DEFAULT_VALIDATE_ANNOTATIONS;
    boolean dynamicValidation = DEFAULT_DYNAMIC_VALIDATION;
    boolean xincludeProcessing = DEFAULT_XINCLUDE;
    boolean xincludeFixupBaseURIs = DEFAULT_XINCLUDE_FIXUP_BASE_URIS;
    boolean xincludeFixupLanguage = DEFAULT_XINCLUDE_FIXUP_LANGUAGE;
    boolean memoryUsage = DEFAULT_MEMORY_USAGE;
    boolean tagginess = DEFAULT_TAGGINESS;
    boolean apply_log_transform = DEFAULT_APPLY_LOG_TRANSFORM;
    //
    // MAIN
    //

    /** Main program entry point. */
    public static void main(String argv[]) throws Exception{
	System.out.println( System.getProperty("java.class.path") );
        // is there anything to do?
        if (argv.length == 0) {
            printUsage();
            System.exit(1);
        }
        System.loadLibrary("junctionbox");
        // variables
        XBIFParser counter = new XBIFParser();
        PrintWriter out = new PrintWriter(System.out);
        XMLReader parser = null;
        // process arguments
        for (int i = 0; i < argv.length; i++) {
            String arg = argv[i];
            if (arg.startsWith("-")) {
                String option = arg.substring(1);
                if (option.equals("p")) {
                    // get parser name
                    if (++i == argv.length) {
                        System.err.println("error: Missing argument to -p option.");
                        continue;
                    }
                    String parserName = argv[i];

                    // create parser
                    try {
                        parser = XMLReaderFactory.createXMLReader(parserName);
                    }
                    catch (Exception e) {
			throw e;
			/*                        try {
                            Parser sax1Parser = ParserFactory.makeParser(parserName);
                            parser = new ParserAdapter(sax1Parser);
                            System.err.println("warning: Features and properties not supported on SAX1 parsers.");
                        }
                        catch (Exception ex) {
                            parser = null;
                            System.err.println("error: Unable to instantiate parser ("+parserName+")");
			    throw ex;
			    }*/
                    }
                    continue;
                }
                if (option.equals("x")) {
                    if (++i == argv.length) {
                        System.err.println("error: Missing argument to -x option.");
                        continue;
                    }
                    String number = argv[i];
                    try {
                        int value = Integer.parseInt(number);
                        if (value < 1) {
                            System.err.println("error: Repetition must be at least 1.");
                            continue;
                        }
                        counter.repetition = value;
                    }
                    catch (NumberFormatException e) {
                        System.err.println("error: invalid number ("+number+").");
                    }
                    continue;
                }
                if (option.equalsIgnoreCase("n")) {
                    counter.namespaces = option.equals("n");
                    continue;
                }
                if (option.equalsIgnoreCase("np")) {
                    counter.namespacePrefixes = option.equals("np");
                    continue;
                }
                if (option.equalsIgnoreCase("v")) {
                    counter.validation = option.equals("v");
                    continue;
                }
                if (option.equalsIgnoreCase("s")) {
                     counter.schemaValidation = option.equals("s");
                    continue;
                }
                if (option.equalsIgnoreCase("f")) {
                     counter.schemaFullChecking = option.equals("f");
                    continue;
                }
                if (option.equalsIgnoreCase("va")) {
                     counter.validateAnnotations = option.equals("va");
                    continue;
                }
                if (option.equalsIgnoreCase("dv")) {
                     counter.dynamicValidation = option.equals("dv");
                    continue;
                }
                if (option.equalsIgnoreCase("xi")) {
                     counter.xincludeProcessing = option.equals("xi");
                    continue;
                }
                if (option.equalsIgnoreCase("xb")) {
                     counter.xincludeFixupBaseURIs = option.equals("xb");
                    continue;
                }
                if (option.equalsIgnoreCase("xl")) {
                     counter.xincludeFixupLanguage = option.equals("xl");
                    continue;
                }
                if (option.equalsIgnoreCase("m")) {
                     counter.memoryUsage = option.equals("m");
                    continue;
                }
                if (option.equalsIgnoreCase("t")) {
                     counter.tagginess = option.equals("t");
                    continue;
                }
                if (option.equals("-rem")) {
                    if (++i == argv.length) {
                        System.err.println("error: Missing argument to -# option.");
                        continue;
                    }
                    System.out.print("# ");
                    System.out.println(argv[i]);
                    continue;
                }
                if (option.equals("h")) {
                    printUsage();
                    continue;
                }
                System.err.println("error: unknown option ("+option+").");
                continue;
            }

            // use default parser?
            if (parser == null) {

                // create parser
                try {
                    parser = XMLReaderFactory.createXMLReader(DEFAULT_PARSER_NAME);
                }
                catch (Exception e) {
		    throw e;
		    /*                    System.err.println("error: Unable to instantiate parser ("+DEFAULT_PARSER_NAME+")");
					  continue;*/
                }
            }

            // set parser features
            try {
                parser.setFeature(NAMESPACES_FEATURE_ID, counter.namespaces);
            }
            catch (SAXException e) {
                System.err.println("warning: Parser does not support feature ("+NAMESPACES_FEATURE_ID+")");
            }
            try {
                parser.setFeature(NAMESPACE_PREFIXES_FEATURE_ID, counter.namespacePrefixes);
            }
            catch (SAXException e) {
                System.err.println("warning: Parser does not support feature ("+NAMESPACE_PREFIXES_FEATURE_ID+")");
            }
            try {
                parser.setFeature(VALIDATION_FEATURE_ID, counter.validation);
            }
            catch (SAXException e) {
                System.err.println("warning: Parser does not support feature ("+VALIDATION_FEATURE_ID+")");
            }
            try {
                parser.setFeature(SCHEMA_VALIDATION_FEATURE_ID,  counter.schemaValidation);
            }
            catch (SAXNotRecognizedException e) {
                System.err.println("warning: Parser does not recognize feature ("+SCHEMA_VALIDATION_FEATURE_ID+")");

            }
            catch (SAXNotSupportedException e) {
                System.err.println("warning: Parser does not support feature ("+SCHEMA_VALIDATION_FEATURE_ID+")");
            }
            try {
                parser.setFeature(SCHEMA_FULL_CHECKING_FEATURE_ID,  counter.schemaFullChecking);
            }
            catch (SAXNotRecognizedException e) {
                System.err.println("warning: Parser does not recognize feature ("+SCHEMA_FULL_CHECKING_FEATURE_ID+")");

            }
            catch (SAXNotSupportedException e) {
                System.err.println("warning: Parser does not support feature ("+SCHEMA_FULL_CHECKING_FEATURE_ID+")");
            }
            try {
                parser.setFeature(VALIDATE_ANNOTATIONS_ID,  counter.validateAnnotations);
            }
            catch (SAXNotRecognizedException e) {
                System.err.println("warning: Parser does not recognize feature ("+VALIDATE_ANNOTATIONS_ID+")");

            }
            catch (SAXNotSupportedException e) {
                System.err.println("warning: Parser does not support feature ("+VALIDATE_ANNOTATIONS_ID+")");
            }
            try {
                parser.setFeature(DYNAMIC_VALIDATION_FEATURE_ID,  counter.dynamicValidation);
            }
            catch (SAXNotRecognizedException e) {
                System.err.println("warning: Parser does not recognize feature ("+DYNAMIC_VALIDATION_FEATURE_ID+")");

            }
            catch (SAXNotSupportedException e) {
                System.err.println("warning: Parser does not support feature ("+DYNAMIC_VALIDATION_FEATURE_ID+")");
            }
            try {
                parser.setFeature(XINCLUDE_FEATURE_ID,  counter.xincludeProcessing);
            }
            catch (SAXNotRecognizedException e) {
                System.err.println("warning: Parser does not recognize feature ("+XINCLUDE_FEATURE_ID+")");

            }
            catch (SAXNotSupportedException e) {
                System.err.println("warning: Parser does not support feature ("+XINCLUDE_FEATURE_ID+")");
            }
            try {
                parser.setFeature(XINCLUDE_FIXUP_BASE_URIS_FEATURE_ID,  counter.xincludeFixupBaseURIs);
            }
            catch (SAXNotRecognizedException e) {
                System.err.println("warning: Parser does not recognize feature ("+XINCLUDE_FIXUP_BASE_URIS_FEATURE_ID+")");

            }
            catch (SAXNotSupportedException e) {
                System.err.println("warning: Parser does not support feature ("+XINCLUDE_FIXUP_BASE_URIS_FEATURE_ID+")");
            }
            try {
                parser.setFeature(XINCLUDE_FIXUP_LANGUAGE_FEATURE_ID,  counter.xincludeFixupLanguage);
            }
            catch (SAXNotRecognizedException e) {
                System.err.println("warning: Parser does not recognize feature ("+XINCLUDE_FIXUP_LANGUAGE_FEATURE_ID+")");

            }
            catch (SAXNotSupportedException e) {
                System.err.println("warning: Parser does not support feature ("+XINCLUDE_FIXUP_LANGUAGE_FEATURE_ID+")");
            }

            // parse file
            parser.setContentHandler(counter);
            parser.setErrorHandler(counter);
            try {
                long timeBefore = System.currentTimeMillis();
                long memoryBefore = Runtime.getRuntime().freeMemory();
                for (int j = 0; j < counter.repetition; j++) {
                    parser.parse(arg);
                }
                long memoryAfter = Runtime.getRuntime().freeMemory();
                long timeAfter = System.currentTimeMillis();

                long time = timeAfter - timeBefore;
                long memory =  counter.memoryUsage
                            ? memoryBefore - memoryAfter : Long.MIN_VALUE;
                //For now, we issue a great many JNI calls to initialize the BN in C.
                //TODO Build mass acceptor methods into C to limit JNI overhead
                SWIGTYPE_p_List variablePointers = junctionbox.List_Create();
                SWIGTYPE_p_List obsPointers = junctionbox.List_Create();
                SWIGTYPE_p_List tablePointers = junctionbox.List_Create();
                for(Variable vbl :counter.variables.values()){
                    vbl.pointer = junctionbox.Variable_CreateDiscrete(vbl.name,vbl.numVals,null);
                    junctionbox.List_Append(variablePointers,vbl.pointer.getVoidPointer());
                }
                SWIGTYPE_p_VariableOrdering order = junctionbox.VariableOrdering_Create(variablePointers);
                for(Observation o : counter.obs.values()){
                    SWIGTYPE_p_Variable vbl = counter.variables.get(o.variable).pointer;
                    SWIGTYPE_p_VariableSet vs = junctionbox.VariableSet_Create(order);
                    junctionbox.VariableSet_Add(vs,vbl);
                    //TODO, when to destruct?
                    o.pointer =junctionbox.Observation_Create(vs, o.value);
                    junctionbox.List_Append(obsPointers,o.pointer.getVoidPointer());
                }
                for(Table t : counter.tables.values()){
                    SWIGTYPE_p_Variable vbl = counter.variables.get(t.forVbl).pointer;
                    SWIGTYPE_p_VariableSet vs = junctionbox.VariableSet_Create(order);
                    junctionbox.VariableSet_Add(vs,vbl);
                    SWIGTYPE_p_List parents = junctionbox.List_Create();
                    for(String parent: t.given){
                        junctionbox.VariableSet_Add(vs,counter.variables.get(parent).pointer);
                        junctionbox.List_Append(parents,counter.variables.get(parent).pointer.getVoidPointer());
                    }

                    SWIGTYPE_p_Pot potential = junctionbox.Pot_CreateTabular("cptPot", vs, t.table);
                    t.pointer        = junctionbox.CPT_Create_ParentList(parents,vbl, potential);
                    junctionbox.List_Append(tablePointers,t.pointer.getVoidPointer());
                }
                SWIGTYPE_p_BNet bn = junctionbox.BNet_Create_List2(order, tablePointers, obsPointers);
                SWIGTYPE_p_VertexOrdering o = junctionbox.BNet_StateSpaceWeightedElimOrdering(bn);
                junctionbox.BNet_BuildElimTree(bn, o);
                junctionbox.BNet_InitializePotentialsLog(bn);
		//    junctionbox.Print_BNetLog(bn);
                junctionbox.BNet_CollectEvidenceLog(bn);
                //    junctionbox.Print_BNetLog(bn);
                junctionbox.BNet_DistributeEvidenceLog(bn);
		junctionbox.Print_BNet(bn);
		// counter.printResults(out, arg, time, memory,  counter.tagginess,
                //                     counter.repetition, counter.obs);
                //Debugging
                //JunctionTreeTester jtt = new JunctionTreeTester(counter.variables,counter.obs,counter.tables,bn);
                //jtt.testMarginalConsistency();

            }
            catch (SAXParseException e) {
                // ignore
            }
            catch (Exception e) {
                System.err.println("error: Parse error occurred - "+e.getMessage());
                Exception se = e;
                if (e instanceof SAXException) {
                    se = ((SAXException)e).getException();
                }
                if (se != null)
                  se.printStackTrace(System.err);
                else
                  e.printStackTrace(System.err);

            }
        }

    } // main(String[])
    // thanks to cybereal from #java for some of this
    public SWIGTYPE_p_double parseTable(String s) {
        int bufSize = 1024;
        List<double[]> doubles = new ArrayList<double[]>();
        double[] buf = new double[bufSize];
        int doubleCount = 0;
        String[] parts = s.split("\\s");
        for(String p : parts){
            try{
                double d=Double.parseDouble(p.trim());
                if(apply_log_transform){
                    d = Math.log(d);
                }
                if (doubleCount >= buf.length) {
                    doubles.add(buf);
                    buf = new double[bufSize];
                    doubleCount = 0;
                }
                buf[doubleCount] = d;
                doubleCount++;
            }catch(NumberFormatException e){
                //   System.out.println("Lousy Doubles!" + p);
            }
        }
        int finalSize = (doubles.size() * bufSize) + doubleCount;
        SWIGTYPE_p_double retDouble = junctionbox.new_doubleArray(finalSize);
        int dubIdx = 0;
        for (double[] dub : doubles) {
            for(int i = 0; i < dub.length;i++){
                junctionbox.doubleArray_setitem(retDouble,dubIdx * bufSize + i,dub[i]);
            }
            dubIdx++;
        }
        for(int i = 0; i < doubleCount;i++){
            junctionbox.doubleArray_setitem(retDouble,dubIdx * bufSize + i,buf[i]);
        }
        return retDouble;
    }
    //
    // Private static methods
    //

    /** Prints the usage. */
    private static void printUsage() {

        System.err.println("usage:");
        System.err.println();

        System.err.println("options:");
        System.err.println("  -p name     Select parser by name.");
        System.err.println("  -x number   Select number of repetitions.");
        System.err.println("  -n  | -N    Turn on/off namespace processing.");
        System.err.println("  -np | -NP   Turn on/off namespace prefixes.");
        System.err.println("              NOTE: Requires use of -n.");
        System.err.println("  -v  | -V    Turn on/off validation.");
        System.err.println("  -s  | -S    Turn on/off Schema validation support.");
        System.err.println("              NOTE: Not supported by all parsers.");
        System.err.println("  -f  | -F    Turn on/off Schema full checking.");
        System.err.println("              NOTE: Requires use of -s and not supported by all parsers.");
        System.err.println("  -va | -VA   Turn on/off validation of schema annotations.");
        System.err.println("              NOTE: Requires use of -s and not supported by all parsers.");
        System.err.println("  -dv | -DV   Turn on/off dynamic validation.");
        System.err.println("              NOTE: Not supported by all parsers.");
        System.err.println("  -xi | -XI   Turn on/off XInclude processing.");
        System.err.println("              NOTE: Not supported by all parsers.");
        System.err.println("  -xb | -XB   Turn on/off base URI fixup during XInclude processing.");
        System.err.println("              NOTE: Requires use of -xi and not supported by all parsers.");
        System.err.println("  -xl | -XL   Turn on/off language fixup during XInclude processing.");
        System.err.println("              NOTE: Requires use of -xi and not supported by all parsers.");
        System.err.println("  -m  | -M    Turn on/off memory usage report");
        System.err.println("  -t  | -T    Turn on/off \"tagginess\" report.");
        System.err.println("  --rem text  Output user defined comment before next parse.");
        System.err.println("  -h          This help screen.");

        System.err.println();
        System.err.println("defaults:");
        System.err.println("  Parser:     "+DEFAULT_PARSER_NAME);
        System.err.println("  Repetition: "+DEFAULT_REPETITION);
        System.err.print("  Namespaces: ");
        System.err.println(DEFAULT_NAMESPACES ? "on" : "off");
        System.err.print("  Prefixes:   ");
        System.err.println(DEFAULT_NAMESPACE_PREFIXES ? "on" : "off");
        System.err.print("  Validation: ");
        System.err.println(DEFAULT_VALIDATION ? "on" : "off");
        System.err.print("  Schema:     ");
        System.err.println(DEFAULT_SCHEMA_VALIDATION ? "on" : "off");
        System.err.print("  Schema full checking:     ");
        System.err.println(DEFAULT_SCHEMA_FULL_CHECKING ? "on" : "off");
        System.err.print("  Dynamic:    ");
        System.err.println(DEFAULT_DYNAMIC_VALIDATION ? "on" : "off");
        System.err.print("  XInclude:   ");
        System.err.println(DEFAULT_XINCLUDE ? "on" : "off");
        System.err.print("  XInclude base URI fixup:  ");
        System.err.println(DEFAULT_XINCLUDE_FIXUP_BASE_URIS ? "on" : "off");
        System.err.print("  XInclude language fixup:  ");
        System.err.println(DEFAULT_XINCLUDE_FIXUP_LANGUAGE ? "on" : "off");
        System.err.print("  Memory:     ");
        System.err.println(DEFAULT_MEMORY_USAGE ? "on" : "off");
        System.err.print("  Tagginess:  ");
        System.err.println(DEFAULT_TAGGINESS ? "on" : "off");

        System.err.println();
        System.err.println("notes:");
        System.err.println("This parser is based off of the sax.XBIFParser pasers that ships with Xerces-js");
        System.err.println();
        System.err.println("");

    } // printUsage()

} // class XBIFParser
