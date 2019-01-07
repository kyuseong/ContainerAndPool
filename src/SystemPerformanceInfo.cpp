#include "pch.h"
#include "systemperformanceinfo.h"
#include <assert.h>

cSystemPerformanceInfo::cSystemPerformanceInfo(const std::wstring& pszName, const std::wstring& pszParentName) :
	m_pParent(NULL),
	m_nHistorySize(0),
	m_nHistoryElements(0),
	m_nHistoryHead(0),
	m_nStartRefCount(0),
	m_nReportingFlags(0),
	m_nNumChildren(0)
{
	m_pszName = pszName;
	m_pszParentName = pszParentName;

	// 리셋
	ResetTimings();
}

cSystemPerformanceInfo::~cSystemPerformanceInfo()
{
}

// 시작 할때 호출 
void cSystemPerformanceInfo::BeginTiming(t_time CurrTime)
{
	// 횟수
	m_nStartRefCount++;
	m_nFrameEntries++;
	m_nTotalEntries++;

	if(m_nStartRefCount == 1)
	{
		// 처음
		m_LastUpdateTime = CurrTime;
	}
	else
	{
		ResumeTiming(CurrTime);
	}
}

// 종료 할때 호출
void cSystemPerformanceInfo::EndTiming(t_time CurrTime)
{
	assert(m_nStartRefCount > 0);
	m_nStartRefCount--;

	// 
	if(m_nStartRefCount == 0)
	{
		t_duration dura = std::chrono::duration_cast<t_duration>(CurrTime - m_LastUpdateTime);
		double fTimeElapsedMS = dura.count();
		m_fFrameTime += fTimeElapsedMS;
		m_fTotalTime += fTimeElapsedMS;
	}
	else
	{
		PauseTiming(CurrTime);
	}
}

void cSystemPerformanceInfo::PauseTiming(t_time CurrTime)
{
	t_duration dura = std::chrono::duration_cast<t_duration>(CurrTime - m_LastUpdateTime);
	double fTimeElapsedMS = dura.count();

	m_fFrameTime += fTimeElapsedMS;
	m_fTotalTime += fTimeElapsedMS;

	m_LastUpdateTime = CurrTime;
}

void cSystemPerformanceInfo::ResumeTiming(t_time CurrTime)
{
	t_duration dura = std::chrono::duration_cast<t_duration>(CurrTime - m_LastUpdateTime);
	double fTimeElapsedMS = dura.count();
	m_fNestedFrameTime += fTimeElapsedMS;
	m_fTotalNestedTime += fTimeElapsedMS;

	m_LastUpdateTime = CurrTime;
}

// 히스토리 사이즈 셋팅
bool cSystemPerformanceInfo::SetHistorySize(size_t nSize)
{
	m_FrameTimeHistory.resize(nSize);
	m_NestedFrameTimeHistory.resize(nSize);
	m_FrameEntriesHistory.resize(nSize);

	m_nHistorySize = nSize;
	m_nHistoryHead = 0;
	m_nHistoryElements = 0;

	return true;
}

void cSystemPerformanceInfo::HandleFrameEvent()
{
	if(m_nHistorySize > 0)
	{
		m_FrameTimeHistory[m_nHistoryHead]			= m_fFrameTime;
		m_NestedFrameTimeHistory[m_nHistoryHead]	= m_fNestedFrameTime;
		m_FrameEntriesHistory[m_nHistoryHead]		= m_nFrameEntries;

		if(m_nHistoryElements < m_nHistorySize)
			m_nHistoryElements++;

		if(GetMaxFrameEntries() > 1000)
			uint32_t nBreak = 1;

		m_nHistoryHead = (m_nHistoryHead + 1) % m_nHistorySize;
	}

	m_fFrameTime		= 0.0;
	m_fNestedFrameTime	= 0.0;
	m_nFrameEntries		= 0;
}

void cSystemPerformanceInfo::ResetTimings()
{
	m_fFrameTime		= 0.0;
	m_fNestedFrameTime	= 0.0;
	m_nFrameEntries		= 0;

	m_fTotalTime		= 0.0;
	m_fTotalNestedTime	= 0.0;
	m_nTotalEntries		= 0;

	m_nHistoryElements	= 0;
	m_nHistoryHead		= 0;
}

void cSystemPerformanceInfo::SetParent(cSystemPerformanceInfo* pParent)
{
	if(m_pParent)
		m_pParent->m_nNumChildren--;

	m_pParent = pParent;

	if(m_pParent)
		m_pParent->m_nNumChildren++;
}

bool cSystemPerformanceInfo::IsChildOf(const cSystemPerformanceInfo* pInfo) const
{
	const cSystemPerformanceInfo* pCurrNode = m_pParent;
	while(pCurrNode)
	{
		if(pCurrNode == pInfo)
			return true;
		pCurrNode = pCurrNode->GetParent();
	}

	return false;
}

uint32_t cSystemPerformanceInfo::GetChildLevel() const
{
	if(!m_pParent)
		return 0;

	return 1 + m_pParent->GetChildLevel();
}

const cSystemPerformanceInfo* cSystemPerformanceInfo::GetParentLevel(uint32_t nLevel) const
{
	if(GetChildLevel() <= nLevel)
		return this;

	return m_pParent->GetParentLevel(nLevel);
}

// 평균 프레임 시간
double cSystemPerformanceInfo::GetAvgFrameTime() const
{
	double fAccumulator = m_fFrameTime;
	for(uint32_t nCurrHistory = 0; nCurrHistory < m_nHistoryElements; nCurrHistory++)
		fAccumulator += m_FrameTimeHistory[nCurrHistory];
	return fAccumulator / (double)(m_nHistoryElements + 1);
}

// 현재 네스티드 프레임 시간
double cSystemPerformanceInfo::GetAvgNestedFrameTime() const
{
	double fAccumulator = m_fNestedFrameTime;
	for(uint32_t nCurrHistory = 0; nCurrHistory < m_nHistoryElements; nCurrHistory++)
		fAccumulator += m_NestedFrameTimeHistory[nCurrHistory];

	return fAccumulator / (double)(m_nHistoryElements + 1);
}

double cSystemPerformanceInfo::GetAvgFrameEntries() const
{
	uint64_t nAccumulator = m_nFrameEntries;
	for(uint32_t nCurrHistory = 0; nCurrHistory < m_nHistoryElements; nCurrHistory++)
		nAccumulator += m_FrameEntriesHistory[nCurrHistory];

	return (double)nAccumulator / (double)(m_nHistoryElements + 1);
}

double cSystemPerformanceInfo::GetMaxFrameTime() const
{
	double fMax = m_fFrameTime;
	for(uint32_t nCurrHistory = 0; nCurrHistory < m_nHistoryElements; nCurrHistory++)
		fMax = std::fmax(fMax, m_FrameTimeHistory[nCurrHistory]);
	return fMax;
}

double cSystemPerformanceInfo::GetMaxNestedFrameTime() const
{
	double fMax = m_fNestedFrameTime;
	for(uint32_t nCurrHistory = 0; nCurrHistory < m_nHistoryElements; nCurrHistory++)
		fMax = std::fmax(fMax, m_NestedFrameTimeHistory[nCurrHistory]);
	return fMax;
}

uint64_t cSystemPerformanceInfo::GetMaxFrameEntries() const
{
	uint64_t nMax = m_nFrameEntries;
	for(uint32_t nCurrHistory = 0; nCurrHistory < m_nHistoryElements; nCurrHistory++)
		nMax = std::fmax(nMax, m_FrameEntriesHistory[nCurrHistory]);
	return nMax;
}

void cSystemPerformanceInfo::SetReportingFlag(uint32_t nFlag, bool bSet)
{
	if(bSet)
		m_nReportingFlags |= nFlag;
	else
		m_nReportingFlags &= ~nFlag;
}
