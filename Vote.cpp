#include <iostream>
#include <fstream>

using namespace std;

const int MAX_FILE_NAME		= 100;
const int MAX_NAME_LEN		= 50;

//////////////////////////////////////////////////////////////////////////
//	�ĺ��� Class
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
		//	�ĺ����� �̸��� ������ ����
		return strncmp(m_strName, kCandidate.m_strName, MAX_NAME_LEN) == 0 ? true : false;
	}

	bool operator<(const CCandidate& kCandidate)
	{
		//	��ǥ�� �������� ������ ����
		if (m_nVote > kCandidate.m_nVote)
		{
			return true;
		}
		else if (m_nVote < kCandidate.m_nVote)
		{	
			return false;
		}

		//	�̸� �������� ������ ����
		if (strncmp (m_strName, kCandidate.m_strName, MAX_NAME_LEN) < 0)
		{
			return true;
		}

		return false;
	}

	bool operator>(const CCandidate& kCandidate)
	{
		//	��ǥ�� �������� ������ ����
		if (m_nVote < kCandidate.m_nVote)
		{
			return true;
		}
		else if (m_nVote > kCandidate.m_nVote)
		{
			return false;
		}

		//	�̸� �������� ������ ����
		if (strncmp (m_strName, kCandidate.m_strName, MAX_NAME_LEN) > 0)
		{
			return true;
		}

		return false;
	}
};

//////////////////////////////////////////////////////////////////////////
//	Quick Sort
//	�ĺ��� ����Ʈ�� ����
//	��ǥ�� : ��������
//	�̸� : ��������
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
//	�ĺ��� ����Ʈ�� �̸����� ���ĵǾ��ٴ� �����Ͽ� ��ǥ�� ���
void MergeByName (CCandidate* pkArrCandidate, int nSize)
{
	if (nSize == 1)
	{
		return;
	}

	CCandidate*	pkCandidate	= &pkArrCandidate[0];
	for (int nIndex = 1 ; nIndex < nSize ; nIndex++)
	{
		//	�̸��� ������ ��ǥ���� �÷��ְ�, �ߺ��� �ĺ��� ���� ����
		if (*pkCandidate == pkArrCandidate[nIndex])
		{
			pkCandidate->m_nVote++;
			pkArrCandidate[nIndex].Init();
		}

		//	�̸��� �ٸ��� �ٸ� �ĺ����� ��ǥ�� ���
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