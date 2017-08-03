#ifndef _LIST_h
#define _LIST_h

/*
List.h - Linked List (single chain)
Created by B. Blechschmidt (Ovex), 16 August 2013
Released into the public domain.
*/
template <class T> class ListElement
{
public:
	ListElement(T Element)
	{
		Content = Element;
		NextElement = NULL;
	}
	~ListElement()
	{
		delete NextElement;
	}
	T Content;
	ListElement<T> *NextElement;
};

class ListClass
{
 protected:


 public:
	void init();
};

extern ListClass List;

template <class T> class List
{
public:
	~List()
	{
		Clear();
		delete FirstElement;
		delete LastElement;
		delete CurrentElement;
	}
	List()
	{
		FirstElement = NULL;
		CurrentElement = NULL;
		LastElement = NULL;
		Length = 0;
	}
	void Clear()
	{
		Begin();
		delete CurrentElement;
		FirstElement = NULL;
		CurrentElement = NULL;
		LastElement = NULL;
		Length = 0;
	}
	void Push(T Element)
	{
		if (LastElement)
		{
			LastElement->NextElement = new ListElement<T>(Element);
			LastElement = LastElement->NextElement;
		}
		else
		{
			FirstElement = new ListElement<T>(Element);
			LastElement = FirstElement;
			CurrentElement = FirstElement;
		}
		Length++;
	}
	bool IsEnd()
	{
		if (CurrentElement)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	bool IsEmpty()
	{
		return FirstElement == NULL;
	}
	unsigned long Count()
	{
		return Length;
	}
	void Begin()
	{
		CurrentElement = FirstElement;
	}
	void Next()
	{
		if (CurrentElement)
		{
			CurrentElement = CurrentElement->NextElement;
		}
	}
	T GetValue()
	{
		return CurrentElement->Content;
	}
private:
	unsigned long Length;
	ListElement<T> *FirstElement;
	ListElement<T> *LastElement;
	ListElement<T> *CurrentElement;
};

#endif

