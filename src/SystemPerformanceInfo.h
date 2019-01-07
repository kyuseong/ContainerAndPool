//--------------------------------------------------------------------------------
// SystemPerformanceInfo.h
//
// 퍼포먼스 트래킹 시스템
// 
//--------------------------------------------------------------------------------

#ifndef __SYSTEMPERFORMANCEINFO_H__
#define __SYSTEMPERFORMANCEINFO_H__

#include <vector>
#include <stdint.h>
#include <chrono>

using t_time = std::chrono::high_resolution_clock::time_point;
using t_duration = std::chrono::milliseconds;

// 성능 정보
class cSystemPerformanceInfo
{

private:
	// 내이름
	std::wstring m_pszName;
	// 부모 이름
	std::wstring m_pszParentName;
	// 부모 
	cSystemPerformanceInfo*	m_pParent;

	// 프레임 시간
	double	m_fFrameTime;
	double	m_fNestedFrameTime;
	uint64_t	m_nFrameEntries;

	// 리스토리
	std::vector<double>	m_FrameTimeHistory;
	std::vector<double>	m_NestedFrameTimeHistory;
	std::vector<uint64_t>	m_FrameEntriesHistory;

	// 히스토리 사이즈
	uint32_t	m_nHistorySize;

	// 현재 헤드
	uint32_t	m_nHistoryHead;

	// 수
	uint32_t	m_nHistoryElements;

	// 전체 값들
	double	m_fTotalTime;
	double	m_fTotalNestedTime;
	uint64_t	m_nTotalEntries;
	
	uint32_t	m_nReportingFlags;
	uint32_t	m_nStartRefCount;
	uint32_t	m_nNumChildren;
	t_time		m_LastUpdateTime;
public:
	cSystemPerformanceInfo(const std::wstring& pszName, const std::wstring& pszParentName);
	~cSystemPerformanceInfo();

	//--------------------------------------------------------
	// 시간
	//--------------------------------------------------------

	// 시작 할때 호출 
	void BeginTiming(t_time CurrTime);

	// 종료 할때 호출
	void EndTiming(t_time CurrTime);

	// Pause
	void PauseTiming(t_time CurrTime);

	// Resume
	void ResumeTiming(t_time CurrTime);

	//--------------------------------------------------------
	// Event handling
	//--------------------------------------------------------

	// 히스토리 사이즈 셋팅
	bool SetHistorySize(size_t nSize);
	// 
	void HandleFrameEvent();

	// 시간 다 리셋하기
	void ResetTimings();

	bool		HasChildren() const						{ return m_nNumChildren > 0; }

	bool		IsChildOf(const cSystemPerformanceInfo* pInfo) const;

	uint32_t		GetChildLevel() const;

	const cSystemPerformanceInfo*	GetParentLevel(uint32_t nLevel) const;

	//the name and parent of this system
	const std::wstring&	GetName() const							{ return m_pszName; }
	const std::wstring& GetParentName() const					{ return m_pszParentName; }

	void SetParent(cSystemPerformanceInfo* pParent);
	cSystemPerformanceInfo* GetParent()					{ return m_pParent; }
	const cSystemPerformanceInfo* GetParent() const		{ return m_pParent; }

	double	GetFrameTime() const						{ return m_fFrameTime; }
	double	GetNestedFrameTime() const					{ return m_fNestedFrameTime; }
	uint64_t	GetFrameEntries() const						{ return m_nFrameEntries; }
	
	double	GetAvgFrameTime() const;
	double	GetAvgNestedFrameTime() const;
	double	GetAvgFrameEntries() const;

	double	GetMaxFrameTime() const;
	double	GetMaxNestedFrameTime() const;
	uint64_t	GetMaxFrameEntries() const;

	double	GetTotalTime() const						{ return m_fTotalTime; }
	double	GetTotalNestedTime() const					{ return m_fTotalNestedTime; }
	uint64_t	GetTotalEntries() const						{ return m_nTotalEntries; }

	bool	IsReportingFlagSet(uint32_t nFlag) const		{ return (m_nReportingFlags & nFlag) != 0; }
	void	SetReportingFlag(uint32_t nFlag, bool bSet);

};

#endif
