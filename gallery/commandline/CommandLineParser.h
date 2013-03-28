#ifndef __CCommandLineParser_H
#define __CCommandLineParser_H
#pragma warning(disable: 4786)	// disable stl warning

#include <map>
#include <string>



#define UNKNOWN_PARAM "UNKNOWN_PARAM"
#define NO_VALUE_PARAM "PARAM_NO_VALUE"
#define GENERIC_ERROR "GENERIC_ERROR"

/**
	Interface that must be implemented by the objects that requires access to the
	command line arguments.
	@see CCommandLineParser
*/
class ICmdLineParam
{
public:
	/**
		 Method called by the CCommandLineParser to pass the arguments and the values.
		 The parser calls this method to indicate the name of the arguments and/or
		 the values only when applicable. The '-' or '/' of the arguments are eliminated
		 before reach this method.
		 @return false only if the parser must stop, on a serious error.
				true when the argument is OK or is malformed, in this second case 
				the function GetError must return a description, see below.
	*/
	virtual bool Parse(std::string argument,std::string value) = 0;

	/** Method called by the CCommandLineParser to retrieve the error description.
		// if an fail in command line occurs, this method must return a description of the error.
		// Ex: "Inaccesible input file", "invalid type of algorithm",..,etc.
		@retrun "" to indicate that no error was produced.
	*/
	virtual std::string GetError() = 0;

};

/** This class can be used to parse command line arguments.
	Usage:
	Once created CCommandLineParser it must be called the methods SetDefaultEntry, SetEmptyEntry, SetValueEntry and SetErrorEntry
	to indicate the objects that require the command line arguments.
	<br>
	CCommandLineParser parser;<br>
	<br>
	parser.SetDefaultEntry(filesObj);<br>
	parser.SetEmptyEntry(dataObj);<br>
	parser.SetValueEntry(valObj);<br>
	parser.SetErrorEntry(helpObj);<br>
	<br>
	Each of this objects must implement the interface ICmdLineParam.<br>
	<br>
	<br>
	A default entry represents the files or other arguments without a '-', '--' or '/'<br>
	Example: copy file1 file2<br>
	<br>
	An empty entry represents a flag in the command line.<br>
	Example: ls -l<br>
	A value entry represents a pair of arguments.<br>
	Example: netstat -p TCP<br>
	The error entry is the object that retrives any error in the proccess of parse arguments.<br>
	<br>
	@see ICmdLineParam
*/
class CCommandLineParser
{
public:
	CCommandLineParser(){m_defaultCommand = NULL;}
	/**
		Object that handle the default arguments. Only one can be specified.
	*/
	void SetDefaultCommand(ICmdLineParam* e);
	/**
		Objects that handle the empty arguments. Can be called how many times as necesary.
		@param para name of the argument that this ICmdLineParam expect.
		@e ICmdLineParam
	*/
	void PutEmptyCommand(std::string argument,ICmdLineParam* e);
	/**
		Entry that handle the values arguments. Can be called how many times as necesary.
		@param para name of the argument that this ICmdLineParam expect.
		@e ICmdLineParam
	*/
	void PutValueCommand(std::string argument,ICmdLineParam* e);
	/**
		Entry that handle the errors of the CCommandLineParser. Only one can be specified.
	*/
	void SetErrorCommand(ICmdLineParam* e);

	
	/**
		Inits the parse process.
		@param argn number of arguments passed to the application.
		@param argv array of string with the arguments passed to the application.
	*/
	bool ParseArguments(int argn,char* argv[]);
private:
	ICmdLineParam* m_defaultCommand;
	ICmdLineParam* m_errorCommand;
	std::map<std::string,ICmdLineParam*> m_mapValueCommand;
	std::map<std::string,ICmdLineParam*> m_mapEmptyCommand;

	/**
		Deletes the '-', '--', '/' of the argument.
		@return <b>true</b> if the argument is not a value.
	*/
	bool DeleteMinus(std::string& param);
};


#endif __CCommandLineParser_H