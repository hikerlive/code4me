#include <stdio.h>
#include <atlrx.h>

int main(int argc, char* argv[])
{
	CAtlRegExp<CAtlRECharTraits> regex;
	REParseError error = regex.Parse(L"");
	if (error != REPARSE_ERROR_OK)
	{
		printf("regular express error:\n");
		return -1;
	}

	CAtlREMatchContext<CAtlRECharTraits> context;
	if (!regex.Match(L"gonggai@channelsoft.com", &context))
	{
		printf("regular match error:\n");
		return -1;
	}
	return 0;
}