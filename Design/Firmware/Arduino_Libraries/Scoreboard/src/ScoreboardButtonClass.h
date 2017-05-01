#ifndef _BUTTON_HANDLER_H_
#define _BUTTON_HANDLER_H_

#ifdef ESP8266

#include <Arduino.h>
#include <functional>
#include <ScoreboardDefines.h>
#include <WSUtility.h>

//#define BUTTON_DEBUG

/*
List.h - Linked List (single chain)
Created by B. Blechschmidt (Ovex), 16 August 2013
Released into the public domain.
*/
// ---------------------------------------------------------------------------------
template <class T> class ListElement
{
public:
    ListElement ( T Element )
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
// ---------------------------------------------------------------------------------
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
    void Push ( T Element )
    {
        if ( LastElement )
        {
            LastElement->NextElement = new ListElement<T> ( Element );
            LastElement = LastElement->NextElement;
        }
        else
        {
            FirstElement = new ListElement<T> ( Element );
            LastElement = FirstElement;
            CurrentElement = FirstElement;
        }
        Length++;
    }
    bool IsEnd()
    {
        if ( CurrentElement )
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
        if ( CurrentElement )
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
// ---------------------------------------------------------------------------------
class ScoreboardButtonClass
{
public:
    ScoreboardButtonClass ( uint8_t bpin, ButtonHandlerFunction bfunction = NULL, int bfunctionvalue = 0, unsigned long bmillis = 0, unsigned long brepeatmillis = 0 );
    uint8_t Pin();
	int Value();
	bool IsPressed();
	bool IsReleased();
	bool IsIdle();
    bool JustToggled();
    bool JustPressed();
    bool JustReleased();
	bool Process(uint8_t pinstate);

private:
	// constants used internally by button state machine
	enum ButtonState : uint8_t
	{
		DEBOUNCE,
		JUSTPRESS,
		DELAYPRESS,
		PRESS,
		JUSTRELEASE,
		IDLE
	};
	const static bool PRESSED = 0;
	const static bool RELEASED = 1;

	uint8_t _pin;

	bool _currentReading;
	bool _previousReading;
	
	ButtonState _state;
	unsigned long _delay;
	unsigned long _repeatDelay;
	unsigned long _stateTimer;
	ButtonHandlerFunction _functionPointer;
	int _functionValue;
};
// ---------------------------------------------------------------------------------
#endif
#endif
