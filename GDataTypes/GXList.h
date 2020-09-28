//////////////////////////////////////////////////////////////////////////
// gxlist.h
// created by cyl 20080425
// GetHeadAs/GetTailAs/GetAtAs 为成员模板函数，附带完成强制转型操作。
// 使用范例：
// list<TYPE_NAME>::iterator& itr = gxlist.GetAtAs<TYPE_NAME>(pos);
// TYPE_NAME为转型后的类型。
// 注意：这种转型存在风险，现在没有做转型成功检测，只能用在T为GenerationData时。
//////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _GXLIST_H_
#define _GXLIST_H_

#include <list>
using namespace std;

template<typename T>
class GXList : public list<T>
{
    public:
    typedef typename list<T>::iterator iterator;
    private:
    T t;
    iterator m_itr;
public:


  GXList(void) {};
  ~GXList(void) {};

  iterator&	GetAt(int pos);				//取得任意位置节点，从0开始
  int		AddTail();					//在队尾加一个新空元素
  iterator&	GetHead();					//取得头结点元素
  iterator&	GetTail();					//取得尾结点元素

  // Can only be used on GenerationData objects NOW!
  template<typename TX>
  typename list<TX>::iterator& GetHeadAs();
  
  template<typename TX>
  typename list<TX>::iterator& GetTailAs();

  template<typename TX>
  typename list<TX>::iterator& GetAtAs(int pos);

	
  void RemoveAll();					//删除所有元素
  void RemoveTail();					//删除队尾巴元素
  void RemoveHead();					//删除队列头元素

  void RemoveAt(iterator& itr);

  // PAY ATTENTION ON the following method.
  // Once an item in list removed, another will take the place
  // of the dead one.
  // So do not call it in a for or while loop by passing a
  // variable position.
  // e.g.  for(int i = 0; i < n; ++i) RemoveAt(i);
  // THIS WILL NOT WORK AS YOU WANT!!!
  // USE vector or other things to instead.
  void RemoveAt(int pos);
                                                          
  size_t GetCount(){ return list<T>::size(); }	//取得元素总数

  bool IsAtList(T& t);
  typename list<T>::iterator GetIterator(T& t);	// 查找元素并返回iterator
};

template<typename T,
	 template <typename> class Container,
	 typename fnc>
inline fnc& do_each_on(Container<T>& _cont, 
		       typename Container<T>::iterator& _first,
		       typename Container<T>::iterator& _last,
		       fnc& _fnc)
{
    typename Container<T>::iterator cur;
    for(; _first != _last;)
    {
        cur = _first;
        ++_first;
        _fnc(_cont, cur);     
    }

    return _fnc;
};

template<typename T,
class Container,
template <typename> class fnc>
inline fnc<T>& do_each_on_t(Container& _cont, 
                       typename Container::iterator& _first,
                       typename Container::iterator& _last,
                       fnc<T>& _fnc)
{
    for(; _first != _last; ++_first)
    {
        _fnc(_cont, _first);
    }

    return _fnc;
};

template<typename T>
struct fnc_RemoveListItem
{
  fnc_RemoveListItem<T>& operator ()(list<T>& l, typename list<T>::iterator& itr)
  {
    l.erase(itr);

    return *this;
  };
};

template<typename T>
typename GXList<T>::iterator& GXList<T>::GetAt(int pos)
{
  m_itr = list<T>::begin();

  while(m_itr != list<T>::end() && pos > 0)
    {
      ++m_itr;
      --pos;
    }

  return m_itr;
};
template<typename T>
int GXList<T>::AddTail()
{
  list<T>::push_back(t);

  return (int)(list<T>::size());
};
template<typename T>
typename GXList<T>::iterator& GXList<T>::GetHead()
{
  m_itr = list<T>::begin();

  return m_itr;
};
template<typename T>
typename GXList<T>::iterator& GXList<T>::GetTail()
{
    m_itr =  (list<T>::end());

    if(list<T>::size() > 0)
    {
        --m_itr;
    }

  return m_itr;
};

template<typename T>
void GXList<T>::RemoveAll()
{
  // list<T>::clear();
    list<T>::clear();
};

template<typename T>
void GXList<T>::RemoveHead()
{
    if(list<T>::begin() != list<T>::end()) erase(list<T>::begin());
};

template<typename T>
void GXList<T>::RemoveTail()
{
    if(list<T>::begin() != list<T>::end()) list<T>::erase(--(list<T>::end()));
};

template<typename T>
void GXList<T>::RemoveAt(iterator& itr)
{
  list<T>::erase(itr);
};
template<typename T>
void GXList<T>::RemoveAt(int pos)
{
  iterator itr;
  itr = list<T>::begin();

  while(itr != list<T>::end() && pos > 0)
    {
      ++itr;
      --pos;
    }

  RemoveAt(itr);
};

template<typename T>
  template<typename TX>
typename list<TX>::iterator& GXList<T>::GetHeadAs()
{
  m_itr = list<T>::begin();

  typename list<T>::iterator* p = &(m_itr);
  typename list<TX>::iterator* px = (typename list<TX>::iterator*)p;

  return *px;
};

template<typename T>
template<typename TX>
typename list<TX>::iterator& GXList<T>::GetTailAs()
{        
    m_itr =  (list<T>::end());

    if(list<T>::size() > 0)
    {
        --m_itr;
    }

    typename list<T>::iterator* p = &(m_itr);
    typename list<TX>::iterator* px = (typename list<TX>::iterator*)p;

    return *px;
};

template<typename T>
template<typename TX>
typename list<TX>::iterator& GXList<T>::GetAtAs(int pos)
{
    GetAt(pos);

    typename list<T>::iterator* p = &(m_itr);
    typename list<TX>::iterator* px = (typename list<TX>::iterator*)p;

    return *px;
};

template<typename T>
typename GXList<T>::iterator GXList<T>::GetIterator(T& t)
{
    m_itr = list<T>::begin();

    for(;m_itr != list<T>::end(); ++m_itr)
    {
        if(*m_itr == t) 
        {
           break;
        }
    }
    return m_itr;
};

template<typename T>
bool GXList<T>::IsAtList(T& t)
{
	typename list<T>::iterator itr;
	itr = list<T>::begin();

	for(;itr != list<T>::end(); ++itr)
	{
		if(*itr == t)
		{
			return true;
		}
	}
	return false;
};


#endif
