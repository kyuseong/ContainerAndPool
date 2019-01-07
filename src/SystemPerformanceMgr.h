//-------------------------------------------------------------------------------
// SystemPerformanceMgr.h
//
// �ý��� ���� ������
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

	// ��ϵ� ����Ʈ
	using TSystemInfoList = std::vector<cSystemPerformanceInfo*>;
	TSystemInfoList		m_Registered;

	// ����
	TSystemInfoList		m_TrackingStack;

	// ������
	uint32_t			m_nHistorySize;
public:

	//singleton
	static cSystemPerformanceMgr& GetSingleton();

	~cSystemPerformanceMgr();

	// ���ο� ���� ��� 
	cSystemPerformanceInfo*	RegisterSystem(const std::wstring& pszSystemName, const std::wstring& pszParentName);

	// �̸����� ���� ã��
	cSystemPerformanceInfo*	GetSystem(const std::wstring& pszSystemName);

	// ����
	uint32_t					GetNumSystems() const				{ return (uint32_t)m_Registered.size(); }
	cSystemPerformanceInfo* GetSystem(uint32_t nIndex) 			{ return m_Registered[nIndex]; }

	// �̺�Ʈ ó��
	void HandleFrameEvent();

	// ��� ��� �ð� ����
	void ResetTimings();

	// ����� ������ �����ϱ�
	void SetHistorySize(uint32_t nSize);

	// �����Ҷ� ȣ��
	void BeginTiming(cSystemPerformanceInfo* pInfo);

	// ������ ȣ��
	void EndTiming(cSystemPerformanceInfo* pInfo);

	// 
	TSystemInfoList& GetRegisteredList() { return m_Registered; }
};

#endif
