//-------------------------------------------------------------------------------
// SystemPerformanceMgr.h
//
// 시스템 성능 관리자
//
//-------------------------------------------------------------------------------

#ifndef __SYSTEMPERFORMANCEMGR_H__
#define __SYSTEMPERFORMANCEMGR_H__

#include "systemperformanceinfo.h"

inline t_time GetPrecisionTime()
{
	return std::chrono::high_resolution_clock::now();
}

class cSystemPerformanceMgr
{

private:
	cSystemPerformanceMgr();

	// 등록된 리스트
	using TSystemInfoList = std::vector<cSystemPerformanceInfo*>;
	TSystemInfoList		m_Registered;

	// 현재
	TSystemInfoList		m_TrackingStack;

	// 사이즈
	uint32_t			m_nHistorySize;
public:

	//singleton
	static cSystemPerformanceMgr& GetSingleton();

	~cSystemPerformanceMgr();

	// 새로운 정보 등록 
	cSystemPerformanceInfo*	RegisterSystem(const std::wstring& pszSystemName, const std::wstring& pszParentName);

	// 이름으로 정보 찾기
	cSystemPerformanceInfo*	GetSystem(const std::wstring& pszSystemName);

	// 갯수
	uint32_t					GetNumSystems() const				{ return (uint32_t)m_Registered.size(); }
	cSystemPerformanceInfo* GetSystem(uint32_t nIndex) 			{ return m_Registered[nIndex]; }

	// 이벤트 처리
	void HandleFrameEvent();

	// 모든 통계 시간 리셋
	void ResetTimings();

	// 남기는 사이즈 셋팅하기
	void SetHistorySize(uint32_t nSize);

	// 시작할때 호출
	void BeginTiming(cSystemPerformanceInfo* pInfo);

	// 끝날때 호출
	void EndTiming(cSystemPerformanceInfo* pInfo);

	// 
	TSystemInfoList& GetRegisteredList() { return m_Registered; }
};

#endif
