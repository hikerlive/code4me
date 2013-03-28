#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "xstring.h"

extern char * strtolower(char * str)
{
	unsigned int	i;
	unsigned char	ch;

	if (!str) return NULL;
	for (i=0; (ch=str[i]); i++) {
		if ((isupper(ch))) str[i]=ch+('a'-'A');
	}
	return str;
}

extern unsigned char xtoi(unsigned char ch)
{
	unsigned char retval;

	retval=tolower(ch);
	if (retval < 'A') retval -= ('0'-0);
	else retval -= ('a' - 0xa);
	return retval;
}

extern char * str_strip_affix(char * str, char const * affix)
{
	unsigned int i, j, n;
	int		match;

	if (!str) return NULL;
	if (!affix) return str;
	for (i=0; str[i]; i++) {
		match=0;
		for (n=0; affix[n]; n++) {
			if (str[i]==affix[n]) {
				match=1;
				break;
			}
		}
		if (!match) break;
	}
	for (j=strlen(str)-1; j>=i; j--) {
		match=0;
		for (n=0; affix[n]; n++) {
			if (str[j]==affix[n]) {
				match=1;
				break;
			}
		}
		if (!match) break;
	}
	if (i>j) {
		str[0]='\0';
	} else {
		memmove(str,str+i,j-i+1);
		str[j-i+1]='\0';
	}
	return str;
}

extern char * hexstrdup(unsigned char const * src)
{
	char	* dest;
	int	len;

	if (!src) return NULL;
	if (!(dest=strdup((char const *)src))) return NULL;
	len=hexstrtoraw(src,dest,strlen(dest)+1);
	dest[len]='\0';
	return dest;
}

extern unsigned int hexstrtoraw(unsigned char const * src, char * data, unsigned int datalen)
{
	unsigned char	ch;
	unsigned int	i, j;

	for (i=0, j=0; j < datalen; i++) {
		ch=src[i];
		if (!ch) break;
		if (ch=='\\') {
			i++;
			ch=src[i];
			if (!ch) {
				break;
			} else if (ch=='\\') {
				data[j++]=ch;
			} else if (ch=='x') {
				if (isxdigit(src[i+1])) { 
					if (isxdigit(src[i+2])) {
						data[j++]=xtoi(src[i+1]) * 0x10 + xtoi(src[i+2]);
						i+=2;
					} else {
						data[j++]=xtoi(src[i+1]);
						i++;
					}
				} else {
					data[j++]=ch;
				}
			} else if (ch=='n') {
				data[j++]='\n';
			} else if (ch=='r') {
				data[j++]='\r';
			} else if (ch=='a') {
				data[j++]='\a';
			} else if (ch=='t') {
				data[j++]='\t';
			} else if (ch=='b') {
				data[j++]='\b';
			} else if (ch=='f') {
				data[j++]='\f';
			} else if (ch=='v') {
				data[j++]='\v';
			} else {
				data[j++]=ch;
			}
		} else {
			data[j++]=ch;
			continue;
		}
	}
	return j;
}

#define SPLIT_STRING_INIT_COUNT		32
#define	SPLIT_STRING_INCREASEMENT	32
extern char * * strtoarray(char const * str, char const * delim, int * argc)
{
	int	i ,n, index_size, count;
	int	in_delim, match;
	char	* temp, * result;
	int		* pindex;
	char	* pd;
	char	const * ps;
	char	* realloc_tmp;

	if (!str || !delim) return NULL;

	temp=(char *)malloc(strlen(str)+1);
	if (!temp) return NULL;

	n = SPLIT_STRING_INIT_COUNT;
	pindex=(int *)malloc(sizeof(char *) * n);
	if (!pindex) {
		free(temp);
		return NULL;
	}

	count=0;
	in_delim=1;
	ps=str;
	pd=temp;
	pindex[0]=0;
	while (*ps!='\0') {
		match=0;
		for (i=0; delim[i]!='\0'; i++) {
			if ( *ps == delim[i]) {
				match=1;
				if (!in_delim) {
					*pd = '\0';
					pd++;
					count++;
					in_delim=1;
				}
				break;
			}
		}
		if (!match) {
			if (in_delim) {
				if (count +1 >=n) {
					n += SPLIT_STRING_INCREASEMENT;
					if (!(realloc_tmp=(char *)realloc(pindex,n * sizeof(char *)))) {
						free(pindex);
						free(temp);
						return NULL;
					}
					pindex=(int *)realloc_tmp;
				}
				pindex[count]= pd-temp;
				in_delim = 0;
			}
			*pd = * ps;
			pd++;
		}
		ps++;
	}
	if (!in_delim) {
		*pd='\0';
		pd++;
		count++;
	}
	index_size=(1+count) * sizeof(char *);
	result=(char *)malloc(pd-temp+index_size);
	if (!result) {
		free(temp);
		free(pindex);
		return NULL;
	}
	memcpy(result+index_size,temp,pd-temp);
	for (i=0; i< count; i++) {
		pindex[i]+=(int)result+index_size;
	}
	pindex[count]=0;
	memcpy(result,pindex,index_size);
	free(temp);
	free(pindex);
	if (argc) *argc=count;
	return (char **) result;
}

extern char * * strtoargv(char const * str, int * argc)
{
	int		n, index_size, count;
	char	* temp;
	int		i, j;
	int		* pindex;
	char	* result;
	char	* realloc_tmp;

	if (!str) return NULL;
	temp=(char *)malloc(strlen(str)+1);
	if (!temp) return NULL;
	n = SPLIT_STRING_INIT_COUNT;
	pindex=(int *)malloc(n * sizeof (char *));
	if (!pindex) return NULL;

	i=j=0;
	count=0;
	while (str[i]) {
		while (str[i]==' ' || str[i]=='\t') i++;
		if (!str[i]) break;
		if (count + 1 >=n ) {
			n += SPLIT_STRING_INCREASEMENT;
			if (!(realloc_tmp=(char *)realloc(pindex,n * sizeof(char *)))) {
				free(pindex);
				free(temp);
				return NULL;
			}
			pindex=(int *)realloc_tmp;
		}
		pindex[count]=j;
		count++;
		if (str[i]=='"') {
			i++;
			while (str[i]) {
				if (str[i]=='\\') {
					i++;
					if (!str[i]) break;
				} else if (str[i]=='"') {
					i++;
					break;
				}
				temp[j++]=str[i++];
			}
		} else {
			while (str[i] && str[i] != ' ' && str[i] != '\t') {
				temp[j++]=str[i++];
			}
		}
		temp[j++]='\0';
	}
	index_size= 1+ count * sizeof(char *);
	result=(char *)malloc(j+index_size);
	if (!result) {
		free(temp);
		free(pindex);
		return NULL;
	}
	memcpy(result+index_size,temp,j);
	for (i=0; i< count; i++) {
		pindex[i] +=(int)result+index_size;
	}
	pindex[count]=0;
	memcpy(result,pindex,index_size);
	free(temp);
	free(pindex);
	if (argc) *argc=count;
	return (char * *)result;
}

#define COMBINE_STRING_INIT_LEN		1024
#define COMBINE_STRING_INCREASEMENT	1024
extern char * arraytostr(char * * array, char const * delim,int count)
{
	int		i,n;
	char	* result;
	char	* realloc_tmp;
	int	need_delim;

	if (!delim || !array) return NULL;

	n=COMBINE_STRING_INIT_LEN;
	result=(char *)malloc(n);
	if (!result) return NULL;
	result[0]='\0';

	need_delim=0;
	for (i=0; ; i++) {
		if (count) {
			if (i >= count) break;
			else if (!array[i]) continue;
		} else if (!array[i]) {
			break;
		}
		if (strlen(result)+strlen(array[i])+strlen(delim)>=(unsigned int)n) {
			n+=COMBINE_STRING_INCREASEMENT;
			if (!(realloc_tmp=(char *)realloc(result,n))) {
				free(result);
				return NULL;
			}
			result=realloc_tmp;
		}
		if (need_delim) {
			strcat(result,delim);
		}
		strcat(result,array[i]);
		need_delim=1;
	}
	if (!(realloc_tmp=(char *)realloc(result,strlen(result)+1))) {
		return result;
	}
	result=realloc_tmp;
	return result;
}


XStringToken::XStringToken()
{
	return;
}

XStringToken::XStringToken(LPCSTR szString)
{
	SetString(szString);
	return;
}

XStringToken::~XStringToken()
{
	return;
}

void XStringToken::SetString(LPCSTR szString)
{
	if (!szString) return;
	str = szString;	
}

BOOL XStringToken::ToArgv()
{
	char * * argv;
	int i, argc;
	string	val;

	array.clear();
	if (str.empty()) return TRUE;
	argv = strtoargv(str.c_str(), &argc);
	if (!argv) return FALSE;
	for (i=0; i < argc; i++) {
		val = argv[i];
		array.push_back(val);
	}
	free(argv);
	return TRUE;
}

BOOL XStringToken::ToArray(char const * delim)
{
	char * * arr;
	int i, count;
	string val;

	array.clear();
	if (str.empty()) return TRUE;
	arr = strtoarray(str.c_str(), delim, &count);
	if (!arr) return FALSE;
	for (i=0; i < count; i++) {
		val = arr[i];
		array.push_back(val);
	}
	free(arr);
	return TRUE;
}

