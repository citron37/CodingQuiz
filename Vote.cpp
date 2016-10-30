#include <iostream>
#include <fstream>

using namespace std;

const int MAX_FILE_NAME		= 100;
const int MAX_NAME_LEN		= 50;

//////////////////////////////////////////////////////////////////////////
//	후보자 Class
class CCandidate
{
public:
	CCandidate()
	{
		Init();
	}
	~CCandidate()
	{
	}

public:
	char	m_strName[MAX_NAME_LEN + 1];
	int		m_nVote;

public:
	void	Init ()
	{
		memset (m_strName, 0x00, sizeof (char));
		m_nVote	= 0;
	}

	//////////////////////////////////////////////////////////////////////////
	//	Operator
	CCandidate&	operator=(const CCandidate& kCandidate)
	{
		memcpy (m_strName, kCandidate.m_strName, sizeof(char) * MAX_NAME_LEN);
		m_nVote	= kCandidate.m_nVote;

		return *this;
	}

	bool operator==(const CCandidate& kCandidate)
	{
		//	후보자의 이름이 같은지 여부
		return strncmp(m_strName, kCandidate.m_strName, MAX_NAME_LEN) == 0 ? true : false;
	}

	bool operator<(const CCandidate& kCandidate)
	{
		//	득표율 내림차순 정렬을 위해
		if (m_nVote > kCandidate.m_nVote)
		{
			return true;
		}
		else if (m_nVote < kCandidate.m_nVote)
		{	
			return false;
		}

		//	이름 오름차순 정렬을 위해
		if (strncmp (m_strName, kCandidate.m_strName, MAX_NAME_LEN) < 0)
		{
			return true;
		}

		return false;
	}

	bool operator>(const CCandidate& kCandidate)
	{
		//	득표율 내림차순 정렬을 위해
		if (m_nVote < kCandidate.m_nVote)
		{
			return true;
		}
		else if (m_nVote > kCandidate.m_nVote)
		{
			return false;
		}

		//	이름 오름차순 정렬을 위해
		if (strncmp (m_strName, kCandidate.m_strName, MAX_NAME_LEN) > 0)
		{
			return true;
		}

		return false;
	}
};

//////////////////////////////////////////////////////////////////////////
//	Quick Sort
//	후보자 리스트를 정렬
//	득표율 : 내림차순
//	이름 : 오름차순
void QuickSortByVoteName (CCandidate* pkArrCandidate, int nLeft, int nRight)
{
	int			nBegin	= nLeft;
	int			nEnd	= nRight;

	CCandidate	kTemp;
	CCandidate	kPivot	= pkArrCandidate[(nLeft + nRight) / 2];

	while (nBegin <= nEnd)
	{
		while (pkArrCandidate[nBegin] < kPivot)
		{
			nBegin++;
		}

		while (pkArrCandidate[nEnd] > kPivot)
		{
			nEnd--;
		}

		if (nBegin <= nEnd)
		{
			kTemp					= pkArrCandidate[nBegin];
			pkArrCandidate[nBegin]	= pkArrCandidate[nEnd];
			pkArrCandidate[nEnd]	= kTemp;

			nBegin++;
			nEnd--;
		}
	};

	if (nLeft < nEnd)
	{
		QuickSortByVoteName(pkArrCandidate, nLeft, nEnd);
	}

	if (nBegin < nRight)
	{
		QuickSortByVoteName(pkArrCandidate, nBegin, nRight);
	}
}

//////////////////////////////////////////////////////////////////////////
//	Merge by name
//	후보자 리스트가 이름으로 정렬되었다는 가정하에 득표율 계산
void MergeByName (CCandidate* pkArrCandidate, int nSize)
{
	if (nSize == 1)
	{
		return;
	}

	CCandidate*	pkCandidate	= &pkArrCandidate[0];
	for (int nIndex = 1 ; nIndex < nSize ; nIndex++)
	{
		//	이름이 같으면 득표율을 올려주고, 중복된 후보자 정보 삭제
		if (*pkCandidate == pkArrCandidate[nIndex])
		{
			pkCandidate->m_nVote++;
			pkArrCandidate[nIndex].Init();
		}

		//	이름이 다르면 다른 후보자의 득표율 계산
		else
		{
			pkCandidate	= &pkArrCandidate[nIndex];
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//	Main
int main (void)
{
	//////////////////////////////////////////////////////////////////////////
	//	In/out file name
	//	in
	cout << "Enter input file name : ";

	char strInputFile[MAX_FILE_NAME + 1];
	memset (strInputFile, 0x00, sizeof(char));
	cin.getline(strInputFile, MAX_FILE_NAME);

	ifstream kInFile(strInputFile);
	if (kInFile.is_open() == false)
	{
		return 0;
	}

	//	out
	cout << "Enter output file name : ";

	char strOutputFile[MAX_FILE_NAME + 1];
	memset (strOutputFile, 0x00, sizeof(char));
	cin.getline(strOutputFile, MAX_FILE_NAME);

	ofstream kOutFile(strOutputFile);
	if (kOutFile.is_open() == false)
	{
		kInFile.close();
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	//	Candidate count for memory allocation
	int		nCandidateSize	= 0;
	char	strName[MAX_NAME_LEN + 1];
	
 	while (kInFile.eof() == false)
	{
		kInFile.getline(strName, MAX_NAME_LEN);
		if (strlen(strName) == 0)
		{
			continue;
		}

		nCandidateSize++;
	}

	//	Memory Allocation
	CCandidate*	pkArrCandidate	= new CCandidate[nCandidateSize];

	//////////////////////////////////////////////////////////////////////////
	//	Read Candidate name
	kInFile.clear();
	kInFile.seekg(0);
	nCandidateSize	= 0;

	while (kInFile.eof() == false)
	{
		kInFile.getline(strName, MAX_NAME_LEN);
		if (strlen(strName) == 0)
		{
			continue;
		}

		memcpy (pkArrCandidate[nCandidateSize].m_strName, strName, sizeof(char) * MAX_NAME_LEN);
		pkArrCandidate[nCandidateSize].m_nVote	= 1;

		nCandidateSize++;
	}

	kInFile.close();
	
	//////////////////////////////////////////////////////////////////////////
	//	Sort by name
	QuickSortByVoteName (pkArrCandidate, 0, nCandidateSize - 1);

	//////////////////////////////////////////////////////////////////////////
	//	Merge by name
	MergeByName (pkArrCandidate, nCandidateSize);

	//////////////////////////////////////////////////////////////////////////
	//	sort by vote & name
	QuickSortByVoteName (pkArrCandidate, 0, nCandidateSize - 1);

	//////////////////////////////////////////////////////////////////////////
	//	Write result
	int	nMaxVote	= pkArrCandidate[0].m_nVote;
	kOutFile << pkArrCandidate[0].m_strName << endl;

	for (int nIndex = 1 ; nIndex < nCandidateSize ; nIndex++)
	{
		if (pkArrCandidate[nIndex].m_nVote == nMaxVote)
		{
			kOutFile << pkArrCandidate[nIndex].m_strName << endl;
		}
		else
		{
			break;
		}
	}

	kOutFile.close();

	//////////////////////////////////////////////////////////////////////////
	//	Delete buffer
	delete [] pkArrCandidate;

	return 0;
};