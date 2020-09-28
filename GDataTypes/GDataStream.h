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

// 应用层请求数据体
class GenerationDataStream : public GenerationData	//GenerationData列表类（将请求列表list<GenerationData*>写成连续内存形式）
{
public:
	inline GenerationDataStream();
	inline ~GenerationDataStream();
	inline bool CreateFromList(const std::list<GenerationData*> & GDataList);	//从list<GenerationData*>创建
	inline GHRESULT RevertToList(std::list<GenerationData*> & GDataList) const;		//还原成list<GenerationData*>,在list尾追加,要清空list调用ReleaseGDataList即可
	inline static void ReleaseGDataList(std::list<GenerationData*> & GDataList);//释放list<GenerationData*>
	inline bool AddGData(const GenerationData* pGData);							//在GDataStream后追加一个GData
	inline bool AddList(const std::list<GenerationData*> & GDataList);			//在GDataStream后追加GDataList
private:
	//inline......
};

#include "GDataStream.inl"
#endif //GDATASTREAM_H
