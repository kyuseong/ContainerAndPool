#include "pch.h"
#include "systemperformancemgr.h"
#include "systemperformanceinfo.h"
#include <algorithm>
#include <assert.h>

const uint32_t DEFAULTHISTORYSIZE = 60;

cSystemPerformanceMgr::cSystemPerformanceMgr()
{
	// 기본 히스토리 사이즈
	m_nHistorySize = DEFAULTHISTORYSIZE;

	m_Registered.reserve(128);
	m_TrackingStack.reserve(128);
}

cSystemPerformanceMgr::~cSystemPerformanceMgr()
{
	for(auto Ptr : m_Registered)
	{
		delete Ptr;
	}
}

// singleton
cSystemPerformanceMgr& cSystemPerformanceMgr::GetSingleton()
{
	static cSystemPerformanceMgr s_Singleton;
	return s_Singleton;
}

// 새로운 정보 등록 
cSystemPerformanceInfo*	cSystemPerformanceMgr::RegisterSystem(const std::wstring& pszSystemName, const std::wstring& pszParentName)
{
	cSystemPerformanceInfo* pNowSystem = GetSystem(pszSystemName);

	if(pNowSystem)
	{
		return pNowSystem;
	}

	cSystemPerformanceInfo* pNewSystem = new cSystemPerformanceInfo(pszSystemName, pszParentName);
	if(!pNewSystem)
		return NULL;

	if(!pNewSystem->SetHistorySize(m_nHistorySize))
	{
		delete pNewSystem;
		return NULL;
	}

	if (pszParentName.empty() == false)
	{
		cSystemPerformanceInfo* pParent = GetSystem(pszParentName);
		if (pParent)
			pNewSystem->SetParent(pParent);
	}

	for(cSystemPerformanceInfo* pChild : m_Registered)
	{
		if(	(pChild->GetParent() == nullptr) && 
			pChild->GetParentName().empty() == false &&
			pChild->GetParentName() == pszSystemName)
		{
			pChild->SetParent(pNewSystem);
		}
	}
		
	m_Registered.push_back(pNewSystem);
	return pNewSystem;
}

cSystemPerformanceInfo*	cSystemPerformanceMgr::GetSystem(const std::wstring& pszSystemName)
{
	for(auto Info : m_Registered)
	{
		if(pszSystemName == Info->GetName())
			return Info;
	}

	return nullptr;
}

void cSystemPerformanceMgr::HandleFrameEvent()
{
	std::for_each(m_Registered.begin(), m_Registered.end(), [](auto Ptr) {Ptr->HandleFrameEvent(); });
}

void cSystemPerformanceMgr::ResetTimings()
{
	std::for_each(m_Registered.begin(), m_Registered.end(), [](auto Ptr) {Ptr->ResetTimings(); });
}

void cSystemPerformanceMgr::SetHistorySize(uint32_t nSize)
{
	m_nHistorySize = nSize;
	std::for_each(m_Registered.begin(), m_Registered.end(), [nSize](auto Ptr) {Ptr->SetHistorySize(nSize); });
}

void cSystemPerformanceMgr::BeginTiming(cSystemPerformanceInfo* pInfo)
{
	t_time CurrTime = GetPrecisionTime();

	if(!m_TrackingStack.empty())
		m_TrackingStack[m_TrackingStack.size() - 1]->PauseTiming(CurrTime);

	m_TrackingStack.push_back(pInfo);

	pInfo->BeginTiming(CurrTime);
}

void cSystemPerformanceMgr::EndTiming(cSystemPerformanceInfo* pInfo)
{
	if(m_TrackingStack.empty())
	{
		assert(!"비어 있네?");
		return;
	}

	if(pInfo != m_TrackingStack[m_TrackingStack.size() - 1])
	{
		assert(!"잘못 씀");

		auto itFind = std::find(m_TrackingStack.begin(), m_TrackingStack.end(), pInfo);
		if(itFind != m_TrackingStack.end())
		{
			while(m_TrackingStack[m_TrackingStack.size() - 1] != pInfo)
				m_TrackingStack.pop_back();
		}
		else
		{
			return;
		}
	}

	t_time CurrTime = GetPrecisionTime();

	pInfo->EndTiming(CurrTime);

	m_TrackingStack.pop_back();

	if(!m_TrackingStack.empty())
		m_TrackingStack[m_TrackingStack.size() - 1]->ResumeTiming(CurrTime);

}


