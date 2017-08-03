#include "ButtonManager.h"

/*
ButtonManager::ButtonManager(unsigned long debounce_millis, unsigned long short_press_millis, unsigned long long_press_millis)
	: _bit_to_monitor(bit_to_monitor)
	, _byte_to_monitor(0)
	, _last_update_bit(0)
	, _last_update_time(0)
	, _debounce_delay(debounce_delay)
	, _current_state(HIGH)
	, _ignore_until_millis(0)
	, _has_changed(false)
{
}
*/




bool ButtonManager::internal_update()
{
	// ignore pin changes until after this delay time
	if (_ignore_until_millis > millis())
	{
		// ignore any changes during this period
		return _current_state;
	}
	else if (_last_update_bit != _current_state) 	// pin has changed 
	{
		_current_state = !_last_update_bit;
		_has_changed = true;
	}
	return _current_state;
}

// has the button been toggled from on -> off, or vice versa
bool ButtonManager::toggled()
{
	internal_update();
	return has_changed();
}





