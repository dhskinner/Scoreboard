// ButtonWithInterrupt.h
// Assumes that an interrupt is triggered externally
// When the interrupt is triggered, a byte is passed to the button that then debounces
// based on whichever bit it has been set to monitor. For example, a port expander or an arduino port 

#ifndef _BUTTON_MANAGER_h
#define _BUTTON_MANAGER_h

#define BUTTON_PRESSED	(uint8_t) LOW
#define BUTTON_RELEASED	(uint8_t) HIGH
#define BIT_READ(byte_to_check, bit_to_check)	((byte_to_check & (1 << bit_to_check)) ? BUTTON_PRESSED : BUTTON_RELEASED)

#include <Arduino.h>
#include <functional>
#include <List.h>
#include <Ticker.h>

typedef std::function<void(int arg1)> ButtonHandler;

class Button
{
protected:
	uint8_t			_bitToMonitor;
	ButtonHandler	_pressedHandler;
	ButtonHandler	_releasedHandler;
	int				_handlerValue;
	uint8_t			_currentState;
	unsigned long	_currentStateChangedMillis;
	bool			_isChanged;

public:
	inline Button(uint8_t bitToMonitor, ButtonHandler pressedHandler, ButtonHandler releasedHandler, int handlerValue)
	{
		_bitToMonitor = bitToMonitor;
		_pressedHandler = pressedHandler;
		_releasedHandler = releasedHandler;
		_handlerValue = handlerValue;
		_currentState = BUTTON_RELEASED;
		_currentStateChangedMillis = 0;
		_isChanged = false;
	};

	inline bool update(byte stateByte)
	{
		uint8_t newState = BIT_READ(stateByte, _bitToMonitor);

		if (newState != _currentState)
		{
			_currentStateChangedMillis = millis();
			_currentState = newState;
			_isChanged = true;

			if (_currentState == BUTTON_PRESSED)
				_pressedHandler(_handlerValue);
			else
				_releasedHandler(_handlerValue);
		}
		return false;
	};

	// tells you if a button has changed after calling the read() function
	bool isChanged()
	{
		if (_isChanged == true)
		{
			_isChanged = false;
			return true;
		}
		return false;
	}

	// has the button gone from off -> on
	bool pressed()
	{
		if (_currentState == BUTTON_PRESSED && isChanged() == true)
			return true;
		else
			return false;
	}

	// has the button gone from on -> off
	bool released()
	{
		if (_currentState == BUTTON_RELEASED && isChanged() == true)
			return true;
		else
			return false;
	}
};

class ButtonManager
{
protected:
	Ticker			_debounceTicker;
	unsigned long	_debounceMillis;
	unsigned long	_longPressMillis;
	byte			_updateState;
	unsigned long	_updateMillis;

public:
	List <Button*> buttons;
	inline ButtonManager(unsigned long debounceMillis = 25, unsigned long longPressMillis = 1000)
		: _debounceMillis(debounceMillis)
		, _longPressMillis(longPressMillis)
	{
	};

	// As bytes are received - keep resetting the timer until a stable state
	// is received for longer than the debounce delay
	inline void update(uint8_t state)
	{
		_updateMillis = millis();
		_updateState = state;
		_debounceTicker.detach();
		_debounceTicker.once_ms(_debounceMillis, this->debounce);
	}

protected:
	inline void debounce()
	{
		Button *button;

		// Button has been in a stable state for the debounce delay
		// Loop through each button handler to see if there is anything to do
		buttons.Begin();
		while (!buttons.IsEnd())
		{
			button = buttons.GetValue();
			button->update(_updateState);
			buttons.Next();
		}
	}
};
#endif