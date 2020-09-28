//////////////////////////////////////////////////////////////////////////
// GDataStream.h
// created by zhangluyuan 20080815
// for GDataStream
//////////////////////////////////////////////////////////////////////////

#ifndef GDATASTREAM_H
#define GDATASTREAM_H

#pragma once

#include <list>
#include "GData.h"

// Ӧ�ò�����������
class GenerationDataStream : public GenerationData	//GenerationData�б��ࣨ�������б�list<GenerationData*>д�������ڴ���ʽ��
{
public:
	inline GenerationDataStream();
	inline ~GenerationDataStream();
	inline bool CreateFromList(const std::list<GenerationData*> & GDataList);	//��list<GenerationData*>����
	inline GHRESULT RevertToList(std::list<GenerationData*> & GDataList) const;		//��ԭ��list<GenerationData*>,��listβ׷��,Ҫ���list����ReleaseGDataList����
	inline static void ReleaseGDataList(std::list<GenerationData*> & GDataList);//�ͷ�list<GenerationData*>
	inline bool AddGData(const GenerationData* pGData);							//��GDataStream��׷��һ��GData
	inline bool AddList(const std::list<GenerationData*> & GDataList);			//��GDataStream��׷��GDataList
private:
	//inline......
};

#include "GDataStream.inl"
#endif //GDATASTREAM_H
