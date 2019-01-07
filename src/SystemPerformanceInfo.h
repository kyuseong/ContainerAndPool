//--------------------------------------------------------------------------------
// SystemPerformanceInfo.h
//
// �����ս� Ʈ��ŷ �ý���
// 
//--------------------------------------------------------------------------------

#ifndef __SYSTEMPERFORMANCEINFO_H__
#define __SYSTEMPERFORMANCEINFO_H__

#include <vector>
#include <stdint.h>
#include <chrono>

using t_time = std::chrono::high_resolution_clock::time_point;
using t_duration = std::chrono::milliseconds;

// ���� ����
class cSystemPerformanceInfo
{

private:
	// ���̸�
	std::wstring m_pszName;
	// �θ� �̸�
	std::wstring m_pszParentName;
	// �θ� 
	cSystemPerformanceInfo*	m_pParent;

	// ������ �ð�
	double	m_fFrameTime;
	double	m_fNestedFrameTime;
	uint64_t	m_nFrameEntries;

	// �����丮
	std::vector<double>	m_FrameTimeHistory;
	std::vector<double>	m_NestedFrameTimeHistory;
	std::vector<uint64_t>	m_FrameEntriesHistory;

	// �����丮 ������
	uint32_t	m_nHistorySize;

	// ���� ���
	uint32_t	m_nHistoryHead;

	// ��
	uint32_t	m_nHistoryElements;

	// ��ü ����
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
	// �ð�
	//--------------------------------------------------------

	// ���� �Ҷ� ȣ�� 
	void BeginTiming(t_time CurrTime);

	// ���� �Ҷ� ȣ��
	void EndTiming(t_time CurrTime);

	// Pause
	void PauseTiming(t_time CurrTime);

	// Resume
	void ResumeTiming(t_time CurrTime);

	//--------------------------------------------------------
	// Event handling
	//--------------------------------------------------------

	// �����丮 ������ ����
	bool SetHistorySize(size_t nSize);
	// 
	void HandleFrameEvent();

	// �ð� �� �����ϱ�
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
