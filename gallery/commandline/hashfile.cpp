// hashfile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
void HandleError(char *s);

#include "CommandLineParser.h"

#define APP_USAGE "Usage: hashfile [-alg md5|sha1] [-out hex] file\nDefault: -alg md5 -out hex\n"

class CHashType : public ICmdLineParam
{
	enum types{MD5,SHA1};

	types m_type;
	string m_strError;
public:
	CHashType()
	{
		m_strError = "";
		m_type = MD5;
	}

	bool Parse(string argument,string value)
	{
		if(argument == "alg")
		{
			if(value == "md5")
			{
				m_type = MD5;
			}
			else if(value == "sha1")
			{
				m_type = SHA1;
			}
			else
			{
				m_strError = "Incorrect hash algorithm. Correct values(case sensitive): md5, sha1. Default md5.";
				return false;
			}
		}
		return true;
	}

	string GetError()
	{
		return m_strError;
	}


	void CalcHash(string strFileName, unsigned char*& outbuffer,DWORD& rdwDataLen)
	{
		unsigned int dLenBuffer = 1024;
		unsigned int dDataLen = 0;

		//unsigned char* inbuffer = NULL;


		FILE* hFile = fopen(strFileName.c_str(),"rb");

		outbuffer = new unsigned char[dLenBuffer];



		GUID g;
		CoCreateGuid(&g);

		char szContName[100];
		_snprintf(szContName,100,"%u-%u-%u-%u",g.Data1,g.Data2,g.Data3,g.Data4);
		

		HCRYPTPROV hProv;
	

		if(CryptAcquireContext(&hProv,
			szContName,
			MS_DEF_PROV,
			PROV_RSA_FULL,
			CRYPT_NEWKEYSET))
		{
			
		}
		else
		{
			HandleError("CryptAcquireContext1 fail");
		}

		ALG_ID alg_id = CALG_MD5;

		switch(m_type)
		{
		case SHA1:
			alg_id = CALG_SHA1;
			break;
		case MD5:
			alg_id = CALG_MD5;
			break;
		};

		HCRYPTHASH hHash;
		if(CryptCreateHash(
			hProv,
			alg_id,
			0,
			0,
			&hHash))
		{
		}
		else
		{
			HandleError("CryptCreateHash fail");
		}



		while(!feof(hFile))
		{
			dDataLen = fread(outbuffer,1,dLenBuffer,hFile);
			if(CryptHashData(
					hHash,
					outbuffer,
					dDataLen,
					0
				))
				{
				}
				else
				{
					HandleError("CryptHashData fail");
				}
		}



		rdwDataLen = 1024;
		if(CryptGetHashParam(
				  hHash,
				  HP_HASHVAL,
				  outbuffer,
				  &rdwDataLen,
				  0
				))
		{
		}
		else
		{
			HandleError("CryptHashData fail");
		}
		fclose(hFile);

		CryptReleaseContext(hProv,0);

		CryptAcquireContext(&hProv,
		szContName,
		MS_DEF_PROV,
		PROV_RSA_FULL,
		CRYPT_DELETEKEYSET);

		


	}
};

class COutputType : public ICmdLineParam
{
	enum types{HEX};
	types m_type;
	string m_strError;
public:
	COutputType()
	{
		m_type = HEX;
	}
	bool Parse(string argument,string value)
	{
		if(argument == "out")
		{
			if(value == "hex")
			{
				m_type = HEX;
			}
			else
			{
				m_strError = "Incorrect output type codification. Correct values(case sensitive): hex. Default hex.";
				return false;
			}

		}
		return true;
	}

	string GetError()
	{
		return m_strError;
	}

	void OutputHash(unsigned char* inbuffer,DWORD dwDataLen)
	{
		char cad[3];

		switch(m_type)
		{
		case HEX:
			for(int i = 0 ; i < dwDataLen ; i++)
			{
				sprintf(cad,"%02x",inbuffer[i]);
				cout << cad;
			}
			break;
		}
	}
};
class CFileHash : public ICmdLineParam
{
	string m_filename;
	string m_strError;
public:
	CFileHash()
	{
		m_filename = "";
		m_strError = "";
	}
	string GetFileName()
	{
		return m_filename;
	}
	bool Parse(string argument,string value)
	{
		if(argument == "")
		{
			m_filename = value;

			if(access(m_filename.c_str(), 04) != 0)
			{
				m_strError = "Inaccesible input file: " + value;
				return false;
			}
		
		}
		return true;
	}
	string GetError()
	{
		if(m_filename == "")
		{
			return "Must specify a file name.";
		}
		
		
		return m_strError;
	}
};

class CError : public ICmdLineParam
{
	
public:
	bool m_displayHelp;
	CError()
	{
		m_displayHelp = false;
	}
	
	bool Parse(string argument,string value)
	{	
		if(argument == "h")
		{
			m_displayHelp = true;
			return false;
			
		}
		if(argument == GENERIC_ERROR)
		{
			cout << value << endl << endl;
			cout << APP_USAGE;
			return false;
		}
		if(argument == UNKNOWN_PARAM)
		{
			cout << "Invalid argument '"<< value << "'." << endl << endl;
			cout << APP_USAGE;
			return false;
		}
		if(argument == NO_VALUE_PARAM)
		{
			cout << "Empty argument '"<< value << "' must specify one value." << endl << endl;
			cout << APP_USAGE;
			return false;
		}
		
		return true;
	}


	string GetError()
	{
		if(m_displayHelp)
		{
			return APP_USAGE;	
		}
		else
		{
			return "";
		}
	}
};



int main(int argc, char* argv[])
{
	CCommandLineParser parser;
	COutputType output;
	CHashType hash;
	CError err;
	CFileHash file;


	parser.PutValueCommand("out",&output);
	parser.PutValueCommand("alg",&hash);
	parser.SetDefaultCommand(&file);
	parser.PutEmptyCommand("h",&err);
	parser.PutEmptyCommand("help",&err);
	parser.SetErrorCommand(&err);

	if(parser.ParseArguments(argc,argv))
	{

		unsigned char* inbuffer = NULL;
		DWORD dwDataLen = 1024;

		
		hash.CalcHash(file.GetFileName().c_str(),inbuffer,dwDataLen);
		output.OutputHash(inbuffer,dwDataLen);

		delete[] inbuffer;

	}


	return 0;
}


void HandleError(char *s)
{
	char message[1000];
	int length = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,0,GetLastError(),0,message,1000,NULL);

    fprintf(stderr,"An error occurred in running the program. \n");
    fprintf(stderr,"%s\n",s);
    fprintf(stderr, "Error number %x: %s.\n", GetLastError(),message);
    fprintf(stderr, "Program terminating. \n");
    exit(1);
} // End of HandleError
