
#include <string>
#include <map>
#include <utility> // make_pair
#include <stdio.h>
//////////////////////////////////////////////////////////////////////////
//http://partow.net/programming/hashfunctions/index.html

#define HashFun1 RSHash
#define HashFun2 JSHash
// hash 算法一
unsigned int RSHash(const char* str, unsigned int length)
{
	unsigned int b    = 378551;
	unsigned int a    = 63689;
	unsigned int hash = 0;
	unsigned int i    = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash = hash * a + (*str);
		a    = a * b;
	}

	return hash;
}

// hash 算法二
unsigned int JSHash(const char* str, unsigned int length)
{
	unsigned int hash = 1315423911;
	unsigned int i    = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash ^= ((hash << 5) + (*str) + (hash >> 2));
	}

	return hash;
}

//////////////////////////////////////////////////////////////////////////

// 移除字符串末尾的换行符
void RemoveLastInvalidValue(std::string &str, std::string &strValue);


int main(int agrc,char* agrv)
{
	std::map<unsigned int,std::string >  hashMap;

	FILE *pFile = fopen("./words.txt","rb");
	if (!pFile)
		return -1;
	std::string str;
	int nCount = 0;
	char szline[1000];  
	while(!feof(pFile))
	{
		fgets(szline,1000,pFile);  
		str = szline;
		printf("%s",str.c_str());

		std::string strValue;
		RemoveLastInvalidValue(str,strValue);
		if (strValue.empty())
			continue;
		unsigned int Hash = HashFun1(strValue.c_str(),strValue.size());
		std::pair< std::map< unsigned int,std::string >::iterator,bool > ret = hashMap.insert(std::make_pair(Hash,strValue));
		if (!ret.second)
		{
			if (str.find(hashMap[Hash]))
			{
				printf("数值不同，使用第二份hash算法\n");
				Hash = HashFun2(strValue.c_str(),strValue.size());
				std::pair< std::map< unsigned int,std::string >::iterator,bool > ret = hashMap.insert(std::make_pair(Hash,strValue));
				// 两份hash算法同时碰撞的概率太小了，这里就不判断是否插入失败了
			}
		}
		nCount ++;
	}
	fclose(pFile);
	pFile = NULL;

	printf("\n///////////// \n");
	printf("\n共读取有效数据%d行  HashMap大小%d\n",nCount,hashMap.size());

//////////////////////////////////////////////////////////////////////////
///////////// 查找测试

	std::string strFind = "北京";
	unsigned int findHash = HashFun1(strFind.c_str(),strFind.size());

	std::map<unsigned int,std::string>::iterator it;
	it = hashMap.find(findHash);
	if (it!=hashMap.end())
	{
		printf("找到了 %s\n",strFind.c_str());
	}
	else
	{
		printf("未查找到数据\n");
	}
///////////// 当数据查找不到的时候，就得用hash2继续查找一次，重新遍历一次，增加了非常大的计算量
//// 提供优化方法的思路：将冲突的hasn保存到表二，冲突表的大小肯定非常小，可以保证效率
	findHash = HashFun2(strFind.c_str(),strFind.size());
	it = hashMap.find(findHash);
	if (it!=hashMap.end())
	{
		printf("找到了 %s\n",strFind.c_str());
	}
	else
	{
		printf("未查找到数据\n");
	}

	return 0;
} 

void RemoveLastInvalidValue(std::string &str, std::string &strValue)
{
	if (str.size() <1)  // 不合理的数据
		return;
	int nRemove = 0;
	for (int i=str.size()-1;i>=0;i--)
	{
		if (str[i] == '\r' || str[i]=='\n' || str[i]=='\0')
		{
			nRemove ++;
		}
		else
			break;
	}
	if(nRemove>=str.size())
		nRemove = 0;
	strValue = str.substr(0,str.size() - nRemove);
}


