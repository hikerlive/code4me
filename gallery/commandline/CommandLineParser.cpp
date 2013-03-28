



#include "CommandLineParser.h"

void CCommandLineParser::SetDefaultCommand(ICmdLineParam* obj)
{
	m_defaultCommand = obj;
}
void CCommandLineParser::PutEmptyCommand(std::string argument,ICmdLineParam* obj)
{
	m_mapEmptyCommand[argument] = obj;
	
}
void CCommandLineParser::PutValueCommand(std::string argument,ICmdLineParam* obj)
{
	m_mapValueCommand[argument] = obj;
}
void CCommandLineParser::SetErrorCommand(ICmdLineParam* obj)
{
	m_errorCommand = obj;
}


bool CCommandLineParser::ParseArguments(int argn,char* argv[])
{
	bool bAllOK = true;
	int i = 1; // First paramter is discarded becouse it's the execution program path.
	
	while(i < argn )
	{
		
		std::string argument = argv[i];

		if(DeleteMinus(argument))
		{
			// Check if this argument requires a value.
			std::map<std::string,ICmdLineParam*>::iterator it = m_mapValueCommand.find(argument);

			if(it != m_mapValueCommand.end())
			{
				if(argv[i + 1] == NULL)
				{
					bAllOK &= m_errorCommand->Parse(NO_VALUE_PARAM,argument);
					return false;
				}
				std::string paso = argv[i + 1];
				if(DeleteMinus(paso))
				{
					bAllOK &= m_errorCommand->Parse(NO_VALUE_PARAM,argument);
					return false;
				}
				else
				{
					bAllOK &= it->second->Parse(argument,paso);
					i++;
				}
			}
			else
			{
				it = m_mapEmptyCommand.find(argument);
				if(it != m_mapEmptyCommand.end())
				{
					bAllOK &= it->second->Parse(argument,"");
				}
				else
				{

					// Try to split a ":"
					std::string::size_type position = argument.find_first_of(":");
					if(position != std::string::npos)
					{
						std::string command = argument.substr(0,position);
						std::string value = argument.substr(position + 1);
						std::map<std::string,ICmdLineParam*>::iterator it = m_mapValueCommand.find(command);
						if(it != m_mapValueCommand.end())
						{
							bAllOK &= it->second->Parse(command,value);
						}
						else
						{
							bAllOK &= m_errorCommand->Parse(UNKNOWN_PARAM,command);
							return false;
						}
					}
					else
					{
						bAllOK &= m_errorCommand->Parse(UNKNOWN_PARAM,argument);
						return false;
					}
				}

			}	
		}
		else
		{
			// Es valor no parámetro.
			if(m_defaultCommand != NULL)
			{
				bAllOK &= m_defaultCommand->Parse("",argument);
			}
			else
			{
				return false;
			}
		}
		i++;
	}
	
	std::string error = m_defaultCommand->GetError();
	if(error != "")
	{
		m_errorCommand->Parse(GENERIC_ERROR,error);
		return false;
	}


	std::map<std::string,ICmdLineParam*>::iterator it = m_mapValueCommand.begin();

	while(it != m_mapValueCommand.end())
	{
		error = it->second->GetError();
		if(error != "")
		{
			m_errorCommand->Parse(GENERIC_ERROR,error);
			return false;
		}
		it++;
	}

	std::map<std::string,ICmdLineParam*>::iterator it2 = m_mapEmptyCommand.begin();

	while(it2 != m_mapEmptyCommand.end())
	{
		error = it2->second->GetError();
		if(error != "")
		{
			m_errorCommand->Parse(GENERIC_ERROR,error);
			return false;
		}
		it2++;
	}


	return bAllOK;// Devuelve false si ha habido error.
}

bool CCommandLineParser::DeleteMinus(std::string& argument)
{
	



	switch(argument[0])
	{
	case '/':
		argument = &(argument.c_str()[1]);
		return true;
	case '-':
		if(argument[1] == '-')
		{
			argument = &(argument.c_str()[2]);
		}
		else
		{
			argument = &(argument.c_str()[1]);
		}
		return true;
	}


	return false;
}

