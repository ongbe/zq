/* Copyright (c) 2003, Oracle Corporation.  All rights reserved.  */

/*
    Please see the online help (oraclec.hlp) 
	for documentation of these classes.
*/

/*
    Oracle Objects for OLE     C++ Classes
    
    This file is the header for the all classes.  That is, all the
    classes except OBound and OBinder (which use the obound.h header)
                           
    CREATED    ********   11/22/94
	SKAYOOR		MODIFIED    08/19/02
				Bug 2445796: Added a new member m_origname to OFIELD Class.
    SKAYOOR		MODIFIED	03/08/02
				Bug 1361495: Created a member in ODynaset to 
				hold the fieldname. This fieldname will freed 
				next when ODynaset::GetFieldName is called or 
				when ODynaset object is freed.
    RWOOLARD	MODIFIED	03/20/95
    			bug#	262914	*ErrorText should return const char *
    					262723	Added overloaded OParameter.Add(..., OValue)	
    PCHARI      MODIFIED        03/19/98
				bug#    624443  *NT 3.5.1 does not support CoInitializeEx
				and so this fix to explicitly link with ole32.dll
	MLAKSHMI	MODIFIED        01/06/99
				changed the OTYPE_ROWID value to 104 from 11
	PCHARI		MODIFIED	06/03/99
    			bug#	806756	Added critical section object

	CSHAY       MODIFIED    01/12/00
	            Oracle 8i support
*/

#ifndef ORACL_ORACLE
#define ORACL_ORACLE

// compiler specific defines
#ifdef WIN32
  #define  __huge
  #define OHUGESP
  #ifdef _MSC_VER
	  #ifdef OORACLMBUILD		
		 #define OEXPORT  __declspec(dllexport)
	  #else
         #define OEXPORT  __declspec(dllimport)
      #endif      
  #endif	  //	_MSC_VER
  #ifdef __BORLANDC__
     #define OEXPORT _export
  #endif     //	__BORLANDC__
#else     // WIN16
  #define OEXPORT _export
  #define OHUGESP
#endif    //WIN32


#ifndef OEXPORT
#define OEXPORT
#define OHUGESP
#endif

#include "windows.h"

#ifdef WIN32
# ifdef	ORAANSI
#  include <ole2.h>
#  include <winnls.h>
# endif
#else
# include <ole2.h>
# include <dispatch.h>
# ifdef _CID_MS15
#  define	OLECHAR char
# endif
#endif

// handy boolean type
typedef int oboolean;
// simple success return value
typedef int oresult;

// oresults can have one of the following values
#define OSUCCESS 0
#define OFAILURE 101

// error numbers returned by the ErrorNumber method.

// ----- error numbers
#define OERROR_NONE 0		// there isn't an error

#define OERROR_NOINTER 11	// we couldn't get a needed interface
#define OERROR_MEMORY 12	// memory allocation problem              
#define OERROR_BADERR 13	// error in error handling (!)
#define OERROR_INVPARENT 14 // parent object on open is invalid (not open)
#define OERROR_SYSTEM 15    // some system error
#define OERROR_NOTOPEN 16   // attempt to use unopened object
#define OERROR_BADARG 17    // bad argument to routine
#define OERROR_INVRECORD 18 // the current record is invalid
#define OERROR_BADTYPE 19   // invalid operation on an Oracle data type

#define OERROR_ADVISEULINK 4096		// not an advisory connection
#define OERROR_DBCONNECT 4097		// connection not made
#define OERROR_POSITION 4098		// invalid database position
#define OERROR_NOFIELDNAME 4099		// field not found
#define OERROR_NOFIELDINDEX 4100	// invalid field index
#define OERROR_TRANSIP 4101			// transaction already in progress
#define OERROR_SCHEMAERR 4102		// error retreiving table definition
#define OERROR_ORLONERR 4103		// unable to make connection
#define OERROR_TRANSNIPC 4104		// commit when no transaction 
#define OERROR_TRANSNIPR 4105		// rollback when no transaction
#define OERROR_NODSET 4106		    // no such dynaset attached to connection
#define OERROR_INVROWNUM 4108		// invalid row reference
#define OERROR_TEMPFILE 4109		// error creating temporary file
#define OERROR_DUPSESSION 4110		// duplicate session name
#define OERROR_NOSESSION 4111	    // no such session on detach
#define OERROR_NOOBJECTN 4112       // no object with specified name
#define OERROR_DUPCONN 4113			// duplicate connection
#define OERROR_NOCONN 4114		    // no such connection on detach
#define OERROR_BFINDEX 4115			// invalid field index
#define OERROR_CURNREADY 4116       // cursor not ready
#define OERROR_NOUPDATES 4117       // updates not allowed
#define OERROR_NOTEDITING 4118      // not currently editing
#define OERROR_DATACHANGE 4119		// data has changed since last read
#define OERROR_NOBUFMEM 4120		// no memory for binding buffers
#define OERROR_INVBKMRK 4121	    // invalid bookmark
#define OERROR_BNDVNOEN 4122		// bind variable not enabled
#define OERROR_DUPPARAM 4123		// duplicate parameter name
#define OERROR_INVARGVAL 4124		// invalid argument value
#define OERROR_INVFLDTYPE 4125		// invalid field type
#define OERROR_NOTIMPL 4126			// operation not implemented
#define OERROR_TRANSFORUP 4127	    // For Update detected, no transaction
#define OERROR_NOTUPFORUP 4128      // For Update detected, not updatable
#define OERROR_TRANSLOCK 4129		// Commit/Rollback, but trans locked
#define OERROR_CACHEPARM 4130		// Invalid cache parameter
#define OERROR_FLDRQROWID 4131   	// Field processing requires ROWID
#define OERROR_OUTOFMEMORY 4132	    // Out of Memory
#define OERROR_POINTER 4133		    // Invalid pointer
#define OERROR_INVNUMBER 4134		// Invalid number
#define OERROR_MAXSIZE 4135		    // Maximum size execeeded.
#define OERROR_INVDIMENSION 4136	// Invalid Dimension
#define OERROR_MAXBUFFER 4137		// Maximum buffer exceeds 32512 bytes.
#define OERROR_ARRAYSIZ 4138		// Array elements not same size

// Find Methods parser errors
#define OERROR_STACK_OVER 4496		// Parser : Stack Overflow
#define OERROR_SYNTAX ERROR 4497	// Parser : Syntax Error near
#define OERROR_MISPLACED_PAREN 4498	// Parser : Misplaced parentheses
#define OERROR_MISPLACED_QUOTE 4499	// Parser : Misplaced quotation marks
#define OERROR_MISSING PAREN 4500	// Parser : WARNING - Missing closing Parenthesis
#define OERROR_EXPECTED_PAREN 4501	// Parser : Usually open parentheses expected
#define OERROR_PARSER_UNKNOWN 4502	// Parser : Unknown parser error condition
#define OERROR_INVALID_FUNCTION 4503// Parser : Syntax not supported
#define OERROR_INVALID_COLUMN 4504	// Parser : Invalid Column Name
#define OERROR_MAX_TOKEN 4505		// Parser : Maximum Token size exceeded
#define OERROR_PARSER_DATA_TYPE 4506// Parser : Unsupported data type
#define OERROR_UNEXPECTED_TOKEN 4507// Parser : Unexpected token found
#define OERROR_END_OF_CLAUSE 4508	// Parser : Unexpected end of clause

// Find Methods runtime errors
#define OERROR_INVALID_INSTR 4516	// Runtime : Internal Error : Invalid Instruction
#define OERROR_STACK_ERROR 4517		// Runtime : Internal Error : Stack over/under-flow
#define OERROR_CONVERT_TYPES 4518	// Runtime : Invalid type conversion
#define OERROR_RUNTIME_DATA_TYPE 4519	// Runtime : Invalid datatype
#define OERROR_INVALID_SQL_ARG 4520	// Runtime : SQL function missing argument"
#define OERROR_INVALID_COMPARE 4521	// Runtime : Invalid comparison
#define OERROR_SELECT_DUAL 4522		// Runtime : Select from dual failed
#define OERROR_DUAL_DATATYPE 4523	// Runtime : Invalid datatype in Select from dual

#define OERROR_ECURSOR 8192			// cannot create cursor
#define OERROR_FETCHERR 8193	    // error fetching field
#define OERROR_BINDERR 8194			// output data binding error
#define OERROR_SQLERR 8195			// error in SQL statement
#define OERROR_ESQLEXEC 8196	    // SQL execution error
#define OERROR_COMERR 8197			// error during commit
#define OERROR_ROLERR 8198			// error during rollback
#define OERROR_OPTERR 8299			// error setting options
#define OERROR_CONNERR 8200			// unable to make connection
#define OERROR_RDBMSVER 8201		// Database version not available

//object and ref errors
#define OERROR_NOOBJECT	4796		//Creating Oracle object instance in client side object cache failed
#define OERROR_OBINDERR	4797		//Binding  Oracle object instance to the SQL statement failed
#define OERROR_NOATTRNAME 4798		//Getting attribute name of Oracle object instance failed
#define OERROR_NOATTRINDEX 4799		//Getting attribute index of Oracle object instance failed
#define OERROR_INVINPOBJECT 4801	//Invalid input object type for binding operation
#define OERROR_BAD_INDICATOR 4802	//Fetched Oracle Object instance comes with invalid indicator structure
#define OERROR_OBJINSTNULL 4803		//Operation on NULL Oracle object instance failed. 
#define OERROR_REFNULL 4804			//Pin Operation on NULL  Ref value failed. 

//lob errors
#define OERROR_INVPOLLPARAMS 4896	//Invalid amount and chunksize specified for LOB read/write operation.
#define OERROR_INVSEEKPARAMS 4897	//Invalid seek value is specified for LOB read/write operation.
#define OERROR_LOBREAD 4898			//Read operation failed
#define OERROR_LOBWRITE	4899		//Write operation failure 
#define OERROR_INVLOBLEN 4902		//Invalid buffer length for LOB write operation
#define OERROR_NOEDIT 4903			//Write,Trim ,Append,Copy operation is allowed outside the dynaset edit
#define OERROR_INVINPUTLOB 4904		//Invalid input LOB for bind operation
#define OERROR_NOEDITONCLONE 4905	//Write,Trim,Append,Copy is not allowed for clone LOB object
#define OERROR_LOBFILEOPEN 4906		//Specified file could not be opened in LOB operation 
#define OERROR_LOBFILEIOERR	4907	//File Read or Write failed in LOB Operation.
#define OERROR_LOBNULL 4908			//Operation on NULL LOB has failed. 

//aq errors
#define OERROR_AQCREATEERR 4996		//Error creating AQ object
#define OERROR_MSGCREATEERR	4997	//Error creating AQMsg object 
#define OERROR_PAYLOADCREATEERR 4998//Error creating Payload object
#define OERROR_MAXAGENTS 4998	    //Maximum number of subscribers exceeded. 
#define OERROR_AGENTCREATEERR 5000	//Error creating AQ Agent

//collection errors
#define OERROR_COLLINSTNULL	5196	//Operation on NULL Oracle collection failed. See IsNull property on OraCollection
#define OERROR_NOELEMENT 5197		//Element does not exist for given index
#define OERROR_INVINDEX	5198		//Invalid collection index is specified
#define OERROR_NODELETE	5199		//Delete operation is not supported for VARRAY collection type

// server data types
#define OTYPE_VARCHAR2	1
#define OTYPE_NUMBER	2
#define OTYPE_SINT	3
#define OTYPE_FLOAT	4
#define OTYPE_STRING    5
#define OTYPE_LONG	8
#define OTYPE_VARCHAR	9
#define OTYPE_DATE	12
#define OTYPE_RAW	23
#define OTYPE_LONGRAW	24
#define OTYPE_UINT      68
#define OTYPE_LONGVARRAW 95
#define OTYPE_CHAR	96
#define OTYPE_CHARZ	97
#define OTYPE_CURSOR    102
#define OTYPE_ROWID	104
#define OTYPE_MSLABEL	106
#define OTYPE_OBJECT	108
#define OTYPE_REF	110
#define OTYPE_COLL	122
#define OTYPE_BLOB	113
#define OTYPE_CLOB	112
#define OTYPE_BFILE	114
#define OTYPE_VARRAY	247
#define OTYPE_TABLE	248
#define OTYPE_METADATA 999 
#define OTYPE_RAW_BIN 2000

//additional datatypes for object attributes

#define	OTYPE_DECIMAL 7               
#define	OTYPE_REAL 21
#define	OTYPE_DOUBLE 22
#define	OTYPE_UNSIGNED8	  23	 
#define	OTYPE_RAW 23 
#define	OTYPE_UNSIGNED16  25
#define	OTYPE_UNSIGNED32  26  
#define OTYPE_SIGNED8	  27
#define OTYPE_SIGNED16	  28
#define OTYPE_SIGNED32	  29
#define OTYPE_PTR 32        
#define OTYPE_OPAQUE 58 
#define OTYPE_UINT 68
#define	OTYPE_LONGVARRAW 95       		
#define OTYPE_RSLT 116        
#define OTYPE_SYSFIRST 228
#define OTYPE_SYSLAST 235
#define OTYPE_OCTET	245
#define OTYPE_SMALLINT 246       
#define	OTYPE_OTMLAST 320

// edit modes for dynaset
#define ODYNASET_EDIT_NOEDIT 0
#define ODYNASET_EDIT_EDITING 1
#define ODYNASET_EDIT_NEWRECORD 2

// parameter io types
#define OPARAMETER_INVAR 1
#define OPARAMETER_OUTVAR 2
#define OPARAMETER_INOUTVAR 3

// parameter status flags
#define OPARAMETER_STATUS_IN 1
#define OPARAMETER_STATUS_OUT 2
#define OPARAMETER_STATUS_AUTOENABLED 4
#define OPARAMETER_STATUS_ENABLED 8

// Options for creating database
#define ODATABASE_DEFAULT 0
#define ODATABASE_PARTIAL_INSERT 1
#define ODATABASE_ORAMODE 1			//	Same as PARTIAL_INSERT 
#define ODATABASE_EDIT_NOWAIT 2
#define ODATABASE_NO_REFETCH	4
#define ODATABASE_NONBLOCK 8
#define ODATABASE_ENLIST_IN_MTS 16

// Options for creating dynaset
#define ODYNASET_DEFAULT 0
#define ODYNASET_NOBIND 1
#define ODYNASET_KEEP_BLANKS 2
#define ODYNASET_READONLY 4
#define ODYNASET_NOCACHE 8
#define ODYNASET_PARTIAL_INSERT 16	
#define ODYNASET_ORAMODE 16		
#define ODYNASET_NO_REFETCH	32
#define ODYNASET_NO_MOVEFIRST	64
#define ODYNASET_DIRTY_WRITE	128
#define	ODYNASET_VIEW_LONG		256 

// Options for creating  SqlStmt object
#define  OSQLSTMT_DEFAULT  0
#define  OSQLSTMT_NOBIND   1

#define OSQLSTMT_FAILEXEC  2
// NON-BLOCKING
#define OSQLSTMT_NONBLK		4
// Threading Model Options
#define OSTARTUP_MULTITHREADED 0
#define OSTARTUP_APARTMENTTHREADED 1

// definitions of actions used in the callback routines
#define OADVISE_MOVE_FIRST 1
#define OADVISE_MOVE_NEXT 2
#define OADVISE_MOVE_PREV 3
#define OADVISE_MOVE_LAST 4
#define OADVISE_FIND_FIRST 5
#define OADVISE_FIND_NEXT 6
#define OADVISE_FIND_PREV 7
#define OADVISE_FIND_LAST 8
#define OADVISE_DELETE 9
#define OADVISE_ADDNEW 10
#define OADVISE_REFRESH 11
#define OADVISE_MOVE_TOMARK 12
#define OADVISE_ROLLBACK 13
#define OADVISE_UPDATE 18
#define OADVISE_MOVE_NEXTN 20
#define OADVISE_MOVE_PREVN 21
#define	OADVISE_MOVETO	22

#define OADVISE_FOUNDLAST 151
#define OADVISE_OTHER 99

//Lob 
#define OLOB_ONE_PIECE 0
#define OLOB_FIRST_PIECE 1
#define OLOB_NEXT_PIECE 2
#define OLOB_LAST_PIECE 3

#define OLOB_SUCCESS 0
#define OLOB_NEED_DATA 99

//AQ 
#define OAQ_BOTH 0
#define OAQ_ENQUEUE	1
#define OAQ_DEQUEUE	2

#define OAQ_ENQ_IMMEDIATE 1
#define OAQ_ENQ_ON_COMMIT 2

#define OAQ_NULL_MSGID 0

#define OAQ_DQ_BROWSE 1
#define OAQ_DQ_LOCKED 2
#define OAQ_DQ_REMOVE 3

#define OAQ_DQ_FIRST_MSG 1
#define OAQ_DQ_NEXT_TRANS 2
#define OAQ_DQ_NEXT_MSG	3

#define OAQ_DQ_WAIT_FOREVER	-1
#define OAQ_DQ_NOWAIT 0

#define OAQ_MSG_NO_DELAY 0

#define OAQ_MSG_PRIORITY_NORMAL	0
#define OAQ_MSG_PRIORITY_HIGH -10
#define OAQ_MSG_PRIORITY_LOW 10

#define OAQ_MSG_NO_EXPIRE 0
#define OAQ_MAX_AGENTS 10

//Ref
#define OREF_NO_LOCK 1
#define OREF_EXCLUSIVE_LOCK 2
#define OREF_NOWAIT_LOCK 3

#define OREF_READ_ANY 3
#define OREF_READ_RECENT 4
#define OREF_READ_LATEST 5

//NON-BLOCKING
#define OSQL_STILL_EXECUTING -3123
#define OSQL_SUCCESS		 0

//MetaData
#define OMD_TABLE 1
#define OMD_VIEW 2
#define OMD_COLUMN 3
#define OMD_COLUMN_LIST	4
#define OMD_TYPE 5
#define OMD_TYPE_ATTR 6
#define OMD_TYPE_ATTR_LIST 7
#define OMD_TYPE_METHOD	8
#define OMD_TYPE_METHOD_LIST 9
#define OMD_TYPE_ARG 10
#define OMD_TYPE_RESULT	11
#define OMD_PROC 12
#define OMD_FUNC 13
#define OMD_ARG	14
#define OMD_ARG_LIST 15	
#define OMD_PACKAGE	16
#define OMD_SUBPROG_LIST 17
#define OMD_COLLECTION 18
#define OMD_SYNONYM	19
#define OMD_SEQENCE	20
#define OMD_SCHEMA 21
#define OMD_OBJECT_LIST	22
#define OMD_SCHEMA_LIST	23
#define OMD_DATABASE 24

//XML definitions
#define OXML_META_NONE 0
#define OXML_META_XMLSCHEMA 1
#define OXML_META_DTD 2

// definitions for UpdateSource sources
enum	updsrcs {SYSDATE, TIMESTAMP, DATESTAMP, FUNCTION, OTHFIELD, STRLITERAL};

// forward references
class OEXPORT OSession;
class OEXPORT OSessionCollection;
class OEXPORT OClient;
class OEXPORT OConnection;
class OEXPORT OConnectionCollection;
class OEXPORT ODatabase;
class OEXPORT ODynaset;
class OEXPORT ODynasetMark;
class OEXPORT OField;
class OEXPORT OFieldCollection;
class OEXPORT OAdvise;
class OEXPORT OParameter;
class OEXPORT OParamArray;
class OEXPORT OParameterCollection;
class OEXPORT OSqlStmt;
class OEXPORT OMDAttribute;
class OEXPORT OMetaData;
class OOLEvar;
class OEXPORT OObject;
class OEXPORT ORef;
class OEXPORT OCollection;
class OEXPORT OBlob;
class OEXPORT OClob;
class OEXPORT OBfile;
class OEXPORT OServer;
class OEXPORT OSnapshotID;
class OEXPORT OValue;
class OEXPORT OAQ;
class OEXPORT OAQMsg;
class OEXPORT OAQAgent;
class OEXPORT OAQCallback;
class OEXPORT OException;

// exception specification not supported by MSVC yet
#pragma warning( disable : 4290 ) 

// ------------------------------------------------------------
// functions that are not class methods

// routine to initialize library.  Should be called once at startup 
// returns TRUE if successful, FALSE if not
oboolean OEXPORT OStartup(int ThreadingModel = OSTARTUP_APARTMENTTHREADED); // Threading Model Options


// routine to close down the library.  Should be called once
// at application shutdown
void OEXPORT OShutdown(void);

class COraCriticalSection 
{
public:
	COraCriticalSection()
	{
		InitializeCriticalSection (&m_CritSect);
	}

	~COraCriticalSection()
	{
		DeleteCriticalSection (&m_CritSect);
	}
	
	void Lock()
	{
		EnterCriticalSection(&m_CritSect);
	}

	void Unlock()
	{		
		LeaveCriticalSection(&m_CritSect);
	}

private:
	CRITICAL_SECTION m_CritSect;
};

// ----- OOracleObject -----------------------------------------------
// base object class
//   This class is the base for the OO4W classes.  By itself
//   it provides the error reporting interface and helps with
//   the various copying mechanisms

class OEXPORT OOracleObject
{
friend ORef;
public:
    // constructors & destructors
    OOracleObject(void);
    OOracleObject(const OOracleObject &other);

    virtual ~OOracleObject(void);
    
    // overloaded operators
    OOracleObject &operator=(const OOracleObject &other);
    int operator==(const OOracleObject &other) const;
    int operator!=(const OOracleObject &other) const;
    
    // properties
    virtual oboolean IsOpen(void) const;
    
    // Error handling methods
    long  ErrorNumber(void) const; // return error "number"
    
    const char *LookupErrorText(long errnum) const;  // get error text for given error number
    const char *GetErrorText(void) const;  // get description of last error

    // set error information
    void  ErrorReset(void) const;  // reset error state to "no error"
    // SetOtherError and SetInternalError are for internal use only
    void  SetOtherError(void *otheri) const;
    void  SetInternalError(long errnum) const;
    
    void *Internal(void) const;
	void *DInternal() const;
    
protected:
    // copy and cleanup routines (used to implement destructor, constructor, =)
    virtual oresult Copy(const OOracleObject &other);
    virtual oresult Cleanup(void);
    
    // access to object interface
	oresult SetObjectInterface(void *obji, oboolean geterror = TRUE);
	oresult SetObjectInterface(void *dp, void *id);

    oresult ActionStart(void) const;  // start of most of the methods
    oresult ActionGetStart(const OOracleObject *nobj) const;

	OOracleObject(void *dp, void *id);
	OOracleObject(void *pImpl);

private:
    void   *m_obji;   // pointer to object interface
    void   *m_erri;   // pointer to error interface of object
    void   *m_errotheri;  // pointer to error interface on other object
    char   *m_lasterrstring;  // last error string we've handled
    
    int     m_errstate;   // where did the error come from (values in implementation)
	long    m_errno;  // error number (see below, or negative for internal values)
};


// ----- OOracleCollection -----------------------------------------------
// general set class
// This is a base class used for collections of sessions, connections and fields
// It has no utility on its own.  Routines to get items from the collection are
// in the subclasses

class OEXPORT OOracleCollection : public OOracleObject
{
public:

    // constructors & destructors
    OOracleCollection(void);
    OOracleCollection(const OOracleCollection &other);
    ~OOracleCollection(void);
    
    // Open & Close
    oresult Close(void);   // close the set
    
    // overloaded operators
    OOracleCollection &operator=(const OOracleCollection &other);
    
	 // # of items in the set
    long   GetCount(void) const;

protected:
    oresult OpenSetHelper(void *idisp, void *otheri, unsigned char stype);  // finish the work of opening the set
    void  *GetItem(unsigned char stype, int index) const;
    void  *GetItem(oboolean oflag, const char *name) const;

private:
    unsigned char m_settype;
    
	 // internal helper routines
    oresult Cleanup(void);
	oresult Copy(const OOracleCollection &other);
}                                                       ;

// ----- OSession -----------------------------------------------

class OEXPORT OSession : public OOracleObject
{
public:
    // construction & destruction
    OSession(void);
    OSession(const OSession &other); // copy constructor
    OSession(const char *sname);     // construct & open (NULL sname means open default session)
	~OSession(void);
    
    // open the session
    oresult Open(void);               // open the default session
    oresult Open(const char *sname);  // open a new session with specific name
    oresult Close(void);
    
    // Getting other objects
    OConnectionCollection GetConnections(void) const;
    OClient GetClient(void) const; 
    static OSession GetNamedSession(const char *sname);

    // overloaded operators
    OSession &operator=(const OSession &other);
	
	oresult CreateDatabasePool
	    (long initSize, long maxSize, long timeOut, 
		 const char *dbname, const char *username, 
		 const char *pwd,long options = ODATABASE_DEFAULT);
	
	ODatabase OSession::GetDatabaseFromPool(long waitTime);

	oresult OSession::GetUserPwd(const char *username, const char *pwd, char **retname);

	oresult OSession::DestroyDatabasePool();

	long OSession::GetDbPoolMaxSize() const;

	long OSession::GetDbPoolCurrentSize() const; 

	long OSession::GetDbPoolInitialSize() const;

	long OSession::GetDbPoolUsedCount() const;

    // error handling
    long ServerErrorNumber(void) const;
    const char *GetServerErrorText(void) const;
    oresult ServerErrorReset(void);
    
    // get properties
    const char       *GetName(void) const;   // returns session name 
    const char       *GetVersion(void) const;  // returns version of Oracle Objects
    
    // transaction operations
    oresult  BeginTransaction(void);  // start a transaction
    oresult  Commit(oboolean startnew = FALSE);   // commit (may start new transaction)
    oresult  Rollback(oboolean startnew = FALSE); // rolls back transaction (may start new transaction)
    oresult  ResetTransaction(void);  // unconditionally rollback (no advisories)
   
	//new to 8.1.6
	oresult OSession::ChangePassword(const char *dbalias, const char *username, const char *current_pass, const char *new_pass) const;
    
	// function used by other classes to construct OSession objects (don't call this!)
    oresult OpenHelper(void *idisp, void *otheri);

    // Memory Management Routines
    oresult  MemoryManager (int flags = -1);
    oresult  MemoryLog (int flags = -1);

private:
	char      *m_name;
	char      *m_errtext;
    char      *m_version;
    
    // internal helper routines
    oresult Cleanup(void);
    oresult Copy(const OSession &other);
    
};

// ----- OSessionCollection -----------------------------------------------

class OEXPORT OSessionCollection : public OOracleCollection
{
public:
    OSession GetSession(int index) const;

    // function used by other classes to construct OSessionCollection objects (don't call this!)
    oresult OpenHelper(void *idisp, void *otheri);  // finish the work of opening the set
};

// ----- ODatabase -----------------------------------------------

class OEXPORT ODatabase : public OOracleObject
{
public:
    // construction & destruction
    ODatabase(void);
    ODatabase(const ODatabase &other);
    ~ODatabase(void); 
    
    // construct & open
	 ODatabase(const OSession &dbsess, const char *dbname, const char *username,
					const char *pwd, long options = ODATABASE_DEFAULT);
	 ODatabase(const char *dbname, const char *username, const char *pwd,
					long options = ODATABASE_DEFAULT);

	 ODatabase(const OServer &server, const char *username, const char *pwd,
					long options = ODATABASE_DEFAULT);

	 // for the Open calls, if pwd is NULL it is assumed that username contains username/password

	 // open a database on a new session (implicitly creates a session)
	 oresult Open(const char *dbname, const char *username, const char *pwd,
						long options = ODATABASE_DEFAULT);
	 // open a database on an existing session
	 oresult Open(const OSession &dbsess, const char *dbname,
						const char *username, const char *pwd,
						long options = ODATABASE_DEFAULT);

	 oresult Open(const OServer &server, const char *username, const char *pwd,
						long options = ODATABASE_DEFAULT);

	 oresult Close(void);

	 // getting other objects
	 OSession GetSession(void) const;
	 OConnection GetConnection(void) const;
	 OParameterCollection GetParameters(void) const;

    // overloaded operators
    ODatabase &operator=(const ODatabase &other);
    
    // execute an arbitrary SQL statement
    oresult ExecuteSQL(const char *sqlstmt) const;
    long	GetRowsProcessed(void) const;

	oresult RemoveFromPool(void) const;

    //new methods in 8.1.6
	OMetaData Describe(const char *SchemaObjectName) throw(OException);
	oresult BeginTrans(void) const;
	oresult CommitTrans(void) const;
	oresult Rollback(void) const;

	 // properties
    const char *GetName(void) const;     // return database name (from the connection)
    const char *GetConnectString(void) const;  // return connect string (without password)
	const char *GetRdbmsVersion(void) const;		// returns RDBMS Version
	long        GetOptions(void) const;
    
    // error handling
    long ServerErrorNumber(void) const;
    const char *GetServerErrorText(void) const;
    oresult ServerErrorReset(void);
    int ServerErrorSQLPos(void) const;
    
	//new properties in 8.1.6
	bool GetAutoCommit(void);
	oresult SetAutoCommit(bool autocommit);
    void SetCacheMaximumSize(long maxsize) throw(OException);
    void SetCacheOptimalSize(long size) throw(OException);

    // function used by other classes to construct ODatabase objects (don't call this!)
    oresult OpenHelper(void *idisp, void *otheri);

	bool GetConnectionOK() const throw(OException);

private:
    long	m_numrows;
    char    *m_dbname;
    char    *m_dbconnect;
	char    *m_errtext;
	char	*m_version;
    
    // internal helper routines
    oresult Cleanup(void);
    oresult Copy(const ODatabase &other);
	 oresult GetUserPwd(const char *username, const char *pwd, char **retname);
};

class OEXPORT OValue
{
public:
	// construction & destruction
    OValue(void);
    OValue(int intval);       // allows OValue val = 3
    OValue(long longval);     // allows OValue val = 99L;
    OValue(double doubleval); // allows initialization with a double
    OValue(const char *tval); // allows initialization with a string
    OValue(const OValue &other); // copy constructor
	OValue(short *intval);		// Added for ARRAYINSERT
	OValue(int *intval);		// Added for ARRAYINSERT
	OValue(long *longval);		// Added for ARRAYINSERT
	OValue(double *doubleval);// Added for ARRAYINSERT
	OValue(char **tval);		// Added for ARRAYINSERT
	OValue(const OBlob &val) throw(OException);
	OValue(const OClob &val) throw(OException);
	OValue(const OBfile &val) throw(OException);
	OValue(const OObject &val) throw(OException);
	OValue(const ORef &val) throw(OException);
	OValue(const OCollection &val) throw(OException);
	OValue(const OMetaData &val) throw(OException);

	OValue(void* dp, int type); 
	
	~OValue(void);
   
	OValue &operator=(const OValue &other);
    int operator==(const OValue &other) const;
    int operator!=(const OValue &other) const;
    
    // setting the data & type
    oresult Clear(void);    // clear the values
    oresult SetValue(const OValue &val);
    oresult SetValue(const char *val); // sets string value (copies text)
    oresult SetValue(int val);     // sets to int value
    oresult SetValue(long val);
    oresult SetValue(double dval); // sets to double value 
    // oresult SetValue(const void *longval, long len);  // set to long value (not implemented)
	oresult SetValue(char **val);	// Added for ARRAYINSERT
	oresult SetValue(short *val);	// Added for ARRAYINSERT
	oresult SetValue(int *val);		// Added for ARRAYINSERT
	oresult SetValue(long *val);	// Added for ARRAYINSERT
	oresult SetValue(double *dval);	// Added for ARRAYINSERT
	oresult SetValue(const OBlob &val);
	oresult SetValue(const OClob &val);
	oresult SetValue(const OBfile &val);
	oresult SetValue(const OObject &val);
	oresult SetValue(const ORef &val);
	oresult SetValue(const OCollection &val);
	oresult SetValue(const OMetaData &val);

	oresult SetValue(void* raw, int type);
	
	oresult SetEmpty(void); //sets to "empty" object

    oboolean IsNull(void) const;  // returns TRUE if value is NULL (which includes uninitialized)
    long GetObjType(void) const;

    // getting data (overloaded cast operators)
    operator int() const;
    operator long() const;
    operator const char *() const; // returns 0 if instance isn't string (no allocation)
    operator double() const;
    
	operator OBlob () const throw(OException);
	operator OClob () const	throw(OException);
	operator OBfile () const throw(OException);
	operator OObject () const throw(OException);
	operator ORef () const throw(OException);
	operator OCollection () const throw(OException);
	operator OMetaData () const throw(OException);

    // conversions to implementation-specific representations (used by OField)
    oresult FromLocalType(void *localv, int type=0);

    // helper routine for implementation of other classes
    void *Internal(void) const;  
	int IsEqual(const OValue &other) const;

private:
    OOLEvar  *m_value;  // pointer to data representation 

    // helper routines
    oresult Copy(const OValue &other);
    oresult Cleanup(void);
};

// ----- ODynaset -----------------------------------------------

class OEXPORT ODynaset : public OOracleObject
{
public:
    // construction & destruction
    ODynaset(void);
	ODynaset(const ODynaset &other);
	ODynaset(const ODatabase &odb, const char *sql_statement,
					long options = ODYNASET_DEFAULT, OSnapshotID *snapID = NULL);  // construct & open
//	Added for custom dynaset
	ODynaset(const ODatabase &odb, const char *sql_statement,
					unsigned int slicesize, unsigned int perblock,
					unsigned int blocks, unsigned int fetchlimit,
					unsigned int fetchsize, long options = ODYNASET_DEFAULT,
					OSnapshotID *snapID = NULL);

	~ODynaset(void);

	// Open takes an SQL query as an argument.  This constructs a set of records which can
	//    then be accessed.
	oresult Open(const ODatabase &odb, const char *sql_statement,
						long options = ODYNASET_DEFAULT, OSnapshotID *snapID = NULL);  // creates a dynaset object
	//	Added for custom dynaset
	oresult Open(const ODatabase &odb, const char *sql_statement,
					unsigned int slicesize, unsigned int perblock,
					unsigned int blocks, unsigned int fetchlimit,
					unsigned int fetchsize, long options = ODYNASET_DEFAULT,
					OSnapshotID *snapID = NULL);

	 // The clone of a dynaset is looking at the original data, but has a different navigational
    //    position.  Useful if you want to navigate through a set of data without side-effects
    oresult Close(void);

    // getting other objects
    ODynaset Clone(void) const;  // clone a dynaset
	ODatabase GetDatabase(void) const;
    OFieldCollection GetFields(void) const;
    OSession GetSession(void) const;
	OField GetField(int index) const;
    OField GetField(const char *fieldname) const;
	int GetFieldOriginalNameIndex(const char *fieldname) const;
    OConnection GetConnection(void) const;
	ODynasetMark GetMark(void) const;  // bookmark at current position
    ODynasetMark GetLastModifiedMark(void) const;  // get bookmark at last modified record 
    
	 // overloaded operators
    ODynaset &operator=(const ODynaset &other);
    
	 // the sql statement of the dynaset
    oresult SetSQL(const char *sql_statement);  // sets new sql statement for the dynaset
    const char *GetSQL(void) const;  // gets sql statement
	oresult Refresh(void);  // refresh dynaset with current sql statement

    // navigation methods
	oresult MoveFirst(void);   // go to first record in the set
    oresult MoveLast(void);    // go to last record in the set.  Note that this requires that all the records
                            //    in the query be downloaded from the server.  Can be expensive.

	 // MovePrev and MoveNext take a "gopast" argument.  If it is TRUE we can navigate to an
	 //   invalid record before the first or after the last.  If "gopast" is FALSE, then we won't
    //   go before the first or after the last record.  
	 oresult MovePrev(oboolean gopast = TRUE);    // go to previous record
	 oresult MoveNext(oboolean gopast = TRUE);    // go to next record - the most frequently used
	//	extra move methods
	 oresult MovePrevN(long rows, oboolean gopast = TRUE);    // go to previous (n) record
	 oresult MoveNextN(long rows, oboolean gopast = TRUE);    // go to next (n) record
	 oresult MoveRel(long rows, oboolean gopast = TRUE);    // go to relative record (n)
	 oresult MoveTo(long rownum, oboolean gopast = TRUE);    // go to record (n)

	 oresult MoveToMark(const ODynasetMark &odmark);  // repositions to mark (see ODynasetMark class)
     oboolean IsEOF(void) const;  // true if at end of dynaset.  Set to TRUE when you have tried to go
                                //     beyond the end
	 oboolean IsBOF(void) const;  // true if at begin of dynaset
     oboolean IsValidRecord(void) const;  // true if current row is valid
    
	 // editing
     oboolean CanTransact(void) const;  // returns TRUE if transaction processing is enabled 
     oboolean CanUpdate(void) const;  // returns TRUE if this dynaset is updatable
	 int GetEditMode(void) const;  // returns current edit mode: ODYNASET_EDIT_*
	 oresult StartEdit(void);  // starts edit operation
	 oresult CancelEdit(void); // cancels edit operation
     oresult Update(void);     // finishes editing
	 oresult DuplicateRecord(void);  // copy the current record, adds a new record, sets field to copied values

	 // Find methods
	 oresult FindFirst(const char *sql);
	 oresult FindNext(const char *sql = 0);
	 oresult FindPrevious(const char *sql = 0);
	 oresult FindLast(const char *sql);
	 oboolean NoMatch(void);							// TRUE if find failed

	 // Tunable cache and fetch parameters
	 unsigned int GetCacheSliceSize (void);
	 unsigned int GetCacheSlicePerBlock (void);
	 unsigned int GetCacheBlocks (void);
	 unsigned int GetFetchLimit (void);
	 unsigned int GetFetchSize (void);
	 void SetCacheSliceSize (unsigned int slice);
	 void SetCacheSlicePerBlock (unsigned int perblock);
	 void SetCacheBlocks (unsigned int blocks);
	 void SetFetchLimit (unsigned int flimit);
	 void SetFetchSize (unsigned int fsize);
	 oboolean IsCacheChanged(void);	// TRUE if cache or fetch parameters changed

	 // paste to clipboard functions
	 oresult PlsqlOpen(const ODatabase &odb, const char *sqlst, 
							const char *CursorName, long options);
	 oresult PlsqlOpen(const ODatabase &odb, const char *sqlst, const char *CursorName, 
					unsigned int slicesize, unsigned int perblock,
					unsigned int blocks, unsigned int fetchlimit,
					unsigned int fetchsize, long options);

	 // others
	oboolean CanRefresh(void) const;  // our dynasets can always requery
    oboolean CanScroll(void) const;    // always scrollable
	oboolean CanMark(void) const;
    
    long GetOptions(void) const;

    // record manipulation
    oresult AddNewRecord(void);
	oresult DeleteRecord(void);
    
    // record access
	int GetFieldCount(void) const;  // returns # of fields in a record
    long GetRecordCount(void) const;  // (dangerous!) gets total number of records in dynaset
                         // this routine downloads the entire dynaset to the client   
	int GetFieldIndex(const char *fieldname) const;  // gets the index of a field by name

	 // all the Get and Set methods can refer to a column either by index (column position in the
    //    query) or by fieldname (the name of the column as specified in the query).  GetFieldIndex
	 //    can be used to find the index of columns - which is more efficient
    short GetFieldServerType(int index) const;
    short GetFieldServerType(const char *fieldname) const;
	short GetFieldPrecision(int index) const;
    short GetFieldPrecision(const char *fieldname) const;
    short GetFieldScale(int index) const;
	short GetFieldScale(const char *fieldname) const;
    oboolean IsFieldNullOK(int index) const;
    oboolean IsFieldNullOK(const char *fieldname) const;

    long GetFieldSize(int index) const;
    long GetFieldSize(const char *fieldname) const;
	long GetFieldServerSize(int index) const;
    long GetFieldServerSize(const char *fieldname) const;
	oboolean IsFieldTruncated(int index) const;
	oboolean IsFieldTruncated(const char *fieldname) const;
    
	 // get data from a field, either by name or index
    oresult GetFieldValue(int index, OValue *val) const;
	oresult GetFieldValue(const char *fieldname, OValue *val) const;
    oresult GetFieldValue(int index, int *val) const;
	oresult GetFieldValue(const char *fieldname, int *val) const;
    oresult GetFieldValue(int index, long *val) const;
	oresult GetFieldValue(const char *fieldname, long *val) const;
    oresult GetFieldValue(int index, double *val) const;
	oresult GetFieldValue(const char *fieldname, double *val) const;
	oresult GetFieldValue(int index, char *val, unsigned short maxlen) const;
	oresult GetFieldValue(const char *fieldname, char *val, unsigned short maxlen) const;
	oresult GetFieldValue(int index, char *val, unsigned short maxlen, unsigned short *outlen) const;
	oresult GetFieldValue(const char *fieldname, char *val, unsigned short maxlen, unsigned short *outlen) const;
	oresult GetFieldValue(int index, void __huge *longval, long len, long *readlen) const;
	oresult GetFieldValue(const char *fieldname, void __huge *longval, long len, long *readlen) const;

	oresult GetFieldValue(int index, OBlob  *val) const;
	oresult GetFieldValue(int index, OClob  *val) const;
	oresult GetFieldValue(int index, OBfile *val) const;
	oresult GetFieldValue(int index, ORef   *val) const;
	oresult GetFieldValue(int index, OObject *val) const;
	oresult GetFieldValue(int index, OCollection *val) const;

	oresult GetFieldValue(const char *fieldname, OBlob  *val) const;
	oresult GetFieldValue(const char *fieldname, OClob  *val) const;
	oresult GetFieldValue(const char *fieldname, OBfile *val) const;
	oresult GetFieldValue(const char *fieldname, ORef   *val) const;
	oresult GetFieldValue(const char *fieldname, OObject *val) const;
	oresult GetFieldValue(const char *fieldname, OCollection *val) const;

	oresult GetFieldChunk(int index, void *chunkp, long offset, unsigned short len) const;
	oresult GetFieldChunk(const char *fieldname, void *chunkp, long offset,
							unsigned short len) const;
	oresult GetFieldChunk(int index, void *chunkp, long offset,
							unsigned short len, unsigned short *bytesread) const;
	oresult GetFieldChunk(const char *fieldname, void *chunkp, long offset,
							unsigned short len, unsigned short *bytesread) const;

	// set data in a field, either by name or index
	oresult SetFieldValue(int index, const OValue &val);
	oresult SetFieldValue(const char *fieldname, const OValue &val);
	oresult SetFieldValue(int index, int val);
	oresult SetFieldValue(const char *fieldname, int val);
	oresult SetFieldValue(int index, long val);
	oresult SetFieldValue(const char *fieldname, long val);
	oresult SetFieldValue(int index, double val);
	oresult SetFieldValue(const char *fieldname, double val);
	oresult SetFieldValue(int index, const char *val);
	oresult SetFieldValue(const char *fieldname, const char *val);
	oresult SetFieldValue(int index, const char *val, int len);
	oresult SetFieldValue(const char *fieldname, const char *val, int len);
	oresult SetFieldValue(int index, const void __huge *longval, long len);
	oresult SetFieldValue(const char *fieldname, const void __huge *longval, long len);

	oresult SetFieldValue(int index, const OBlob  &val);
	oresult SetFieldValue(int index, const OClob  &val);
	oresult SetFieldValue(int index, const OBfile &val);
	oresult SetFieldValue(int index, const ORef   &val);
	oresult SetFieldValue(int index, const OObject &val);
	oresult SetFieldValue(int index, const OCollection &val);

	oresult SetFieldValue(const char *fieldname, const OBlob  &val);
	oresult SetFieldValue(const char *fieldname, const OClob  &val);
	oresult SetFieldValue(const char *fieldname, const OBfile &val);
	oresult SetFieldValue(const char *fieldname, const ORef   &val);
	oresult SetFieldValue(const char *fieldname, const OObject &val);
	oresult SetFieldValue(const char *fieldname, const OCollection &val);

	oresult AppendFieldChunk(int index, const void *chunkp, unsigned short len);
	oresult AppendFieldChunk(const char *fieldname, const void *chunkp, unsigned short len);

	// function used by other classes to construct ODynaset objects (don't call this!)
	oresult OpenHelper(void *idisp, void *otheri);

	oboolean IsFirst(void) const;
	oboolean IsLast(void) const;

    oboolean IsFieldUpdatable(int index) const;
    oboolean IsFieldUpdatable(const char *fieldname) const;
    oresult SetFieldUpdatable(int index, oboolean update) const;
    oresult SetFieldUpdatable(const char *fieldname, oboolean update) const;

	// Original Column name and update source
	oresult GetOrigFieldIndex(const char *fieldname) const;
	const char * GetFieldName(int index) const;
	const char * GetFieldOriginalName(int index) const;
 	oresult UpdateSource(int index, updsrcs updsrc, const char *source=NULL);
	oresult UpdateSource(const char *fieldname, updsrcs updsrc, const char *source=NULL);
	oresult CopyToClipboard(long NumOfRows);
	oresult CopyToClipboard(long NumOfRows, char colsep, char rowsep);

	oboolean get_CurrentRowModified( void) const;

	OSnapshotID GetSnapshot(void) const throw(OException);
	void SetSnapshot(OSnapshotID* snapID) const throw(OException);

	const char * GetXML(unsigned long startrow = 0, unsigned long maxrows = 0) throw(OException);
	void GetXMLToFile(const char *filename, unsigned long startrow = 0, unsigned long maxrows = 0) const throw(OException);
	
	const char * GetXMLCollID() throw(OException);
	void SetXMLCollID(const char *collid) const throw(OException);
	const char * GetXMLEncoding() const throw(OException);
	void SetXMLEncoding(const char *encoding) const throw(OException);
	bool GetXMLNullIndicator() const throw(OException);
	void SetXMLNullIndicator(bool usenullind) const throw(OException);
	const char * GetXMLRowsetTag() throw(OException);
	void SetXMLRowsetTag(const char *rowsettag) const throw(OException);
	const char * GetXMLRowID() throw(OException);
	void SetXMLRowID(const char *rowidtag) const throw(OException);
	const char * GetXMLRowTag() throw(OException); 
	void SetXMLRowTag(const char *rowtag) const throw(OException);
	bool GetXMLUpperCase() const throw(OException);
	void SetXMLUpperCase(bool upper) const throw(OException);
	bool GetXMLOmitEncodingTag() const throw(OException);
	void SetXMLOmitEncodingTag(bool omit) const throw(OException);
	const char * GetXMLEncodingTag() throw(OException);
	void SetXMLEncodingTag(const char *encodingtag) const throw(OException);
	


private:
	 char *m_sqlstmt;  // the sql statement that creates the dynaset
	 char *m_xmlbuf;
	 char *m_xmlcollid;
	 char *m_xmlencoding;
	 char *m_xmlrowsettag;
	 char *m_xmlrowid;
	 char *m_xmlrowtag;
	 char *m_xmldoctag;
// Bug 1361495
	 char *m_fieldname;

	 // internal helper routines
	 oresult Copy(const ODynaset &other);
	 oresult Cleanup(void);
};

// ----- ODynasetMark -----------------------------------------------

class OEXPORT ODynasetMark : public OOracleObject
{
public:
	 // construction & destruction
    ODynasetMark(void); 
	 ODynasetMark(const ODynasetMark &other);
	 ~ODynasetMark(void);
    
    // overloaded operators
	 ODynasetMark &operator=(const ODynasetMark &other);
	 int operator==(const ODynasetMark &other) const;
    int operator!=(const ODynasetMark &other) const;

    
    oresult Close(void);
    oboolean IsOpen(void) const;
    
    // function used by other classes to construct ODynasetMark objects (don't call this!)
    oresult OpenHelper(void *idisp, void *otheri);
    
    char *Internal(void) const;

private:
    char  *m_markdata;  // holds data for bookmark
    
    oresult Cleanup(void);
};

// ----- OField -----------------------------------------------

class OEXPORT OField : public OOracleObject
{
public:
    // construction & destruction
    OField(void);
    OField(const OField &other);
    ~OField(void);              
    
    oresult Close(void);
    
    // overloaded operators
    OField &operator=(const OField &other);
    
    // other objects
    ODynaset GetDynaset(void) const;
    
    // properties
    const char *GetName(void) const; // returns name of field (user doesn't need to free)
    long GetSize(void) const;  // returns length of field
    long GetServerSize(void) const;  // returns length of a field (in server)
    short GetServerType(void) const;
    short GetPrecision(void) const;
    short GetScale(void) const;
    oboolean IsNullOK(void) const;
	 oboolean IsTruncated(void) const;
	 
	 oboolean IsUpdatable(void) const;
	 oresult SetUpdatable(oboolean upd = TRUE);

    // field value
    oresult GetValue(OValue *val) const;  // returns value of field in val
    oresult GetValue(int *val) const;
    oresult GetValue(long *val) const;
    oresult GetValue(double *val) const;
    oresult GetValue(const char **val) const;
	oresult GetValue(const char **val, int *len) const;
    oresult GetValue(void __huge *longval, long len, long *readlen) const;
    oresult SetValue(const OValue &val);  // sets the field to new value
    oresult SetValue(int val);
    oresult SetValue(long val);
    oresult SetValue(double val);
    oresult SetValue(const char *val);
	oresult SetValue(const char *val, int len);
    oresult SetValue(const void __huge *longval, long len); 
	oresult GetValue(OBlob *val) const;
	oresult GetValue(OClob *val) const;
	oresult GetValue(OBfile *val) const;
	oresult GetValue(ORef *val) const;
	oresult GetValue(OObject *val) const;
	oresult GetValue(OCollection *val) const;

	oresult SetValue(const OBlob &val);
	oresult SetValue(const OClob &val);
	oresult SetValue(const OBfile &val);
	oresult SetValue(const ORef &val);
	oresult SetValue(const OObject &val);
	oresult SetValue(const OCollection &val);
    
    // for long fields
    oresult AppendChunk(const void *chunkp, unsigned short numbytes);
	oresult GetChunk(const char **chunkp, long offset, unsigned short numbytes) const;

	oresult GetChunk(const char **chunkp, long offset, unsigned short numbytes,
														unsigned short *bytesread) const;
	// Added for testing
	oresult GetChunk(const void **chunkp, long offset, unsigned short numbytes,
														unsigned short *bytesread) const;
	oresult AppendChunk(const char *chunkp, unsigned short numbytes);

    // getting data (overloaded cast operators)
    operator int() const;
    operator long() const;
    operator double() const;
    operator const char *() const;
	operator OBlob () const throw(OException);
	operator OClob () const	throw(OException);
	operator OBfile () const throw(OException);
	operator OObject () const throw(OException);
	operator ORef () const throw(OException);
	operator OCollection () const throw(OException);

    // function used by other classes to construct OField objects (don't call this!)
    oresult OpenHelper(void *idisp, void *otheri);

	// V2.1 dev - Original field name
	const char	*GetOriginalName (void) const;
	int			GetIndex () {return m_index;}
	oresult		UpdateSource(updsrcs updsrc, const char *source=NULL);

	bool GetXMLAsAttribute() const throw(OException);
	void SetXMLAsAttribute(bool asattribute) const throw(OException); 
	const char * GetXMLTagName() throw(OException);
	void SetXMLTagName(const char *tagname) const throw(OException);


private:
    short  m_index;
    char  *m_name;
// Bug 2445796
	char  *m_origname;
    char  *m_data;
	char  *m_xmltagname;

    // internal helper routines
    oresult Copy(const OField &other);
    oresult Cleanup(void);   
};

// ----- OFieldCollection -----------------------------------------------

class OEXPORT OFieldCollection : public OOracleCollection
{
public:
    OField GetField(int index) const;
	OField GetField(const char *name) const;
	OField GetFieldOname(const char *oname) const;
     OField GetFieldByOriginalName(const char *oname) const;
    // function used by other classes to construct OFieldCollection objects (don't call this!)
    oresult OpenHelper(void *idisp, void *otheri);  // finish the work of opening the set
};

// ----- OClient -----------------------------------------------

class OEXPORT OClient : public OOracleObject
{
public:
    // construction & destruction
    OClient(void);
    OClient(const OClient &other);
    ~OClient(void);
    
    // use OSession::GetClient to open a client object
    oresult Close(void);

    // Getting other objects
    OSessionCollection GetSessions(void) const;  // get set of sessions

    // overloaded operators
    OClient &operator=(const OClient &other);
    
    // get properties
    const char        *GetName(void) const;        // returns client name.
    
    // function used by other classes to construct OSession objects (don't call this!)
    oresult OpenHelper(void *idisp, void *otheri);

private:
    char *m_name;

    // internal helper routines
    oresult Copy(const OClient &other);
    oresult Cleanup(void);   
};

// ----- OConnection -----------------------------------------------

class OEXPORT OConnection : public OOracleObject
{
public:
    // construction & destruction
    OConnection(void);
    OConnection(const OConnection &other);
    ~OConnection(void);
    
    oresult Close(void);
    
    // getting other objects
    OSession GetSession(void) const;   

    // overloaded operators
    OConnection &operator=(const OConnection &other);
    
    // properties
    const char  *GetConnectString(void) const;  // returns connect string
    const char  *GetDatabaseName(void) const;  // returns database name for this connection

 	// transactions on database
	oresult  BeginTransaction(void);  // start a transaction
    oresult  Commit(oboolean startnew = FALSE);   // commit (may start new transaction)
    oresult  Rollback(oboolean startnew = FALSE); // rolls back transaction (may start new transaction)
    oresult  ResetTransaction(void);  // unconditionally rollback (no advisories)
    
    // function used by other classes to construct OConnection objects (don't call this!)
    oresult OpenHelper(void *idisp, void *otheri);

private:
    char *m_dbname;
    char *m_dbconnect;

    // internal helper routines
    oresult Copy(const OConnection &other);
    oresult Cleanup(void);   
};
 
// ----- OConnectionCollection -----------------------------------------------

class OEXPORT OConnectionCollection : public OOracleCollection
{
public:
    OConnection GetConnection(int index) const;

    // function used by other classes to construct OConnectionCollection objects (don't call this!)
    oresult OpenHelper(void *idisp, void *otheri);  // finish the work of opening the set
};

// ----- OAdvise -----------------------------------------------
 
class OEXPORT OAdvise : public OOracleObject
{
public:
    OAdvise(void);
    OAdvise(const OAdvise &other);
    OAdvise(const ODynaset &odyn);  // construct & open
    virtual ~OAdvise(void);
    
    virtual oresult Open(const ODynaset &odyn);
    virtual oresult Close(void);  // stop the advisory sink
    
    // getting other objects
    ODynaset GetDynaset(void) const;
    
    // overloaded operators
    OAdvise &operator=(const OAdvise &other);
    
    // called before an action takes place
    virtual oboolean ActionRequest(int movekind);  // returns TRUE if action is allowed, FALSE to cancel
    
    // routines called to to notify that action occured
    virtual void ActionNotify(int movekind); 
    
    // routine called on status change
    virtual void StatusChange(int statuskind);
    
private:
    ODynaset  m_oDyn;    // the dynaset we're getting notices from
    
    // private helper routines
    oresult Cleanup(void);
	 oresult Copy(const OAdvise &other);
};

// ----- OParameter -----------------------------------------------

class OEXPORT OParameter : public OOracleObject
{
public:
    OParameter(void);
    OParameter(const OParameter &other);
    ~OParameter(void);
    
    oresult Close(void);
    
    // overloaded operators
    OParameter &operator=(const OParameter &other);
    
    // parameter name
    const char *GetName(void) const;

    short GetServerType(void) const;    

    const char *GetLastErrorText(void ) ;
    
    // parameter value
	 oresult GetValue(OValue *val) const;
	 oresult GetValue(int *val) const;
	 oresult GetValue(long *val) const;
	 oresult GetValue(double *val) const;
	 oresult GetValue(const char **val) const;
	 oresult GetValue(const char **val, int *len) const;
	 oresult GetValue(OBlob *val) const;
	 oresult GetValue(OClob *val) const;
	 oresult GetValue(OBfile *val) const;
	 oresult GetValue(OObject *val) const;
	 oresult GetValue(ORef *val) const;
	 oresult GetValue(OCollection *val) const;

	 //multiple cursor return
	oresult GetValue(ODynaset *odyn) const;

    int	    Count (void);
    
    oresult Clear(void);
	oresult SetValue(const OValue &val);
	oresult SetValue(int val);
	oresult SetValue(long val);
	oresult SetValue(double val);
	oresult SetValue(const char *val);
	oresult SetValue(const char *val, int len);
	oresult SetValue(const OBlob &val);
	oresult SetValue(const OClob &val);
	oresult SetValue(const OBfile &val);
	oresult SetValue(const OObject &val);
	oresult SetValue(const ORef &val);
	oresult SetValue(const OCollection &val);

    oresult MinimumSize (long size);
    long    MinimumSize (void);

    // getting data (overloaded cast operators)
    operator int() const;
    operator long() const;
    operator double() const;
    operator const char *() const;
	operator OBlob () const throw(OException);
	operator OClob () const	throw(OException);
	operator OBfile () const throw(OException);
	operator OObject () const throw(OException);
	operator ORef () const throw(OException);
	operator OCollection () const throw(OException);

    // other status information
    int GetStatus(void) const;
    
    // enabling
    oresult AutoEnable(oboolean enable);

    // function used by other classes to construct OParameter objects (don't call this!)
    oresult OpenHelper(void *idisp, void *otheri);  // finish the work of opening the object

	// PL/SQL cursor return options
	void SetDynasetOption(long options);
	void SetDynasetCacheParams(long SliceSize, long perblock, long Blocks, long FetchLimit, long FetchSize) ;

private:
    char *m_name; 
    char *m_data;

    // internal helper routines
    oresult Copy(const OParameter &other);
	oresult Cleanup(void);
	oresult HelpGetValue(OOLEvar *tempv) const;
};

// ----- OParameterCollection -----------------------------------------------

class OEXPORT OParameterCollection : public OOracleObject
{
public:
    OParameterCollection(void);
    OParameterCollection(const OParameterCollection &other);
    ~OParameterCollection(void);
    
    oresult Close(void);
    
    // overloaded operators
    OParameterCollection &operator=(const OParameterCollection &other);
    
    // getting other objects
    OParameter GetParameter(int index) const;
    OParameter GetParameter(const char *pname) const;
	OParamArray GetParamArray(int index) const;
	OParamArray GetParamArray(const char *pname) const;

    // adding a new parameter
    OParameter Add(const char *name, int value, int iotype, int serverType, char* object_name = NULL);
    OParameter Add(const char *name, long value, int iotype, int serverType, char* object_name = NULL);
    OParameter Add(const char *name, double value, int iotype, int serverType, char* object_name = NULL);
    OParameter Add(const char *name, const char *value, int iotype, int serverType, char* object_name = NULL);  
	OParameter Add(const char *name, const char *value, int len, int iotype, int serverType, char* object_name = NULL);     
	OParameter Add(const char *name, const OBlob &value, int iotype, int servertype);
	OParameter Add(const char *name, const OClob &value, int iotype, int servertype);
	OParameter Add(const char *name, const OBfile &value, int iotype, int servertype);
	OParameter Add(const char *name, const OObject &value, int iotype, int servertype, char* object_name);
	OParameter Add(const char *name, const ORef &value, int iotype, int servertype, char* object_name = NULL);
	OParameter Add(const char *name, const OCollection &value, int iotype, int servertype, char* object_name);
    OParameter Add(const char *name, const OValue &value, int iotype, int serverType, char* object_name = NULL);     

	// array binding
	OParamArray AddTable(const char *name, int iotype, int serverType, int Dimension, int Size=0, char* object_name = NULL);
    
    // removing a parameter
    oresult Remove(int index);
    oresult Remove(const char *name);

    // getting # of parameters
	long GetCount(void) const;

    // function used by other classes to construct OParameterCollection objects (don't call this!)
    oresult OpenHelper(void *idisp, void *otheri);  // finish the work of opening the set

private:
    // internal helper routines
	int m_arraysize ;	// ARRAYINSERT
	oresult Copy(const OParameterCollection &other);
    oresult Cleanup(void);   
	OParameter AddHelp(const char *name, const OValue &val, int iotype, int serverType, char* object_name = NULL);
};


// ----- OSqlStmt -----------------------------------------------

class OEXPORT OSqlStmt : public OOracleObject
{
public:
    // construction & destruction
	 OSqlStmt(void);
	 OSqlStmt(const OSqlStmt &other);
	 OSqlStmt(const ODatabase &odb, const char *sql_statement,
					long options = OSQLSTMT_DEFAULT);  // construct & open
	 ~OSqlStmt(void);

	 // Open takes an SQL query as an argument.  This constructs a set of records which can
	 //    then be accessed.
	 oresult Open(const ODatabase &odb, const char *sql_statement,
						long options = OSQLSTMT_DEFAULT);  // creates a dynaset object
	 oresult Close(void);

    // getting other objects
	 ODatabase GetDatabase(void) const;
	 OSession GetSession(void) const;
	 OConnection GetConnection(void) const;

	 // overloaded operators
	 OSqlStmt &operator=(const OSqlStmt &other);
    
	 // the sql statement of the object
	 oresult SetSQL(const char *sql_statement);  // sets new sql statement
     const char *GetSQL(void) const;  // gets sql statement
	 oresult Refresh(void);  // refresh with current sql statement

	 // editing
	 oboolean CanTransact(void) const;  // returns TRUE if transaction processing is enabled
	 // others
	 oboolean CanRefresh(void) const;  // our sql statement can always requery

	 long GetOptions(void) const;
	 long GetRecordCount(void) const;

	 // function used by other classes to construct ODynaset objects (don't call this!)
	 oresult OpenHelper(void *idisp, void *otheri);
	 // NON-BLOCKING
	 oresult Cancel(void) throw(OException);
	 long	 NonBlockingState(void) throw(OException);
private:
	 char *m_sqlstmt;  // the sql statement that creates the dynaset

	 // internal helper routines
	 oresult Copy(const OSqlStmt &other);
	 oresult Cleanup(void);
};

// ----- OParamArray -----------------------------------------------

class OEXPORT OParamArray : public OOracleObject
{
public:
	 OParamArray(void);
	 OParamArray(const OParamArray &other);
	 ~OParamArray(void);
    
    oresult Close(void);
    
    // overloaded operators
	OParamArray &operator=(const OParamArray &other);
    
    // parameter name
    const char *GetName(void) const;

    short GetServerType(void) const;    

	const char *GetLastErrorText(void ) ;
    
    // parameter value
	oresult GetValue(OValue *val, int index) const;
	oresult GetValue(int *val, int index) const;
	oresult GetValue(long *val, int index) const;
	oresult GetValue(double *val, int index) const;
	oresult GetValue(const char **val, int index) const;
	oresult GetValue(const char **val, int *len, int index) const;

	oresult GetValue(OBlob *val, int index) const;
	oresult GetValue(OClob *val, int index) const;
	oresult GetValue(OBfile *val, int index) const;
	oresult GetValue(ORef *val, int index) const;
	oresult GetValue(OObject *val, int index) const;
	oresult GetValue(OCollection *val, int index) const;

	oresult Clear(void);
	oresult SetValue(const OValue &val, int index);
	oresult SetValue(int val, int index);
	oresult SetValue(long val, int index);
	oresult SetValue(double val, int index);
	oresult SetValue(const char *val, int index);
	oresult SetValue(const char *val, int len, int index);

	oresult SetValue(const OBlob &val, int index);
	oresult SetValue(const OClob &val, int index);
	oresult SetValue(const OBfile &val, int index);
	oresult SetValue(const ORef &val, int index);
	oresult SetValue(const OObject &val, int index);
	oresult SetValue(const OCollection &val, int index);

    oresult MinimumSize (long size);
    long    MinimumSize (void);

    // other status information
    int GetStatus(void) const;
	const char *GetLastError(void);		
	int GetArraySize(void);

	 // enabling
    oresult AutoEnable(oboolean enable);

    // function used by other classes to construct OParameter objects (don't call this!)
    oresult OpenHelper(void *idisp, void *otheri);  // finish the work of opening the object

private:
	char *m_name; 
    char **m_data;
    char *m_errtext;

    // internal helper routines
	oresult Copy(const OParamArray &other);
    oresult Cleanup(void);
	oresult HelpGetValue(OOLEvar *tempv, unsigned int index) const;
};


// ----- ODatabaseCollection -----------------------------------------------

class OEXPORT ODatabaseCollection : public OOracleCollection
{
public:
    ODatabase GetDatabase(int index) const;

    // function used by other classes to construct OConnectionCollection objects (don't call this!)
    oresult OpenHelper(void *idisp, void *otheri);  // finish the work of opening the set
};


// ----- OException ---------------------------------------------------------
//
//   Oracle Exception Class
//
//   This exception object is thrown by new methods (ver >= 8.1.6) when 
//   error conditions occur. 

class OEXPORT OException
{
public:
    // constructors & destructors
	OException(const OException &other);
	virtual ~OException(void);

	//these two constructors for internal use only
    OException(const char *methodName, const OOracleObject *other);
    OException(const char *methodName, int arg, const OOracleObject *other);
    
	OException &operator=(const OException &other);

	//public methods
	virtual const char *GetErrorText(void) const; //OIP error text when available
	virtual long  GetErrorNumber(void) const;     //Error Number
	virtual const char *GetFailedMethodName(void) const; //id of exception source
    
	//internal use
	virtual void SetFailedMethodName(const char *method);
protected:
	virtual void Copy(const OException &other);
	virtual void Cleanup(void);
	
private:
    char *m_methodname;
	char *m_oiperrtxt;
    long m_errnum;
	int m_arg;

	void HelpInit(void);
	void HelpInit(const char *methodName, int arg, const OOracleObject *other);	
};


// ----- OServer -----------------------------------------------

class OEXPORT OServer : public OOracleObject
{

public:
	
	// construction & destruction
	OServer(void) throw(OException); 
	OServer(const OServer& other) throw(OException);
    OServer(const char *server_alias) throw(OException);

	OServer(void *dp);

	~OServer(void); 

	OServer &operator=(const OServer &other);

	//methods
	void ChangePassword(const char *username, const char *current_pass, const char *new_pass) const throw(OException);
	
	void Open(const char *server_alias) throw(OException);
	void Close(void) throw(OException);

	//properties
	const char* GetName(void) throw(OException);
	OSession GetSession(void) const throw(OException);
	ODatabaseCollection GetDatabases(void) const throw(OException);
	
private:
	char *m_name;
	void HelpInit(void);
	oresult Copy(const OServer &other);
    oresult Cleanup(void);
};


// ----- OObject --------------------------------------------------

class OEXPORT OObject : public OOracleObject
{
public:
    // construction & destruction
	OObject(void) throw(OException);
	OObject(const OObject& other) throw(OException);
	OObject(ODatabase& odb, const char *type_name) throw(OException);
    virtual ~OObject(void);

	OObject(void *dp);
 
	OObject &operator=(const OObject &other);

	//methods
	void Open(ODatabase& odb, const char *type_name) throw(OException);
	virtual void Close(void) throw(OException);

	OObject Clone(void) const throw(OException);
	void Clone(OObject *clone) const throw(OException);

	OValue GetAttrValue(int index) const throw(OException);
	OValue GetAttrValue(const char *attrname) const throw(OException);

	void GetAttrValue(int index, OValue *value) const throw(OException);	
	void GetAttrValue(int index, int *val) const throw(OException);
	void GetAttrValue(int index, long *val) const throw(OException);
	void GetAttrValue(int index, double *val) const throw(OException);
	void GetAttrValue(int index, char *val, unsigned short maxlen) const throw(OException);
	void GetAttrValue(int index, OBlob *val) const throw(OException);
	void GetAttrValue(int index, OClob *val) const throw(OException);
	void GetAttrValue(int index, OBfile *val) const throw(OException);
	void GetAttrValue(int index, ORef *val) const throw(OException);
	void GetAttrValue(int index, OObject *val) const throw(OException);
	void GetAttrValue(int index, OCollection *val) const throw(OException);
	
	void GetAttrValue(const char *attrname, OValue *value) const throw(OException);
	void GetAttrValue(const char *attrname, int *val) const throw(OException);
	void GetAttrValue(const char *attrname, long *val) const throw(OException);
	void GetAttrValue(const char *attrname, double *val) const throw(OException);
	void GetAttrValue(const char *attrname, char *val, unsigned short maxlen) const throw(OException);
	void GetAttrValue(const char *attrname, OBlob *val) const throw(OException);
	void GetAttrValue(const char *attrname, OClob *val) const throw(OException);
	void GetAttrValue(const char *attrname, OBfile *val) const throw(OException);
	void GetAttrValue(const char *attrname, ORef *val) const throw(OException);
	void GetAttrValue(const char *attrname, OObject *val) const throw(OException);
	void GetAttrValue(const char *attrname, OCollection *val) const throw(OException);
	

	void SetAttrValue(int index, const OValue& val) const throw(OException);
	void SetAttrValue(int index, int val) const throw(OException);
	void SetAttrValue(int index, long val) const throw(OException);
	void SetAttrValue(int index, double val) const throw(OException);
	void SetAttrValue(int index, const char *val) const throw(OException);
	void SetAttrValue(int index, const OBlob  &val) const throw(OException);
	void SetAttrValue(int index, const OClob  &val) const throw(OException);
	void SetAttrValue(int index, const OBfile &val) const throw(OException);
	void SetAttrValue(int index, const ORef   &val) const throw(OException);
	void SetAttrValue(int index, const OObject &val) const throw(OException);
	void SetAttrValue(int index, const OCollection &val) const throw(OException);

	void SetAttrValue(const char *attrname, const OValue& val) const throw(OException);
	void SetAttrValue(const char *attrname, int val) const throw(OException);
	void SetAttrValue(const char *attrname, long val) const throw(OException);
	void SetAttrValue(const char *attrname, double val) const throw(OException);
	void SetAttrValue(const char *attrname, const char *val) const throw(OException);
	void SetAttrValue(const char *attrname, const OBlob  &val) const throw(OException);
	void SetAttrValue(const char *attrname, const OClob  &val) const throw(OException);
	void SetAttrValue(const char *attrname, const OBfile &val) const throw(OException);
	void SetAttrValue(const char *attrname, const ORef   &val) const throw(OException);
	void SetAttrValue(const char *attrname, const OObject &val) const throw(OException);
	void SetAttrValue(const char *attrname, const OCollection &val) const throw(OException);

	const char* GetAttrName(int index) throw(OException);

	int GetAttrType(int index) const throw(OException);
	int GetAttrType(const char *attrname) const throw(OException);
	
    // properties
	int GetCount(void) const throw(OException);
	bool IsNull(void) const throw(OException);
	const char* GetTypeName(void) throw(OException);
	const char* GetVersion(void) throw(OException);	
	virtual oboolean IsOpen(void) const;

protected:
	virtual oresult Copy(const OObject &other);
    virtual oresult Cleanup(void);

private:
	char *m_typename;
	char *m_attrname;
	char *m_version;
	void HelpInit(void);	
};


// ----- ORef --------------------------------------------------
//
// Note: ORef derives from OObject
//
 
class OEXPORT ORef : public OObject
{
public:
    // construction & destruction
	ORef(void) throw(OException); 
	ORef(const ORef& other) throw(OException);
	ORef(ODatabase& odb, const char *type_name, const char *tablename) throw(OException);
    ~ORef(void); 

	ORef(void *dp);

    ORef &operator=(const ORef &other);

	//methods
	void Open(ODatabase& odb, const char *type_name, const char *tablename) throw(OException);
	void Close(void) throw(OException);

	void CancelEdit(void) const throw(OException);
	ORef Clone(void) const throw(OException);
	void Clone(ORef *clone) const throw(OException);
	void Delete(void) const throw(OException);
	void Edit(void) const throw(OException);
	void Refresh(void) const throw(OException);
	void Update(void) const throw(OException);

	//properties
	void SetEditOption(int option) const throw(OException);
	int GetEditOption(void) const throw(OException);
	bool IsRefNull(void) const throw(OException);
	void SetPinOption(int option) const throw(OException);
	int GetPinOption(void) const throw(OException);
	const char* GetTableName(void) throw(OException);
	oboolean IsOpen(void) const;
	
	void *DInternal() const;

private:
	OOracleObject m_oracleobject;
	char *m_tablename;
	void HelpInit(void);
	oresult Copy(const ORef &other);
    oresult Cleanup(void);
};


// ----- OCollection --------------------------------------------------

class OEXPORT OCollection : public OOracleObject
{
public:
    // construction & destruction
	OCollection(void) throw(OException);
	OCollection(ODatabase& odb, const char *type_name) throw(OException);
    ~OCollection(void);

	OCollection(void *dp);

	OCollection &operator=(const OCollection &other);

    // methods
	void Open(ODatabase& odb, const char *type_name) throw(OException);
	void Close(void) throw(OException);

	void Append(OValue& var) const throw(OException); 	
    OCollection Clone(void) const throw(OException);
	void Clone(OCollection *clone) const throw(OException);
	void CreateIterator(void) const throw(OException);
    void Delete(int index) const throw(OException);  
	void DeleteIterator(void) const throw(OException);
    bool Exist(int index) const throw(OException); 
	OValue GetCurrIterValue(void) const throw(OException);
	void GetCurrIterValue(OValue *val) const throw(OException);
	void GetCurrIterValue(int *val) const throw(OException);
	void GetCurrIterValue(long *val) const throw(OException);
	void GetCurrIterValue(double *val) const throw(OException);
	void GetCurrIterValue(char *val, unsigned short maxlen) const throw(OException);
	void GetCurrIterValue(OObject *val) const throw(OException);
	void GetCurrIterValue(ORef *val) const throw(OException);

	OValue GetValue(int index) const throw(OException);
	void GetValue(int index, OValue *val) const throw(OException);

	void GetValue(int index, int *val) const throw(OException);
	void GetValue(int index, long *val) const throw(OException);
	void GetValue(int index, double *val) const throw(OException);
	void GetValue(int index, char *val, unsigned short maxlen) const throw(OException);
	void GetValue(int index, OObject *val) const throw(OException);
	void GetValue(int index, ORef *val) const throw(OException);

	void InitIterator(void) const throw(OException);
	void IterNext(void) const throw(OException);
	void IterPrev(void) const throw(OException);
	void SetValue(int index, const OValue& val) const throw(OException);

	void SetValue(int index, int val) const throw(OException);
	void SetValue(int index, long val) const throw(OException);
	void SetValue(int index, double val) const throw(OException);
	void SetValue(int index, const char *val) const throw(OException);
	void SetValue(int index, const OObject& val) const throw(OException);
	void SetValue(int index, const ORef& val) const throw(OException);

    void Trim(int size) const throw(OException);

	//properties
	bool BOC(void) const throw(OException);
	int GetElementType(void) const throw(OException);
	bool EOC(void) const throw(OException);
	bool IsLocator(void) const throw(OException);
	bool IsNull(void) const throw(OException);
	int GetMaxSize(void) const throw(OException);
	int GetSize(void) const throw(OException);
	int GetTableSize(void) const throw(OException);
	int GetType(void) const throw(OException);
	
private:
	void HelpInit(void);
	oresult Copy(const OCollection &other);
    oresult Cleanup(void);
};


// ----- OMetaData --------------------------------------------------

class OEXPORT OMetaData : public OOracleObject
{

public:
    // construction & destruction
	OMetaData(void) throw(OException);
	OMetaData(const OMetaData& other) throw(OException);
	OMetaData(ODatabase& db, const char *SchemaObjectName) throw(OException);
	~OMetaData(void);

	OMetaData(void *dp);

	OMetaData &operator=(const OMetaData &other);

	//methods
	void Open(ODatabase& db, const char *SchemaObjectName) throw(OException);
	void Close(void) throw(OException);

	OMDAttribute GetAttribute(int index) const throw(OException);
	OMDAttribute GetAttribute(const char *index) const throw(OException);
	
	OValue GetAttrValue(int index) const throw(OException);
	OValue GetAttrValue(const char *attrname) const throw(OException);
	void GetAttrValue(int index, OValue *value) const throw(OException);
	void GetAttrValue(const char *attrname, OValue *value) const throw(OException);


    // properties
    int GetType(void) const throw(OException);  
	int GetCount(void) const throw(OException);

	const char* GetAttrName(int index) throw(OException);
	
private:
	char *m_name;
	void HelpInit(void);
	oresult Copy(const OMetaData &other);
    oresult Cleanup(void);
};


// ----- OMDAttribute --------------------------------------------------

class OEXPORT OMDAttribute : public OOracleObject
{

public:
    // construction & destruction
	OMDAttribute(void) throw(OException);
	OMDAttribute(const OMDAttribute& other) throw(OException);
	~OMDAttribute(void);

	OMDAttribute(void *dp);

	OMDAttribute &operator=(const OMDAttribute &other);

	void Close(void) throw(OException);

    // properties
    const char* GetName(void) throw(OException);  
	bool IsMDObject(void) const throw(OException);
	OValue GetValue(void) const throw(OException);
	void GetValue(OValue *value) const throw(OException); 

private:
	char *m_name;	
	void HelpInit(void);
	oresult Copy(const OMDAttribute &other);
    oresult Cleanup(void);
};


// ----- OBlob --------------------------------------------------

class OEXPORT OBlob : public OOracleObject
{

public:
	
	// construction & destruction
	OBlob(void) throw(OException);
    ~OBlob(void);

	OBlob(void *dp);
	OBlob(ODatabase& odb, bool usecaching = false) throw(OException);

	OBlob &operator=(const OBlob &other);

	void Close(void) throw(OException);

	//methods
	void Append(const OBlob& other) const throw(OException);
	OBlob Clone(void) const throw(OException);
	void Clone(OBlob* dest) const throw(OException);

	bool Compare(const OBlob& src_lob, unsigned long amount, unsigned long dst_offset = 1,
		unsigned long src_offset = 1) const throw(OException);

	void CopyFromBlob(const OBlob& src_lob, unsigned long amount, unsigned long dst_offset =1,
		unsigned long src_offset = 1) const throw(OException);

	void CopyFromBfile(const OBfile& src_bfile, unsigned long amount, unsigned long dst_offset = 1,
		unsigned long src_offset = 1) const throw(OException);

	void CopyFromFile(const char *filename, unsigned long amount = 0, unsigned long chunksize = 0, 
		unsigned long offset = 1) const throw(OException);

	void CopyToFile(const char *filename, unsigned long amount, unsigned long chunksize,
		unsigned long offset = 1) const throw(OException);

	void DisableBuffering(void) const throw(OException);
	void EnableBuffering(void) const throw(OException);
	void Erase(unsigned long amount, unsigned long offset) const throw(OException);
	void FlushBuffer(void) const throw(OException);

	unsigned long MatchPos(const char *pattern, unsigned long bufsize, unsigned long offset,
		unsigned long nth) const throw(OException);

	unsigned long Read(unsigned char *buffer, unsigned long buffersize,
		unsigned long offset = 0) const throw(OException);
	
	unsigned long Read(short *status, unsigned char *buffer, unsigned long buffersize,
		unsigned long offset = 0) const throw(OException);
	
	void Trim(unsigned long newlen) const throw(OException);

	unsigned long Write(short *status, unsigned char *buffer, unsigned long buffersize,
		unsigned char piecetype = OLOB_ONE_PIECE,
		unsigned long offset = 0) const throw(OException);

	unsigned long Write(unsigned char *buffer, unsigned long buffersize,
		unsigned char piecetype = OLOB_ONE_PIECE,
		unsigned long offset = 0) const throw(OException);
	
	void EnableStreaming(unsigned long totalamount, unsigned long initialoffset = 1) const throw(OException);
	void DisableStreaming(void) const throw(OException);
	unsigned long GetOptimumChunkSize(void) const throw(OException);

	//properties
	bool IsNull(void) const throw(OException);	
	unsigned long GetSize(void) const throw(OException);  
	
private:
	void HelpInit(void);
	oresult Copy(const OBlob &other);
    oresult Cleanup(void);
};


// ----- OClob --------------------------------------------------

class OEXPORT OClob : public OOracleObject
{

public:	
	// construction & destruction 
	OClob(void) throw(OException);
    ~OClob(void);
	
	OClob(void *dp);
	OClob(ODatabase& odb, bool usecaching = false) throw(OException);

	OClob &operator=(const OClob &other);

	void Close(void) throw(OException);

	//methods
	void Append(const OClob& pOther) const throw(OException);
	OClob Clone(void) const throw(OException);
	void Clone(OClob* dest) const throw(OException);

	bool Compare(const OClob& src_lob, unsigned long amount, unsigned long dst_offset = 1, 
		unsigned long src_offset = 1) const throw(OException);

	void CopyFromClob(const OClob& src_lob, unsigned long amount, 
		unsigned long dst_offset = 1, unsigned long src_offset = 1) const throw(OException);

	void CopyFromBfile(const OBfile& src_bfile, unsigned long amount,
		unsigned long dst_offset = 1, unsigned long src_offset = 1) const throw(OException);
	
	void CopyFromFile(const char *filename, unsigned long amount = 0, unsigned long chunksize = 0, 
		unsigned long offset = 1) const throw(OException);
	
	void CopyToFile(const char *filename, unsigned long amount, unsigned long chunksize,
		unsigned long offset = 1) const throw(OException);
	
	void DisableBuffering(void) const throw(OException);
	void EnableBuffering(void) const throw(OException);
	void Erase(unsigned long amount, unsigned long offset) const throw(OException);
	void FlushBuffer(void) const throw(OException);
	
	unsigned long MatchPos(const char *pattern, unsigned long bufsize, unsigned long offset,
		unsigned long nth) const throw(OException);
	
	unsigned long Read(unsigned char *buffer, unsigned long buffersize, unsigned long offset = 0) const throw(OException);
	
	unsigned long Read(short *status, unsigned char *buffer, unsigned long buffersize,
		unsigned long offset = 0) const throw(OException);

	void Trim(unsigned long newlen) const throw(OException);
	
	unsigned long Write(short *status, unsigned char *buffer, unsigned long buffersize,
		unsigned char piecetype = OLOB_ONE_PIECE, unsigned long offset = 0) const throw(OException);
	
	unsigned long Write(unsigned char *buffer, unsigned long buffersize, 
		unsigned char piecetype = OLOB_ONE_PIECE, unsigned long offset = 0) const throw(OException);
	
	void EnableStreaming(unsigned long totalamount, unsigned long initialoffset = 1) const throw(OException);
	void DisableStreaming(void) const throw(OException);
	unsigned long GetOptimumChunkSize(void) const throw(OException);

	//properties
	bool IsNull(void) const throw(OException);   	   
	unsigned long GetSize(void) const throw(OException);
	
private:
	void HelpInit(void);
	oresult Copy(const OClob &other);
    oresult Cleanup(void);
};


// ----- OBfile --------------------------------------------------

class OEXPORT OBfile : public OOracleObject
{

public:

	// construction & destruction
    OBfile(void) throw(OException);
	OBfile(const OBfile& other) throw(OException);
    ~OBfile(void); 

	OBfile(void *dp);
	
	OBfile &operator=(const OBfile &other);

	void Close(void) throw(OException);

	//methods
	OBfile Clone(void) const throw(OException);
	void Clone(OBfile* dest) const throw(OException);
	void CloseBfile(void) const throw(OException);
	void CloseAll(void) const throw(OException);

	bool Compare(const OBfile& src_lob, unsigned long amount, unsigned long dst_offset = 1,
		unsigned long src_offset = 1) const throw(OException);

	void CopyToFile(const char *filename, unsigned long amount, unsigned long chunksize,
		unsigned long offset = 1) const throw(OException);

	unsigned long MatchPos(const char *pattern, unsigned long bufsize, unsigned long offset,
		unsigned long nth) const throw(OException);
	
	void OpenBfile(void) const throw(OException);

	unsigned long Read(unsigned char *buffer, unsigned long buffersize,
		unsigned long offset = 0) const throw(OException);
	
	unsigned long Read(short *status, unsigned char *buffer, unsigned long buffersize,
		unsigned long offset = 0) const throw(OException);

	void EnableStreaming(unsigned long totalamount, unsigned long initialoffset = 1) const throw(OException);
	void DisableStreaming(void) const throw(OException);
	
	//properties
	const char* GetDirectoryName(void) throw(OException);
	void SetDirectoryName(const char *dirname) const throw(OException);
	const char* GetFileName(void) throw(OException);
	void SetFileName(const char *filename) const throw(OException);
	void GetFileName(const char **dirname, const char **filename) throw(OException);
	void SetFileName(const char *dirname, const char *filename) throw(OException);
	bool Exists(void) const throw(OException);
	bool IsNull(void) const throw(OException); 
	bool IsBfileOpen(void) const throw(OException);	
	unsigned long GetSize(void) const throw(OException);    	    

private:
	char *m_filename;
	char *m_dirname;
	void HelpInit(void);
	oresult Copy(const OBfile &other);
    oresult Cleanup(void);
};


// ----- OSnapshotID --------------------------------------------------

class OEXPORT OSnapshotID : public OOracleObject
{

public:
    // construction & destruction
	OSnapshotID(void) throw(OException);
	OSnapshotID(const OSnapshotID& other) throw(OException);
	OSnapshotID(ODynaset& dyn) throw(OException);
	~OSnapshotID(void);

	OSnapshotID(void *dp); 

	OSnapshotID &operator=(const OSnapshotID &other);

	//methods
	void Open(ODynaset& dyn) throw(OException);
	void Close(void) throw(OException);

private:
	void HelpInit(void);
	oresult Copy(const OSnapshotID &other);
    oresult Cleanup(void);
};


// ----- OAQ --------------------------------------------------

class OEXPORT OAQ : public OOracleObject
{

public:
	// constructors
	OAQ(void) throw(OException);
	OAQ(const OAQ& other) throw(OException);
	OAQ(const ODatabase& odb, const char *queuename) throw(OException);
	~OAQ(void);

	OAQ(void *dp);
	
	OAQ &operator=(const OAQ &other);

    //methods
	void Open(const ODatabase& odb, const char *queuename) throw(OException);
	void Close(void) throw(OException);
	const char* Dequeue(void) throw(OException);
    const char* Enqueue(void) throw(OException);

	// properties
	void SetVisibility(int val) const throw(OException);
	int GetVisibility(void) const throw(OException);

	void SetRelMsgID(const char *msgid) const throw(OException);
	const char* GetRelMsgID(void) throw(OException);

	void SetConsumer(const char *name) const throw(OException);
	const char* GetConsumer(void) throw(OException);

	void SetDequeueMode(int mode) const throw(OException);
	int GetDequeueMode(void) const throw(OException);

	void SetNavigation(int position) const throw(OException);
	int GetNavigation(void) const throw(OException);

	void SetWait(int duration) const throw(OException);
	int GetWait(void) const throw(OException);

	void SetCorrelate(const char *identifier) const throw(OException);
	const char* GetCorrelate(void) throw(OException);

	void SetDequeueMsgID(const char *msg_id) const throw(OException);
	const char* GetDequeueMsgID(void) throw(OException);
	
private:
	char *m_initenqmsgid;
	char *m_initdeqmsgid;
	char *m_relmsgid;
	char *m_consumername;
	char *m_correlateid;
	char *m_deqmsgid;
	void HelpInit(void);
	oresult Copy(const OAQ &other);
    oresult Cleanup(void);
};


// ----- OAQMsg -----------------------------------------------

class OEXPORT OAQMsg : public OOracleObject
{

public:
    // constructors
	OAQMsg(void) throw(OException);
	OAQMsg(const OAQMsg& other) throw(OException);
	OAQMsg(OAQ& aq, int msgtype = OTYPE_RAW, const char *tname = NULL, const char *schema= NULL) throw(OException);
	~OAQMsg(void);

	OAQMsg(void *dp);
	
	OAQMsg &operator=(const OAQMsg &other);

	//methods
	void Open(OAQ& aq, int msgtype=OTYPE_RAW, const char *tname = NULL, const char *schema = NULL) throw(OException);
	void Close(void) throw(OException);
    
	// properties
	long GetValue(char *val, unsigned short maxlen) const throw(OException);       	
	void GetValue(OValue *val) const throw(OException);
	void GetValue(OObject *val) const throw(OException);		
	
	void SetValue(OValue& val) const throw(OException);

	int GetDelay(void) const throw(OException);
	void SetDelay(int delay) const throw(OException);

	const char* GetExceptionQueue(void) throw(OException);
	void SetExceptionQueue(const char *qname) const throw(OException);

	int GetPriority(void) const throw(OException);
	void SetPriority(int priority) const throw(OException);

	const char* GetCorrelation(void) throw(OException);
	void SetCorrelation(const char *corr) const throw(OException);

    int GetExpiration(void) const throw(OException);
	void SetExpiration(int expiretime) const throw(OException);
	
private:
	char *m_qname;
	char *m_corr;
	char *m_value;
	void HelpInit(void);
	oresult Copy(const OAQMsg &other);
    oresult Cleanup(void);
};


// ----- OAQAgent -----------------------------------------------

class OEXPORT OAQAgent : public OOracleObject
{

public:
    // constructors
	OAQAgent(void) throw(OException);
	OAQAgent(const OAQAgent& other) throw(OException);
	OAQAgent(OAQMsg &msg, const char *name, const char *addr) throw(OException);
	~OAQAgent(void);

	OAQAgent(void *dp);
	
	OAQAgent &operator=(const OAQAgent &other);

	//methods
	void Open(OAQMsg &msg, const char *name, const char *addr) throw(OException);
	void Close(void) throw(OException);
	// properties
    const char* GetName(void) throw(OException);
	void SetName(const char *name) const throw(OException);
	const char* GetAddress(void) throw(OException);
	void SetAddress(const char *address) const throw(OException);

private:
	char *m_name;
	char *m_address;
	void HelpInit(void);
	oresult Copy(const OAQAgent &other);
    oresult Cleanup(void);
};


#ifdef ORAANSI
typedef char FAR* BSTRA;
typedef BSTRA * LPBSTRA;

typedef struct FARSTRUCT tagVARIANTA  VARIANTA;
typedef struct FARSTRUCT tagVARIANTA FAR * LPVARIANTA;
typedef struct FARSTRUCT tagVARIANTA VARIANTARGA;
typedef struct FARSTRUCT tagVARIANTA FAR * LPVARIANTARGA;

struct FARSTRUCT tagVARIANTA{
	VARTYPE vt;
	unsigned short wReserved1;
	unsigned short wReserved2;
	unsigned short wReserved3;
	union {
	  unsigned char bVal;              /* VT_UI1                */
	  short    iVal;              /* VT_I2                */
	  long     lVal;              /* VT_I4                */
	  float    fltVal;            /* VT_R4                */
	  double       dblVal;            /* VT_R8                */
	  VARIANT_BOOL boolvar;              /* VT_BOOL              */
	  SCODE    scode;             /* VT_ERROR             */
	  CY       cyVal;             /* VT_CY                */
	  DATE     date;              /* VT_DATE              */
	  BSTRA    bstrVal;           /* VT_BSTR              */
	  IUnknown     FAR* punkVal;      /* VT_UNKNOWN           */
	  IDispatch  FAR* pdispVal;     /* VT_DISPATCH          */
	  SAFEARRAY   FAR* parray;       /* VT_ARRAY|*           */

	  unsigned char    FAR* pbVal;        /* VT_BYREF|VT_UI1       */
	  short    FAR* piVal;        /* VT_BYREF|VT_I2       */
	  long     FAR* plVal;        /* VT_BYREF|VT_I4       */
	  float    FAR* pfltVal;      /* VT_BYREF|VT_R4       */
	  double       FAR* pdblVal;      /* VT_BYREF|VT_R8       */
	  VARIANT_BOOL FAR* pbool;        /* VT_BYREF|VT_BOOL     */
	  SCODE    FAR* pscode;       /* VT_BYREF|VT_ERROR    */
	  CY       FAR* pcyVal;       /* VT_BYREF|VT_CY       */
	  DATE     FAR* pdate;        /* VT_BYREF|VT_DATE     */
	  BSTRA    FAR* pbstrVal;     /* VT_BYREF|VT_BSTR     */
	  IUnknown   FAR* FAR* ppunkVal;  /* VT_BYREF|VT_UNKNOWN  */
	  IDispatch FAR* FAR* ppdispVal; /* VT_BYREF|VT_DISPATCH */
	  SAFEARRAY FAR* FAR* pparray;   /* VT_BYREF|VT_ARRAY|*  */
	  VARIANT    FAR* pvarVal;      /* VT_BYREF|VT_VARIANT  */

	  void     FAR* byref;        /* Generic ByRef        */
	}
#if defined(NONAMELESSUNION) || (defined(_MAC) && !defined(__cplusplus) && !defined(_CID_MS20))
	u
#endif
	;
};


STDAPI_(BSTRA) SysAllocStringA(const char FAR*);
#define SysAllocStringLenA	SysAllocStringByteLen
inline void SysFreeStringA(BSTRA bstr) { SysFreeString((BSTR)bstr); }
STDAPI_(int)   SysReAllocStringLenA(BSTRA FAR*, const char FAR*, unsigned int);
HRESULT ConvertVariantToA(LPVARIANTA);
HRESULT ConvertVariantToW(LPVARIANT);

STDAPI  VariantChangeTypeA(	VARIANTARGA FAR* pvargDest,VARIANTARGA FAR* pvarSrc,unsigned short wFlags,VARTYPE vt);


#endif // WRAPPER
#endif // ORACL_ORACLE

